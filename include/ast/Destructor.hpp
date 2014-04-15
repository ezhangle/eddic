//=======================================================================
// Copyright Baptiste Wicht 2011-2014.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef AST_DESTRUCTOR_H
#define AST_DESTRUCTOR_H

#include <string>
#include <memory>
#include <vector>

#include <boost/fusion/include/adapt_struct.hpp>

#include "ast/Instruction.hpp"
#include "ast/Position.hpp"
#include "ast/FunctionParameter.hpp"

namespace eddic {

class FunctionContext;

namespace ast {

/*!
 * \class ASTDestructor
 * \brief The AST node for a destructor declaration.  
 * Should only be used from the Deferred version (eddic::ast::Destructor).
 */
struct ASTDestructor { 
    std::shared_ptr<FunctionContext> context;
    
    std::string mangledName;
    std::shared_ptr<const eddic::Type> struct_type = nullptr;

    std::vector<FunctionParameter> parameters;

    Position position;
    std::vector<Instruction> instructions;

    mutable long references = 0;
};

/*!
 * \typedef FunctionDeclaration
 * \brief The AST node for a function declaration.
 */
typedef Deferred<ASTDestructor> Destructor; 

} //end of ast

} //end of eddic

//Adapt the struct for the AST
BOOST_FUSION_ADAPT_STRUCT(
    eddic::ast::Destructor, 
    (eddic::ast::Position, Content->position)
    (std::vector<eddic::ast::Instruction>, Content->instructions)
)

#endif
