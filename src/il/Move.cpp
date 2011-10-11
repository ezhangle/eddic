//=======================================================================
// Copyright Baptiste Wicht 2011.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include "il/Move.hpp"

using namespace eddic;

Move::Move(std::shared_ptr<Operand> lhs, std::shared_ptr<Operand> rhs) : m_lhs(lhs), m_rhs(rhs) {}

void Move::write(AssemblyFileWriter& writer){
   //TODO 
}
