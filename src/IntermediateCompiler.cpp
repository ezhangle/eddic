//=======================================================================
// Copyright Baptiste Wicht 2011.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include <string>

#include "IntermediateCompiler.hpp"

#include "MainDeclaration.hpp"
#include "Methods.hpp"
#include "StringPool.hpp"
#include "VisitorUtils.hpp"

#include "Context.hpp"
#include "FunctionContext.hpp"
#include "Variable.hpp"
#include "SemanticalException.hpp"
#include "IsImmediateVisitor.hpp"

#include "mangling.hpp"

#include "il/IntermediateProgram.hpp"
#include "il/Operands.hpp"
#include "il/Labels.hpp"
#include "il/Math.hpp"

#include "ast/Program.hpp"

using namespace eddic;

inline bool isImmediate(ASTValue& value){
   return boost::apply_visitor(IsImmediateVisitor(), value); 
}

inline Operation toOperation(char op){
    switch(op){
        case '+':
            return Operation::ADD;
        case '-':
            return Operation::SUB;
        case '/':
            return Operation::DIV;
        case '*':
            return Operation::MUL;
        case '%':
            return Operation::MOD;
        default:
            assert(false);
    }
}

inline void putInRegister(ASTValue& value, std::shared_ptr<Operand> operand, IntermediateProgram& program);

inline std::shared_ptr<Operand> performIntOperation(ASTComposedValue& value, IntermediateProgram& program){
    assert(value.Content->operations.size() > 0); //This has been enforced by previous phases

    auto registerA = program.registers(EAX);
    auto registerB = program.registers(EBX);

    putInRegister(value.Content->first, registerA, program);

    //Apply all the operations in chain
    for(auto& operation : value.Content->operations){
        if(isImmediate(operation.get<1>())){
            putInRegister(operation.get<1>(), registerB, program);

            //Perform the operation 
            program.addInstruction(program.factory().createMath(toOperation(operation.get<0>()), registerA, registerB));
        } else { //The right value is composed
            program.addInstruction(program.factory().createPush(registerA)); //To be sure that the right operation does not override our register 
            
            putInRegister(operation.get<1>(), registerB, program);
            program.addInstruction(program.factory().createMove(createStackOperand(0), registerA));
            
            program.addInstruction(program.factory().createMath(toOperation(operation.get<0>()), registerA, registerB));
        }
    }

    return registerB;
}

inline std::pair<std::shared_ptr<Operand>, std::shared_ptr<Operand>> performStringOperation(ASTComposedValue& value, IntermediateProgram& program);

class PushValue : public boost::static_visitor<> {
    private:
        IntermediateProgram& program;
    
    public:
        PushValue(IntermediateProgram& p) : program(p) {}

        void operator()(ASTLitteral& litteral){
            program.addInstruction(
                program.factory().createPush(
                    createImmediateOperand(litteral.label)
                )
            );
            
            program.addInstruction(
                program.factory().createPush(
                    createImmediateOperand(litteral.value.size() - 2)
                )
            );
        }

        void operator()(ASTInteger& integer){
            program.addInstruction(
                program.factory().createPush(
                    createImmediateOperand(integer.value)
                )
            );
        }

        void operator()(ASTVariable& variable){
            auto var = variable.Content->var;

            if(var->type() == Type::INT){
                program.addInstruction(
                    program.factory().createPush(
                        var->toIntegerOperand()
                    )
                );
            } else {
                auto operands = var->toStringOperand();

                program.addInstruction(
                    program.factory().createPush(
                        operands.first
                    )
                );
                
                program.addInstruction(
                    program.factory().createPush(
                        operands.second
                    )
                );
            }
        }

        void operator()(ASTComposedValue& value){
            Type type = GetTypeVisitor()(value);

            if(type == Type::INT){
                program.addInstruction(program.factory().createPush(performIntOperation(value, program)));
            } else if(type == Type::STRING){
                auto pair = performStringOperation(value, program);

                program.addInstruction(program.factory().createPush(pair.first));
                program.addInstruction(program.factory().createPush(pair.second));
            }
        } 
};

class AssignValueToOperand : public boost::static_visitor<> {
    private:
        std::shared_ptr<Operand> operand;
        IntermediateProgram& program;
    
    public:
        AssignValueToOperand(std::shared_ptr<Operand> op, IntermediateProgram& p) : operand(op), program(p) {}

        void operator()(ASTLitteral&){
            assert(false); //Cannot assign a string to a single operand
        }

        void operator()(ASTInteger& integer){
            program.addInstruction(
                program.factory().createMove(
                    createImmediateOperand(integer.value),
                    operand
                )
            ); 
        }

        void operator()(ASTVariable& variable){
            if(variable.Content->var->type() == Type::INT){
                program.addInstruction(program.factory().createMove(variable.Content->var->toIntegerOperand(), operand));
            } else {
                assert(false); //Cannot assign a string to a single operand
            }
        }

        void operator()(ASTComposedValue& value){
            assert(GetTypeVisitor()(value) == Type::INT); //Cannot be used for string operations

            program.addInstruction(program.factory().createMove(performIntOperation(value, program), operand));
        } 
};

class AssignValueToVariable : public boost::static_visitor<> {
    private:
        std::shared_ptr<Variable> variable;
        IntermediateProgram& program;
    
    public:
        AssignValueToVariable(std::shared_ptr<Variable> v, IntermediateProgram& p) : variable(v), program(p) {}

        void operator()(ASTLitteral& litteral){
            auto operands = variable->toStringOperand();

            program.addInstruction(
                program.factory().createMove(
                    createImmediateOperand(litteral.label),
                    operands.first
                )
            );
            
            program.addInstruction(
                program.factory().createMove(
                    createImmediateOperand(litteral.value.size() - 2),
                    operands.second
                )
            );
        }

        void operator()(ASTInteger& integer){
            program.addInstruction(
                program.factory().createMove(
                    createImmediateOperand(integer.value),
                    variable->toIntegerOperand()
                )
            ); 
        }

        void operator()(ASTVariable& variableSource){
            auto var = variableSource.Content->var;

            if(var->type() == Type::INT){
                program.addInstruction(program.factory().createMove(var->toIntegerOperand(), variable->toIntegerOperand()));
            } else {
                auto source = var->toStringOperand();
                auto destination = variable->toStringOperand();

                program.addInstruction(program.factory().createMove(source.first, destination.first));
                program.addInstruction(program.factory().createMove(source.second, destination.second));
            }
        }

        void operator()(ASTComposedValue& value){
            Type type = GetTypeVisitor()(value);

            if(type == Type::INT){
                program.addInstruction(program.factory().createMove(performIntOperation(value, program), variable->toIntegerOperand()));
            } else if(type == Type::STRING){
                auto source = performStringOperation(value, program);
                auto destination = variable->toStringOperand();

                program.addInstruction(program.factory().createMove(source.first, destination.first));
                program.addInstruction(program.factory().createMove(source.second, destination.second));
            }
        } 
};

inline JumpCondition toJumpNotCondition(std::string op){
    if(op == "!="){
        return JumpCondition::EQUALS;
    } else if(op == "=="){
        return JumpCondition::NOT_EQUALS;
    } else if(op == ">="){
        return JumpCondition::LESS;
    } else if(op == ">"){
        return JumpCondition::LESS_EQUALS;
    } else if(op == "<="){
        return JumpCondition::GREATER;
    } else if(op == "<"){
        return JumpCondition::GREATER_EQUALS;
    }

    assert(false); //Not handled
}

inline void writeILJumpIfNot(IntermediateProgram& program, ASTCondition& condition, const std::string& label, int labelIndex) {
    //No need to jump for a true boolean value 
    if(boost::get<ASTFalse>(&condition)){
        program.addInstruction(program.factory().createJump(JumpCondition::ALWAYS, eddic::label(label, labelIndex)));
    } else if(auto* ptr = boost::get<ASTBinaryCondition>(&condition)){
        ASTBinaryCondition& binaryCondition = *ptr;
        
        AssignValueToOperand leftVisitor(program.registers(EAX), program);

        boost::apply_visitor(leftVisitor, binaryCondition.Content->lhs);
        
        AssignValueToOperand rightVisitor(program.registers(EBX), program);
        boost::apply_visitor(rightVisitor, binaryCondition.Content->rhs);

        program.addInstruction(program.factory().createCompare(program.registers(EBX), program.registers(EAX)));

        program.addInstruction(program.factory().createJump(toJumpNotCondition(binaryCondition.Content->op), eddic::label(label, labelIndex)));
    }
}

inline std::pair<std::shared_ptr<Operand>, std::shared_ptr<Operand>> performStringOperation(ASTComposedValue& value, IntermediateProgram& program){
    assert(value.Content->operations.size() > 0); //Other values must be transformed before that phase

    auto registerA = program.registers(EAX);
    auto registerB = program.registers(EDX);

    PushValue pusher(program); 
    boost::apply_visitor(pusher, value.Content->first);

    unsigned int iter = 0;

    //Perfom all the additions
    for(auto& operation : value.Content->operations){
        boost::apply_visitor(pusher, operation.get<1>());

        program.addInstruction(program.factory().createCall("concat"));
        
        program.addInstruction(program.factory().createMath(Operation::ADD, createImmediateOperand(16), program.registers(ESP)));

        ++iter;

        //If there is more operation, push the answer
        if(iter < value.Content->operations.size()){
           program.addInstruction(program.factory().createPush(registerA)); 
           program.addInstruction(program.factory().createPush(registerB)); 
        }
    }
    
    return make_pair(registerA, registerB); 
}

inline void putInRegister(ASTValue& value, std::shared_ptr<Operand> operand, IntermediateProgram& program){
    if(isImmediate(value)){
        AssignValueToOperand visitor(operand, program);
        boost::apply_visitor(visitor, value);
    } else {
        PushValue visitor(program);
        boost::apply_visitor(visitor, value);

        program.addInstruction(program.factory().createMove(createStackOperand(0), operand));

        program.addInstruction(program.factory().createMath(Operation::ADD, createImmediateOperand(4), program.registers(ESP)));
    }
}

#include <iostream>

class CompilerVisitor : public boost::static_visitor<> {
    private:
        StringPool& pool;
        IntermediateProgram& program;
    
    public:
        CompilerVisitor(StringPool& p, IntermediateProgram& intermediateProgram) : pool(p), program(intermediateProgram) {}
        
        void operator()(ASTProgram& p){
            MainDeclaration().writeIL(program);

            visit_each(*this, p.Content->blocks);

            Methods().writeIL(program);

            pool.writeIL(program);

            p.Content->context->writeIL(program);
        }

        void operator()(ASTFunctionDeclaration& function){
            program.addInstruction(program.factory().createFunctionDeclaration(function.Content->mangledName, function.Content->context->size()));

            visit_each(*this, function.Content->instructions);

            program.addInstruction(program.factory().createFunctionExit(function.Content->context->size()));
        }

        void operator()(GlobalVariableDeclaration&){
            //Nothing to compile, the global variable values are written using global contexts
        }

        void operator()(ASTIf& if_){
            //TODO Make something accessible for others operations
            static int labels = 0;

            if (if_.Content->elseIfs.empty()) {
                int a = labels++;

                writeILJumpIfNot(program, if_.Content->condition, "L", a);

                visit_each(*this, if_.Content->instructions);

                if (if_.Content->else_) {
                    int b = labels++;

                    program.addInstruction(program.factory().createJump(JumpCondition::ALWAYS, eddic::label("L", b)));

                    program.addInstruction(program.factory().createLabel(eddic::label("L", a)));

                    visit_each(*this, (*if_.Content->else_).instructions);

                    program.addInstruction(program.factory().createLabel(eddic::label("L", b)));
                } else {
                    program.addInstruction(program.factory().createLabel(eddic::label("L", a)));
                }
            } else {
                int end = labels++;
                int next = labels++;

                writeILJumpIfNot(program, if_.Content->condition, "L", next);

                visit_each(*this, if_.Content->instructions);

                program.addInstruction(program.factory().createJump(JumpCondition::ALWAYS, eddic::label("L", end)));

                for (std::vector<ASTElseIf>::size_type i = 0; i < if_.Content->elseIfs.size(); ++i) {
                    ASTElseIf& elseIf = if_.Content->elseIfs[i];

                    program.addInstruction(program.factory().createLabel(eddic::label("L", next)));

                    //Last elseif
                    if (i == if_.Content->elseIfs.size() - 1) {
                        if (if_.Content->else_) {
                            next = labels++;
                        } else {
                            next = end;
                        }
                    } else {
                        next = labels++;
                    }

                    writeILJumpIfNot(program, elseIf.condition, "L", next);

                    visit_each(*this, elseIf.instructions);

                    program.addInstruction(program.factory().createJump(JumpCondition::ALWAYS, eddic::label("L", end)));
                }

                if (if_.Content->else_) {
                    program.addInstruction(program.factory().createLabel(eddic::label("L", next)));

                    visit_each(*this, (*if_.Content->else_).instructions);
                }

                program.addInstruction(program.factory().createLabel(eddic::label("L", end)));
            }
        }

        void operator()(ASTAssignment& assignment){
            AssignValueToVariable visitor(assignment.Content->context->getVariable(assignment.Content->variableName), program);
            boost::apply_visitor(visitor, assignment.Content->value);
        }

        void operator()(ASTDeclaration& declaration){
            AssignValueToVariable visitor(declaration.Content->context->getVariable(declaration.Content->variableName), program);
            boost::apply_visitor(visitor, declaration.Content->value);
        }

        void operator()(ASTSwap& swap){
            auto lhs_var = swap.Content->lhs_var;
            auto rhs_var = swap.Content->rhs_var;

            //We have the guarantee here that both variables are of the same type
            switch (lhs_var->type()) {
                case Type::INT:{
                    auto registerA = program.registers(EAX);
             
                    auto left = lhs_var->toIntegerOperand();
                    auto right = rhs_var->toIntegerOperand();

                    program.addInstruction(program.factory().createMove(left, registerA));
                    program.addInstruction(program.factory().createMove(right, left));
                    program.addInstruction(program.factory().createMove(registerA, right));

                    break;
                }
                case Type::STRING:{
                    auto registerA = program.registers(EAX);
                   
                    auto left = lhs_var->toStringOperand();
                    auto right = rhs_var->toStringOperand();
                    
                    program.addInstruction(program.factory().createMove(left.first, registerA));
                    program.addInstruction(program.factory().createMove(right.first, left.first));
                    program.addInstruction(program.factory().createMove(registerA, right.first));
                    
                    program.addInstruction(program.factory().createMove(left.second, registerA));
                    program.addInstruction(program.factory().createMove(right.second, left.second));
                    program.addInstruction(program.factory().createMove(registerA, right.second));
                    
                    break;
                }
                default:
                   throw SemanticalException("Variable of invalid type");
            }
        }

        void operator()(ASTWhile& while_){
            //TODO Make something accessible for others operations
            static int labels = 0;

            int startLabel = labels++;
            int endLabel = labels++;

            program.addInstruction(program.factory().createLabel(label("WL", startLabel)));

            writeILJumpIfNot(program, while_.Content->condition, "WL", endLabel);

            visit_each(*this, while_.Content->instructions);

            program.addInstruction(program.factory().createJump(JumpCondition::ALWAYS, label("WL", startLabel)));

            program.addInstruction(program.factory().createLabel(label("WL", endLabel)));
        }

        void operator()(ASTFor for_){
            visit_optional(*this, for_.Content->start);

            static int labels = -1;

            ++labels;

            program.addInstruction(program.factory().createLabel(label("start_for", labels)));

            if(for_.Content->condition){
                writeILJumpIfNot(program, *for_.Content->condition, "end_for", labels);
            }

            visit_each(*this, for_.Content->instructions);

            visit_optional(*this, for_.Content->repeat);

            program.addInstruction(program.factory().createJump(JumpCondition::ALWAYS, label("start_for", labels)));

            program.addInstruction(program.factory().createLabel(label("end_for", labels)));
        }

        //TODO Rewrite that function, perhaps with a transformation into several element in a previous stage
        void operator()(ASTForeach& foreach){
            ASTInteger fromValue;
            fromValue.value = foreach.Content->from;
            
            ASTInteger toValue;
            toValue.value = foreach.Content->to;

            AssignValueToVariable visitor(foreach.Content->context->getVariable(foreach.Content->variableName), program);
            
            //Assign the base value to the variable
            visit_non_variant(visitor, fromValue);
            
            static int labels = -1;

            ++labels;

            program.addInstruction(program.factory().createLabel(label("start_foreach", labels)));

            //Create a condition
            ASTVariable v;
            v.Content->variableName = foreach.Content->variableName;
            v.Content->context = foreach.Content->context;
            v.Content->var = v.Content->context->getVariable(foreach.Content->variableName);
        
            //Avoid doing all that conversion stuff...  
            ASTCondition condition; 
            ASTBinaryCondition binaryCondition; 
            binaryCondition.Content->lhs = v;
            binaryCondition.Content->rhs = toValue;
            binaryCondition.Content->op = "<=";

            condition = binaryCondition;

            writeILJumpIfNot(program, condition, "end_foreach", labels);

            //Write all the instructions
            visit_each(*this, foreach.Content->instructions);

            //Increment the variable
            ASTInteger inc;
            inc.value = 1;
           
            ASTComposedValue addition;
            addition.Content->first = v;
            addition.Content->operations.push_back(boost::tuples::tuple<char, ASTValue>('+', inc));
           
            visit_non_variant(visitor, addition);
            
            program.addInstruction(program.factory().createJump(JumpCondition::ALWAYS, label("start_foreach", labels)));

            program.addInstruction(program.factory().createLabel(label("end_foreach", labels)));
        }

        void operator()(ASTFunctionCall& functionCall){
            PushValue visitor(program);
            for(auto& value : functionCall.Content->values){
                boost::apply_visitor(visitor, value);
            }

            if(functionCall.Content->functionName == "print" || functionCall.Content->functionName == "println"){
                Type type = boost::apply_visitor(GetTypeVisitor(), functionCall.Content->values[0]);

                switch (type) {
                    case Type::INT:
                        program.addInstruction(program.factory().createCall("print_integer"));
                        program.addInstruction(program.factory().createMath(Operation::ADD, createImmediateOperand(4), program.registers(ESP)));

                        break;
                    case Type::STRING:
                        program.addInstruction(program.factory().createCall("print_string"));
                        program.addInstruction(program.factory().createMath(Operation::ADD, createImmediateOperand(8), program.registers(ESP)));

                        break;
                    default:
                        throw SemanticalException("Variable of invalid type");
                }
    
                if(functionCall.Content->functionName == "println"){
                    program.addInstruction(program.factory().createCall("print_line"));
                }
            } else {
                std::string mangled = mangle(functionCall.Content->functionName, functionCall.Content->values);

                program.addInstruction(program.factory().createCall(mangled));

                int total = 0;

                for(auto& value : functionCall.Content->values){
                    Type type = boost::apply_visitor(GetTypeVisitor(), value);   

                    total += size(type);
                }

                program.addInstruction(program.factory().createMath(Operation::ADD, createImmediateOperand(total), program.registers(ESP)));
            }
        }
};

void IntermediateCompiler::compile(ASTProgram& program, StringPool& pool, IntermediateProgram& intermediateProgram){
    CompilerVisitor visitor(pool, intermediateProgram);
    visitor(program);
}