//=======================================================================
// Copyright Baptiste Wicht 2011-2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef AST_OPERATOR_H
#define AST_OPERATOR_H

#include <string>

namespace eddic {

namespace ast {

/*! 
 * \enum Operator
 * \brief Define an operator in the AST for generic node. 
 */
enum class Operator : unsigned int {
    ASSIGN,

    ADD,
    SUB,
    DIV,
    MUL,
    MOD,

    AND,
    OR,
    NOT,

    DEC,
    INC,

    EQUALS,
    NOT_EQUALS,
    LESS,
    LESS_EQUALS,
    GREATER,
    GREATER_EQUALS,

    //Prefix operators

    STAR,               /**< Star operator, to dereference pointers. */ 
    ADDRESS,            /**< Address operator, get address of variable. */

    //Postfix operators

    BRACKET,            /**< Bracket operator, to access array index. */ 
    DOT,                /**< Dot operator, to access to members */ 
    CALL                /**< Call operator, to call member functions */ 
};

std::string toString(Operator op);
std::ostream& operator<< (std::ostream& stream, Operator);

} //end of ast

} //end of eddic

#endif
