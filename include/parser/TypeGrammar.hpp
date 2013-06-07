//=======================================================================
// Copyright Baptiste Wicht 2011-2013.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef TYPE_GRAMMAR_H
#define TYPE_GRAMMAR_H

#include "boost_cfg.hpp"
#include <boost/spirit/include/qi.hpp>

#include "lexer/SpiritLexer.hpp"

#include "ast/VariableType.hpp"
#include "ast/Struct.hpp"

namespace qi = boost::spirit::qi;

namespace eddic {

namespace parser {

/*!
 * \class TypeGrammar
 * \brief Grammar representing types in EDDI language.
 */
struct TypeGrammar : qi::grammar<lexer::Iterator, ast::Type()> {
    TypeGrammar(const lexer::Lexer& lexer);

    qi::rule<lexer::Iterator, ast::Type()> type;
};

} //end of parser

} //end of eddic

#endif
