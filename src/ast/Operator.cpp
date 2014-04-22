//=======================================================================
// Copyright Baptiste Wicht 2011-2014.
// Distributed under the Boost Software License: Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#include "assert.hpp"

#include "ast/Operator.hpp"

using namespace eddic;

std::ostream& ast::operator<< (std::ostream& stream, ast::Operator op){
    return stream << ast::toString(op);
}

std::string eddic::ast::toString(Operator op){
    switch(op){
        case ast::Operator::ADD:
            return "+";
        case ast::Operator::ASSIGN:
            return "=";
        case ast::Operator::SWAP:
            return "<=>";
        case ast::Operator::SUB:
            return "-";
        case ast::Operator::DIV:
            return "/";
        case ast::Operator::MUL:
            return "*";
        case ast::Operator::MOD:
            return "%";
        case ast::Operator::AND:
            return "&&";
        case ast::Operator::OR:
            return "||";
        case ast::Operator::NOT:
            return "!";
        case ast::Operator::DEC:
            return "--";
        case ast::Operator::INC:
            return "++";
        case ast::Operator::EQUALS:
            return "==";
        case ast::Operator::NOT_EQUALS:
            return "!=";
        case ast::Operator::LESS:
            return "<";
        case ast::Operator::LESS_EQUALS:
            return "<=";
        case ast::Operator::GREATER:
            return ">";
        case ast::Operator::GREATER_EQUALS:
            return ">=";
        case ast::Operator::STAR:
            return "*";
        case ast::Operator::ADDRESS:
            return "&";
        case ast::Operator::BRACKET:
            return "[]";
        case ast::Operator::DOT:
            return ".";
        case ast::Operator::CALL:
            return "()";
    }

    eddic_unreachable("This operator cannot be converted to string");
}
