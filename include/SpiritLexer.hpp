//=======================================================================
// Copyright Baptiste Wicht 2011.
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

#include "SpiritToken.hpp"
#include <boost/spirit/include/lex_lexertl.hpp>

namespace eddic {

namespace spirit = boost::spirit;
namespace lex = boost::spirit::lex;

typedef lex::lexertl::actor_lexer<Tok> lexer_type;

template<typename L>
class SimpleLexer : public lex::lexer<L> {
    private:

    public:
        SimpleLexer() {
            keyword_for = "for";
            keyword_while = "while";
            keyword_if = "if";
            keyword_else = "else";
            keyword_false = "false";
            keyword_true = "true";
            keyword_from = "from";
            keyword_to = "to";
            keyword_foreach = "foreach";

            word = "[a-zA-Z]+";
            integer = "[0-9]+";
            litteral = "\\\"[^\\\"]*\\\"";

            left_parenth = '('; 
            right_parenth = ')'; 
            left_brace = '{'; 
            right_brace = '}'; 

            stop = ';';
            comma = ',';

            swap = "<>";
            assign = '=';
            addition = '+';
            subtraction = '-';
            multiplication = '*';
            division = '/';
            modulo = '%';

            equals = "==";
            not_equals = "!=";
            greater = '>';
            less = '<';
            greater_equals = ">=";
            less_equals = "<=";

            whitespaces = "[ \\t\\n]+";
            comments = "\\/\\*[^*]*\\*+([^/*][^*]*\\*+)*\\/";

            //Add keywords
            this->self += keyword_for | keyword_while | keyword_true | keyword_false | keyword_if | keyword_else | keyword_from | keyword_to | keyword_foreach;
            this->self += integer | word | litteral;
           
            this->self += equals | not_equals | greater_equals | less_equals | greater | less ;
            this->self += left_parenth | right_parenth | left_brace | right_brace;
            this->self += comma | stop;
            this->self += assign | swap | addition | subtraction | multiplication | division | modulo;

            //Ignore whitespaces and comments
            this->self += whitespaces [lex::_pass = lex::pass_flags::pass_ignore];
            this->self += comments [lex::_pass = lex::pass_flags::pass_ignore]; 
        }
      
        typedef lex::token_def<lex::omit> ConsumedToken;
        typedef lex::token_def<std::string> StringToken;
        typedef lex::token_def<int> IntegerToken;

        StringToken word, litteral;
        IntegerToken integer;

        ConsumedToken left_parenth, right_parenth, left_brace, right_brace;
        
        ConsumedToken stop, comma;
        
        ConsumedToken assign, swap, addition, subtraction, multiplication, division, modulo;
        ConsumedToken equals, not_equals, greater, less, greater_equals, less_equals;
        
        //Keywords
        ConsumedToken keyword_if, keyword_else, keyword_for, keyword_while, keyword_from, keyword_to, keyword_foreach;
        ConsumedToken keyword_true, keyword_false;

        //Ignored tokens
        ConsumedToken whitespaces;
        ConsumedToken comments;
};

class SpiritLexer {
    private:
        std::ifstream stream;
        
        SimpleLexer<lexer_type> lexer;
        
        lexer_type::iterator_type iter;
        lexer_type::iterator_type end;

		Tok def;

        //Tok& current;
        Tok& defaultToken;

		bool first;

        //std::stack<std::reference_wrapper<const Tok>> buffer;

        bool readNext();
	
    public:
        SpiritLexer();

        void lex(const std::string& file);
        bool next();
        void pushBack(const Tok& token);

        const Tok& getCurrentToken() const;
        const Tok& getDefaultToken() const;

        bool isWord() const;
        bool isAssign() const;
        bool isSwap() const;
        bool isLitteral() const;
        bool isLeftParenth() const;
        bool isRightParenth() const;
        bool isLeftBrace() const;
        bool isRightBrace() const;
        bool isStop() const;
        bool isInteger() const;
        bool isAddition() const;
        bool isSubtraction() const;
        bool isMultiplication() const;
        bool isDivision() const;
        bool isModulo() const;
        bool isEquals() const;
        bool isNotEquals() const;
        bool isGreater() const;
        bool isLess() const;
        bool isGreaterOrEquals() const;
        bool isLessOrEquals() const;
        bool isIf() const;
        bool isElse() const;
        bool isWhile() const;
        bool isFor() const;
        bool isForeach() const;
        bool isFrom() const;
        bool isTo() const;
        bool isTrue() const;
        bool isFalse() const;
        bool isComma() const;
    
};

} //end of eddic

#endif
