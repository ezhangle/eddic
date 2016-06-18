//=======================================================================
// Copyright Baptiste Wicht 2011-2016.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef AST_TEMPLATE_FUNCTION_DECLARATION_H
#define AST_TEMPLATE_FUNCTION_DECLARATION_H

#include <boost/fusion/include/adapt_struct.hpp>

#include "ast/FunctionParameter.hpp"
#include "ast/Instruction.hpp"
#include "ast/Position.hpp"
#include "ast/VariableType.hpp"

namespace eddic {

class FunctionContext;

namespace ast {

/*!
 * \class TemplateFunctionDeclaration
 * \brief The AST node for a template function declaration.
 */
struct TemplateFunctionDeclaration {
    std::shared_ptr<FunctionContext> context;

    std::string mangledName;
    std::string struct_name;

    Position position;
    std::vector<std::string> template_types;
    Type returnType;
    std::string functionName;
    std::vector<FunctionParameter> parameters;
    std::vector<Instruction> instructions;

    mutable long references = 0;
};

} //end of ast

} //end of eddic

//Adapt the struct for the AST
BOOST_FUSION_ADAPT_STRUCT(
    eddic::ast::TemplateFunctionDeclaration,
    (eddic::ast::Position, position)
    (std::vector<std::string>, template_types)
    (eddic::ast::Type, returnType)
    (std::string, functionName)
    (std::vector<eddic::ast::FunctionParameter>, parameters)
    (std::vector<eddic::ast::Instruction>, instructions)
)

#endif
