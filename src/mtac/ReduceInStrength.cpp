//=======================================================================
// Copyright Baptiste Wicht 2011-2013.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include "Utils.hpp"

#include "mtac/ReduceInStrength.hpp"
#include "mtac/OptimizerUtils.hpp"
#include "mtac/Quadruple.hpp"

using namespace eddic;

void mtac::ReduceInStrength::operator()(mtac::Quadruple& quadruple){
    switch(quadruple.op){
        case mtac::Operator::MOD:
            if(auto* ptr = boost::get<int>(&*quadruple.arg2)){
                auto constant = *ptr;

                if(isPowerOfTwo(constant)){
                    replaceRight(*this, quadruple, *quadruple.arg1, mtac::Operator::AND, constant - 1);
                }
            }

            break;
        case mtac::Operator::MUL:
            if(*quadruple.arg1 == 2){
                replaceRight(*this, quadruple, *quadruple.arg2, mtac::Operator::ADD, *quadruple.arg2);
            } else if(*quadruple.arg2 == 2){
                replaceRight(*this, quadruple, *quadruple.arg1, mtac::Operator::ADD, *quadruple.arg1);
            }

            break;
        case mtac::Operator::FMUL:
            if(*quadruple.arg1 == 2.0){
                replaceRight(*this, quadruple, *quadruple.arg2, mtac::Operator::FADD, *quadruple.arg2);
            } else if(*quadruple.arg2 == 2.0){
                replaceRight(*this, quadruple, *quadruple.arg1, mtac::Operator::FADD, *quadruple.arg1);
            }

            break;
        default:
            break;
    }
}
