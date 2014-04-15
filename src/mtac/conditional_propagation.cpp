//=======================================================================
// Copyright Baptiste Wicht 2011-2014.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#include "Variable.hpp"

#include "mtac/conditional_propagation.hpp"
#include "mtac/Function.hpp"
#include "mtac/Quadruple.hpp"
#include "mtac/Utils.hpp"
#include "mtac/variable_usage.hpp"

using namespace eddic;

namespace {

mtac::Quadruple& get_variable_declaration(mtac::basic_block_p basic_block, std::shared_ptr<Variable> variable){
    for(auto& quadruple : basic_block){
        if(quadruple.result == variable){
            return quadruple;
        }
    }

    return basic_block->statements.front();
}

template<bool If>
mtac::Operator to_binary_operator(mtac::Operator op){
    if(If){
        switch(op){ 
            /* relational operators */
            case mtac::Operator::EQUALS:
                return mtac::Operator::IF_EQUALS;
            case mtac::Operator::NOT_EQUALS:
                return mtac::Operator::IF_NOT_EQUALS;
            case mtac::Operator::GREATER:
                return mtac::Operator::IF_GREATER;
            case mtac::Operator::GREATER_EQUALS:
                return mtac::Operator::IF_GREATER_EQUALS;
            case mtac::Operator::LESS:
                return mtac::Operator::IF_LESS;
            case mtac::Operator::LESS_EQUALS:
                return mtac::Operator::IF_LESS_EQUALS;

            /* float relational operators */
            case mtac::Operator::FE:
                return mtac::Operator::IF_FE;
            case mtac::Operator::FNE:
                return mtac::Operator::IF_FNE;
            case mtac::Operator::FG:
                return mtac::Operator::IF_FG;
            case mtac::Operator::FGE:
                return mtac::Operator::IF_FGE;
            case mtac::Operator::FLE:
                return mtac::Operator::IF_FLE;
            case mtac::Operator::FL:
                return mtac::Operator::IF_FL;

            default:
                eddic_unreachable("Not a binary operator");
        }
    } else {
        switch(op){ 
            /* relational operators */
            case mtac::Operator::EQUALS:
                return mtac::Operator::IF_FALSE_EQUALS;
            case mtac::Operator::NOT_EQUALS:
                return mtac::Operator::IF_FALSE_NOT_EQUALS;
            case mtac::Operator::GREATER:
                return mtac::Operator::IF_FALSE_GREATER;
            case mtac::Operator::GREATER_EQUALS:
                return mtac::Operator::IF_FALSE_GREATER_EQUALS;
            case mtac::Operator::LESS:
                return mtac::Operator::IF_FALSE_LESS;
            case mtac::Operator::LESS_EQUALS:
                return mtac::Operator::IF_FALSE_LESS_EQUALS;

            /* float relational operators */
            case mtac::Operator::FE:
                return mtac::Operator::IF_FALSE_FE;
            case mtac::Operator::FNE:
                return mtac::Operator::IF_FALSE_FNE;
            case mtac::Operator::FG:
                return mtac::Operator::IF_FALSE_FG;
            case mtac::Operator::FGE:
                return mtac::Operator::IF_FALSE_FGE;
            case mtac::Operator::FLE:
                return mtac::Operator::IF_FALSE_FLE;
            case mtac::Operator::FL:
                return mtac::Operator::IF_FALSE_FL;

            default:
                eddic_unreachable("Not a binary operator");
        }
    }
}

template<bool If, typename Branch>
bool optimize_branch(Branch& branch, mtac::basic_block_p basic_block, mtac::VariableUsage variable_usage){
    if(mtac::isVariable(*branch.arg1)){
        auto variable = boost::get<std::shared_ptr<Variable>>(*branch.arg1);

        if(variable_usage[variable] == 2){
            auto& declaration = get_variable_declaration(basic_block, variable);

            if(declaration.result == variable){
                if(
                        (declaration.op >= mtac::Operator::EQUALS && declaration.op <= mtac::Operator::LESS_EQUALS)
                        ||  (declaration.op >= mtac::Operator::FE && declaration.op <= mtac::Operator::FL)){
                    branch.arg1 = *declaration.arg1;
                    branch.arg2 = *declaration.arg2;
                    branch.op = to_binary_operator<If>(declaration.op);

                    return true;
                }
            }
        }
    }

    return false;
}

} //end of anonymous namespace

bool mtac::conditional_propagation::operator()(mtac::Function& function){
    bool optimized = false;

    auto variable_usage = mtac::compute_variable_usage(function);

    for(auto& basic_block : function){
        for(auto& quadruple : basic_block){
            if(quadruple.op == mtac::Operator::IF_FALSE_UNARY){
                optimized |= optimize_branch<false>(quadruple, basic_block, variable_usage);
            } else if(quadruple.op == mtac::Operator::IF_UNARY){
                optimized |= optimize_branch<true>(quadruple, basic_block, variable_usage);
            }
        }
    }

    if(optimized){
        auto usage = mtac::compute_read_usage(function);

        for(auto& block : function){
            for(auto& quadruple : block){
                if(mtac::erase_result(quadruple.op)){
                    if(usage.read[quadruple.result] == 0){
                        mtac::transform_to_nop(quadruple);
                    }
                }
            }
        }
    }

    return optimized;
}
