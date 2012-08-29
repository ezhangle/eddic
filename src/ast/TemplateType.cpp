//=======================================================================
// Copyright Baptiste Wicht 2011.
// Distributed under the Boost Software License: Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include "ast/TemplateType.hpp"

#include "Utils.hpp"

using namespace eddic;

bool ast::operator==(const ast::TemplateType& a, const ast::TemplateType& b){
    return a.type == b.type && are_equals(a.template_types, b.template_types);
}
