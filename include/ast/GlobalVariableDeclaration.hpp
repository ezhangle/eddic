//=======================================================================
// Copyright Baptiste Wicht 2011-2014.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef AST_GLOBAL_VARIABLE_DECLARATION_H
#define AST_GLOBAL_VARIABLE_DECLARATION_H

#include <memory>

#include <boost/fusion/include/adapt_struct.hpp>

#include "ast/Value.hpp"
#include "ast/VariableType.hpp"

namespace eddic {

class Context;

namespace ast {

/*!
 * \class ASTGlobalVariableDeclaration
 * \brief The AST node for a declaration of a global variable. 
 * Should only be used from the Deferred version (eddic::ast::GlobalVariableDeclaration).
 */
struct ASTGlobalVariableDeclaration {
    std::shared_ptr<Context> context;

    Position position;
    Type variableType;
    std::string variableName;
    boost::optional<Value> value;

    mutable long references = 0;
};

/*!
 * \typedef GlobalVariableDeclaration
 * \brief The AST node for a declaration of a global variable. 
 */
typedef Deferred<ASTGlobalVariableDeclaration> GlobalVariableDeclaration;

} //end of ast

} //end of eddic

//Adapt the struct for the AST
BOOST_FUSION_ADAPT_STRUCT(
    eddic::ast::GlobalVariableDeclaration, 
    (eddic::ast::Position, Content->position)
    (eddic::ast::Type, Content->variableType)
    (std::string, Content->variableName)
    (boost::optional<eddic::ast::Value>, Content->value)
)

#endif
