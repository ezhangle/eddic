//=======================================================================
// Copyright Baptiste Wicht 2011-2013.
// Distributed under the Boost Software License: Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include "ast/PointerType.hpp"

using namespace eddic;

bool ast::operator==(const ast::PointerType& a, const ast::PointerType& b){
    return a.type.get() == b.type.get();
}

std::ostream& ast::operator<<(std::ostream& out, const ast::PointerType& type){
    return out << "Pointer Type " << type.type.get();
}
