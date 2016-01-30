//=======================================================================
// Copyright Baptiste Wicht 2011-2016.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef AST_TEMPLATE_TYPE_H
#define AST_TEMPLATE_TYPE_H

#include <vector>
#include <string>
#include <ostream>

#include "variant.hpp"

#include "ast/VariableType.hpp"

#include <boost/fusion/include/adapt_struct.hpp>

namespace eddic {

namespace ast {

/*!
 * \struct TemplateType
 * \brief A template type in the AST.  
 */
struct TemplateType {
    std::string type;
    std::vector<Type> template_types;
};

bool operator==(const TemplateType& a, const TemplateType& b);

std::ostream& operator<<(std::ostream& out, const ast::TemplateType& type);

} //end of ast

} //end of eddic

//Adapt the struct for the AST
BOOST_FUSION_ADAPT_STRUCT(
    eddic::ast::TemplateType, 
    (std::string, type)
    (std::vector<eddic::ast::Type>, template_types)
)

#endif
