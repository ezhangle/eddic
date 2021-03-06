//=======================================================================
// Copyright Baptiste Wicht 2011-2016.
// Distributed under the Boost Software License: Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#include "ast/Null.hpp"

using namespace eddic;

std::ostream& ast::operator<< (std::ostream& stream, ast::Null){
    return stream << "null";
}
