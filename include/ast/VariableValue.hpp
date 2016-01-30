//=======================================================================
// Copyright Baptiste Wicht 2011-2016.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef AST_VARIABLE_VALUE_H
#define AST_VARIABLE_VALUE_H

#include <memory>
#include <string>

#include "ast/Deferred.hpp"
#include "ast/Position.hpp"

namespace eddic {

class Context;
class Variable;

namespace ast {

/*!
 * \class ASTVariableValue
 * \brief The AST node for a variable value.  
 * Should only be used from the Deferred version (eddic::ast::VariableValue).
 */
struct ASTVariableValue {
    std::shared_ptr<Context> context;
    std::shared_ptr<Variable> var;

    Position position;
    std::string variableName;

    mutable long references = 0;
};

/*!
 * \struct VariableValue
 * \brief The AST node for a variable value.
*/
struct VariableValue : public Deferred<ASTVariableValue> {
    std::shared_ptr<Variable> variable() const {
        return Content->var;
    }
    
    std::shared_ptr<Context> context() const {
        return Content->context;
    }
};

} //end of ast

} //end of eddic

//Adapt the struct for the AST
BOOST_FUSION_ADAPT_STRUCT(
    eddic::ast::VariableValue, 
    (eddic::ast::Position, Content->position)
    (std::string, Content->variableName)
)

#endif
