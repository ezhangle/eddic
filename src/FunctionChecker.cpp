//=======================================================================
// Copyright Baptiste Wicht 2011.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include "FunctionChecker.hpp"

#include "ast/Program.hpp"
#include "FunctionTable.hpp"

#include "SemanticalException.hpp"
#include "ASTVisitor.hpp"
#include "VisitorUtils.hpp"

#include "mangling.hpp"

using namespace eddic;

class FunctionInserterVisitor : public boost::static_visitor<> {
    private:
        FunctionTable& functionTable;

    public:
        FunctionInserterVisitor(FunctionTable& table) : functionTable(table) {}

        AUTO_RECURSE_PROGRAM()
         
        void operator()(ASTFunctionDeclaration& declaration){
            auto signature = std::make_shared<FunctionSignature>();

            signature->name = declaration.Content->functionName;

            for(auto& param : declaration.Content->parameters){
                auto parameter = std::make_shared<ParameterType>();
                parameter->name = param.parameterName;
                parameter->paramType = stringToType(param.parameterType);

                signature->parameters.push_back(parameter);
            }
            
            declaration.Content->mangledName = signature->mangledName = mangle(declaration.Content->functionName, signature->parameters);

            //TODO Verifiy that the function has not been previously defined

            functionTable.addFunction(signature);

            //Stop recursion here
        }

        void operator()(GlobalVariableDeclaration&){
            //Stop recursion here
        }
};

class FunctionCheckerVisitor : public boost::static_visitor<> {
    private:
        FunctionTable& functionTable;

    public:
        FunctionCheckerVisitor(FunctionTable& table) : functionTable(table) {}

        AUTO_RECURSE_PROGRAM()
        AUTO_RECURSE_FUNCTION_DECLARATION() 
        AUTO_RECURSE_GLOBAL_DECLARATION() 
        AUTO_RECURSE_SIMPLE_LOOPS()
        AUTO_RECURSE_FOREACH()
        AUTO_RECURSE_BRANCHES()
        AUTO_RECURSE_BINARY_CONDITION()
        AUTO_RECURSE_COMPOSED_VALUES()
        AUTO_RECURSE_VARIABLE_OPERATIONS()

        void operator()(ASTFunctionCall& functionCall){
            std::string name = functionCall.Content->functionName;
            
            if(name == "println" || name == "print"){
                return;
            }

            std::string mangled = mangle(name, functionCall.Content->values);

            if(!functionTable.exists(mangled)){
                throw SemanticalException("The function \"" + functionCall.Content->functionName + "()\" does not exists");
            }
        }
        
        void operator()(ASTVariable&){
            //No function calls there
        }

        void operator()(ASTSwap&){
            //No function calls there
        }
        
        void operator()(TerminalNode&){
            //No function calls there
        }
};

void FunctionChecker::check(ASTProgram& program, FunctionTable& functionTable){
    //First phase : Collect functions
    FunctionInserterVisitor inserterVisitor(functionTable);
    inserterVisitor(program);

    //Second phase : Verify calls
    FunctionCheckerVisitor checkerVisitor(functionTable);
    checkerVisitor(program);
}