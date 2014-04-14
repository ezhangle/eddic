//=======================================================================
// Copyright Baptiste Wicht 2011-2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef SPIRIT_LEXER_H
#define SPIRIT_LEXER_H

#include <fstream>
#include <string>
#include <utility>
#include <stack>

#include "boost_cfg.hpp"

#include <boost/spirit/include/classic_position_iterator.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/lex_static_lexertl.hpp>
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_functor_parser.hpp>
#include <boost/spirit/include/classic_attribute.hpp>
#include <boost/spirit/include/classic_symbols.hpp>

#include "lexer/static_lexer.hpp"

namespace eddic {

namespace lexer {

namespace spirit = boost::spirit;
namespace lex = boost::spirit::lex;

/*!
 * \class SimpleLexer
 * \brief The EDDI lexer.
 *
 * This class is used to do lexical analysis on an EDDI source file. This file is based on a Boost Spirit Lexer. It's
 * used by the parser to parse a source file.
 */

template<typename L>
class SpiritLexer : public lex::lexer<L> {
    public:
        SpiritLexer() {
            /* keywords  */
            for_ = "for";
            while_ = "while";
            do_ = "do";
            if_ = "if";
            else_ = "else";
            false_ = "false";
            true_ = "true";
            from_ = "from";
            to_ = "to";
            foreach_ = "foreach";
            in_ = "in";
            return_ = "return";
            const_ = "const";
            include = "include";
            struct_ = "struct";
            null = "null";
            this_ = "this";
            new_ = "new";
            delete_ = "delete";
            switch_ = "switch";
            case_ = "case";
            default_ = "default";
            type = "type";
            template_ = "template";
            extends = "extends";

            /* Raw values  */
            identifier = "[a-zA-Z_][a-zA-Z0-9_]*";
            float_ = "[0-9]+\".\"[0-9]+";
            integer = "[0-9]+";
            string_literal = "\\\"[^\\\"]*\\\"";
            char_literal = "'.'";

            /* Constructs  */
            left_parenth = '(';
            right_parenth = ')';
            left_brace = '{';
            right_brace = '}';
            left_bracket = '[';
            right_bracket = ']';

            stop = ';';
            comma = ',';
            dot = '.';

            tilde = '~';

            /* Ternary operator */
            double_dot = ':';
            question_mark = '?';

            /* Assignment operators */
            swap = "<=>";
            assign = '=';

            /* compound assignment operators */
            compound_add = "\\+=";
            compound_sub = "-=";
            compound_mul = "\\*=";
            compound_div = "\\/=";
            compound_mod = "%=";

            /* Binary operators  */
            addition = '+';
            subtraction = '-';
            multiplication = '*';
            division = '/';
            modulo = '%';

            /* Unary operators */
            not_ = '!';
            addressof = "\\&";

            /* Suffix and prefix math operators  */
            increment = "\\+\\+";
            decrement = "--";

            /* Logical operators */
            and_ = "\\&\\&";
            or_ = "\\|\\|";

            /* Relational operators  */
            equals = "==";
            not_equals = "!=";
            greater = ">";
            less = "<";
            greater_equals = ">=";
            less_equals = "<=";

            whitespaces = "[ \\t\\n]+";
            multiline_comment = "\\/\\*[^*]*\\*+([^/*][^*]*\\*+)*\\/";
            singleline_comment = "\\/\\/[^\n]*";

            //Ignore whitespaces
            this->self += whitespaces [lex::_pass = lex::pass_flags::pass_ignore];

            this->self += left_parenth | right_parenth | left_brace | right_brace | left_bracket | right_bracket;
            this->self += float_ | integer | string_literal ;
            this->self += assign | swap;
            this->self += comma | stop | dot;
            this->self += addition | subtraction | multiplication | division | modulo;
            this->self += compound_add | compound_sub | compound_mul | compound_div | compound_mod;
            this->self += increment | decrement;
            this->self += for_ | do_ | while_ | true_ | false_ | if_ | else_ | from_ | to_ | in_ | foreach_ | return_ | const_ | include | struct_ | null | this_;
            this->self += new_ | delete_;
            this->self += and_ | or_;
            this->self += addressof;
            this->self += equals | not_equals | greater_equals | less_equals | greater | less | not_;
            this->self += double_dot | question_mark | tilde;
            this->self += template_ | type | extends;
            this->self += case_ | switch_ | default_;
            this->self += identifier | char_literal;

            //Ignore comments
            this->self += multiline_comment [lex::_pass = lex::pass_flags::pass_ignore];
            this->self += singleline_comment [lex::_pass = lex::pass_flags::pass_ignore];
        }

        typedef lex::token_def<lex::omit> ConsumedToken;
        typedef lex::token_def<std::string> StringToken;
        typedef lex::token_def<int> IntegerToken;
        typedef lex::token_def<char> CharToken;
        typedef lex::token_def<double> FloatToken;

        StringToken identifier, string_literal, char_literal;
        IntegerToken integer;
        FloatToken float_;

        CharToken addition, subtraction, multiplication, division, modulo, not_, addressof;
        StringToken increment, decrement;
        StringToken compound_add, compound_sub, compound_mul, compound_div, compound_mod;
        StringToken equals, not_equals, greater, less, greater_equals, less_equals;
        StringToken and_, or_;

        ConsumedToken left_parenth, right_parenth, left_brace, right_brace, left_bracket, right_bracket;
        ConsumedToken stop, comma, dot;
        ConsumedToken assign, swap;
        ConsumedToken question_mark, double_dot, tilde;

        //Keywords
        ConsumedToken if_, else_, for_, while_, do_, from_, in_, to_, foreach_, return_;
        ConsumedToken true_, false_;
        ConsumedToken const_, include;
        ConsumedToken struct_, null;
        ConsumedToken case_, switch_, default_;
        ConsumedToken new_, delete_;
        ConsumedToken template_, type, extends;
        StringToken this_; //As this is handled like a variable, we need its value

        //Ignored tokens
        ConsumedToken whitespaces, singleline_comment, multiline_comment;
};

//Token definitions
typedef std::string::iterator base_iterator_type;
typedef boost::spirit::classic::position_iterator2<base_iterator_type> pos_iterator_type;
typedef boost::spirit::lex::lexertl::token<pos_iterator_type> Tok;

//Lexer Types
typedef lex::lexertl::actor_lexer<Tok> dynamic_lexer_type;
typedef boost::spirit::lex::lexertl::static_actor_lexer<lexer::Tok, boost::spirit::lex::lexertl::static_::lexer_sl> static_lexer_type;

//Typedef for the parsers
typedef lexer::SpiritLexer<dynamic_lexer_type> DynamicLexer;
typedef lexer::SpiritLexer<static_lexer_type> StaticLexer;

//Iterators
typedef static_lexer_type::iterator_type StaticIterator;

} //end of lexer

} //end of eddic

#endif
