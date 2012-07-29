//=======================================================================
// Copyright Baptiste Wicht 2011.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include <algorithm>
#include <memory>

#include "variant.hpp"
#include "SemanticalException.hpp"
#include "Context.hpp"
#include "GlobalContext.hpp"
#include "FunctionContext.hpp"
#include "Variable.hpp"
#include "Warnings.hpp"
#include "Options.hpp"
#include "VisitorUtils.hpp"
#include "Utils.hpp"

#include "ast/WarningsEngine.hpp"
#include "ast/SourceFile.hpp"
#include "ast/ASTVisitor.hpp"
#include "ast/Position.hpp"
#include "ast/GetTypeVisitor.hpp"
#include "ast/TypeTransformer.hpp"

using namespace eddic;

namespace {

typedef std::unordered_map<std::shared_ptr<Variable>, ast::Position> Positions;  

struct Collector : public boost::static_visitor<> {
    public:
        AUTO_RECURSE_PROGRAM()
        AUTO_RECURSE_STRUCT()
        AUTO_RECURSE_DESTRUCTOR()

        void operator()(ast::FunctionDeclaration& function){
            for(auto& param : function.Content->parameters){
                positions[function.Content->context->getVariable(param.parameterName)] = function.Content->position;
            }
            
            visit_each(*this, function.Content->instructions);
        }
        
        void operator()(ast::Constructor& function){
            for(auto& param : function.Content->parameters){
                positions[function.Content->context->getVariable(param.parameterName)] = function.Content->position;
            }
            
            visit_each(*this, function.Content->instructions);
        }
        
        void operator()(ast::GlobalVariableDeclaration& declaration){
            positions[declaration.Content->context->getVariable(declaration.Content->variableName)] = declaration.Content->position;
        }

        void operator()(ast::GlobalArrayDeclaration& declaration){
            positions[declaration.Content->context->getVariable(declaration.Content->arrayName)] = declaration.Content->position;
        }
        
        void operator()(ast::ArrayDeclaration& declaration){
            positions[declaration.Content->context->getVariable(declaration.Content->arrayName)] = declaration.Content->position;
        }
        
        void operator()(ast::VariableDeclaration& declaration){
            positions[declaration.Content->context->getVariable(declaration.Content->variableName)] = declaration.Content->position;
        }

        AUTO_IGNORE_OTHERS()

        const ast::Position& getPosition(std::shared_ptr<Variable> var){
            assert(positions.find(var) != positions.end());

            return positions[var];   
        }

    private:
        Positions positions;  
};

struct Inspector : public boost::static_visitor<> {
    public:
        Inspector(Collector& collector) : collector(collector) {}
    
        /* The following constructions can contains instructions with warnings  */
        AUTO_RECURSE_GLOBAL_DECLARATION() 
        AUTO_RECURSE_FUNCTION_CALLS()
        AUTO_RECURSE_MEMBER_FUNCTION_CALLS()
        AUTO_RECURSE_BUILTIN_OPERATORS()
        AUTO_RECURSE_SIMPLE_LOOPS()
        AUTO_RECURSE_FOREACH()
        AUTO_RECURSE_BRANCHES()
        AUTO_RECURSE_BINARY_CONDITION()
        AUTO_RECURSE_COMPOSED_VALUES()
        AUTO_RECURSE_RETURN_VALUES()
        AUTO_RECURSE_ARRAY_VALUES()
        AUTO_RECURSE_VARIABLE_OPERATIONS()
        AUTO_RECURSE_TERNARY()
        AUTO_RECURSE_SWITCH()
        AUTO_RECURSE_SWITCH_CASE()
        AUTO_RECURSE_DEFAULT_CASE()

        void check(std::shared_ptr<Context> context){
            if(option_defined("warning-unused")){
                auto iter = context->begin();
                auto end = context->end();

                for(; iter != end; iter++){
                    auto var = iter->second;

                    if(var->referenceCount() == 0){
                        if(var->position().isStack()){
                            warn(collector.getPosition(var), "unused variable '" + var->name() + "'");
                        } else if(var->position().isGlobal()){
                            warn(collector.getPosition(var), "unused global variable '" + var->name() + "'");
                        } else if(var->position().isParameter()){
                            warn(collector.getPosition(var), "unused parameter '" + var->name() + "'");
                        }
                    }
                }
            }
        }

        void operator()(ast::SourceFile& program){
            check(program.Content->context);

            visit_each(*this, program.Content->blocks);
        }
        
        void operator()(ast::Struct& declaration){
            if(option_defined("warning-unused")){
                auto struct_ = symbols.get_struct(declaration.Content->name);

                if(struct_->get_references() == 0){
                    warn(declaration.Content->position, "unused structure '" + declaration.Content->name + "'");
                } else {
                    for(auto member : struct_->members){
                        if(member->get_references() == 0){
                            warn(declaration.Content->position, "unused member '" + declaration.Content->name + ".'" + member->name);
                        }
                    }
                }
            }
        }
        
        void operator()(ast::FunctionDeclaration& declaration){
            check(declaration.Content->context);
            
            if(option_defined("warning-unused")){
                int references = symbols.referenceCount(declaration.Content->mangledName);

                if(references == 0){
                    warn(declaration.Content->position, "unused function '" + declaration.Content->functionName + "'");
                }
            }
        
            visit_each(*this, declaration.Content->instructions);
        }
    
        void operator()(ast::Cast& cast){
            if(option_defined("warning-cast")){
                auto src_type = visit(ast::GetTypeVisitor(), cast.Content->value);
                auto dest_type = visit(ast::TypeTransformer(), cast.Content->type);

                if(src_type == dest_type){
                    warn(cast.Content->position, "useless cast");
                }
            }
        }
        
        //No warnings for other types
        AUTO_IGNORE_OTHERS()
    
    private:
        Collector& collector;
};

} //end of anonymous namespace

void ast::checkForWarnings(ast::SourceFile& program){
    Collector collector;
    visit_non_variant(collector, program);

    Inspector inspector(collector);
    visit_non_variant(inspector, program);
}
