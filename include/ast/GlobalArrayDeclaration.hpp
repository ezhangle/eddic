//=======================================================================
// Copyright Baptiste Wicht 2011-2014.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef AST_GLOBAL_ARRAY_DECLARATION_H
#define AST_GLOBAL_ARRAY_DECLARATION_H

#include <memory>

#include <boost/fusion/include/adapt_struct.hpp>

#include "ast/Deferred.hpp"
#include "ast/Position.hpp"
#include "ast/VariableType.hpp"
#include "ast/Value.hpp"

namespace eddic {

class Context;

namespace ast {

/*!
 * \class ASTGlobalArrayDeclaration
 * \brief The AST node for a global array declaration.   
 * Should only be used from the Deferred version (eddic::ast::GlobalarrayDeclaration).
 */
struct ASTGlobalArrayDeclaration {
    std::shared_ptr<Context> context;
    
    Position position;
    Type arrayType;
    std::string arrayName;
    Value size;

    mutable long references = 0;
};

/*!
 * \typedef GlobalArrayDeclaration
 * \brief The AST node for a global array declaration.
 */
typedef Deferred<ASTGlobalArrayDeclaration> GlobalArrayDeclaration;

} //end of ast

} //end of eddic

//Adapt the struct for the AST
BOOST_FUSION_ADAPT_STRUCT(
    eddic::ast::GlobalArrayDeclaration, 
    (eddic::ast::Position, Content->position)
    (eddic::ast::Type, Content->arrayType)
    (std::string, Content->arrayName)
    (eddic::ast::Value, Content->size)
)

#endif
