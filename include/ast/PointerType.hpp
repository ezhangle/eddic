//=======================================================================
// Copyright Baptiste Wicht 2011-2014.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef AST_POINTER_TYPE_H
#define AST_POINTER_TYPE_H

#include <string>
#include <ostream>

#include <boost/fusion/include/adapt_struct.hpp>

#include "ast/VariableType.hpp"

namespace eddic {

namespace ast {

/*!
 * \struct PointerType
 * \brief A pointer type in the AST.  
 */
struct PointerType {
    boost::recursive_wrapper<ast::Type> type;
};

bool operator==(const PointerType& a, const PointerType& b);

std::ostream& operator<<(std::ostream& out, const ast::PointerType& type);

} //end of ast

} //end of eddic

//Adapt the struct for the AST
BOOST_FUSION_ADAPT_STRUCT(
    eddic::ast::PointerType, 
    (boost::recursive_wrapper<eddic::ast::Type>, type)
)

#endif
