//=======================================================================
// Copyright Baptiste Wicht 2011-2012.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include <unordered_set>

#include "FunctionContext.hpp"
#include "Function.hpp"
#include "likely.hpp"

#include "mtac/merge_basic_blocks.hpp"
#include "mtac/Function.hpp"
#include "mtac/Utils.hpp"
#include "mtac/Statement.hpp"

using namespace eddic;

bool mtac::merge_basic_blocks::operator()(mtac::Function& function){
    bool optimized = false;

    std::unordered_set<mtac::basic_block_p> usage;

    computeBlockUsage(function, usage);

    auto it = iterate(function);

    //The ENTRY Basic block should not be merged
    ++it;

    while(it.has_next()){
        auto& block = *it;
        
        if(block->index == -1){
            ++it;
            continue;
        } else if(block->index == -2){
            break;
        }
                
        auto next = block->next;

        if(unlikely(block->statements.empty())){
            if(usage.find(block) == usage.end()){
                it.erase();
                optimized = true;

                --it;
                continue;
            } else {
                if(next && next->index != -2 && usage.find(next) == usage.end()){
                    it.merge_in(next);
                    optimized = true;

                    --it;
                    continue;
                }
            }
        } else {
            auto& last = block->statements.back();

            bool merge = false;

            if(auto* ptr = boost::get<std::shared_ptr<mtac::Quadruple>>(&last)){
                auto& quadruple = *ptr;

                if(quadruple->op == mtac::Operator::GOTO){
                    merge = quadruple->block == next;

                    if(merge){
                        block->statements.pop_back();
                        computeBlockUsage(function, usage);
                    }
                } else if(quadruple->op == mtac::Operator::CALL){
                    merge = safe(quadruple->function().mangled_name()); 
                } else {
                    merge = true;
                }
            } 

            if(merge && next && next->index != -2){
                //Only if the next block is not used because we will remove its label
                if(usage.find(next) == usage.end()){
                    if(!next->statements.empty()){
                        if(auto* ptr = boost::get<std::shared_ptr<mtac::Quadruple>>(&(next->statements.front()))){
                            if((*ptr)->op == mtac::Operator::CALL && !safe((*ptr)->function().mangled_name())){
                                ++it;
                                continue;
                            }
                        }
                    }

                    it.merge_in(next);

                    optimized = true;

                    --it;
                    continue;
                }
            }
        }

        ++it;
    }

    if(optimized){
        (*this)(function);
    }
   
    return optimized; 
}