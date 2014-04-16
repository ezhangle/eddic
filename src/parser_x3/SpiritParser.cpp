//=======================================================================
// Copyright Baptiste Wicht 2011-2014.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#include <iomanip>
#include <istream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

//#include "boost_cfg.hpp"

#include <boost/mpl/vector.hpp>
#include <boost/mpl/count.hpp>

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>

//#include "GlobalContext.hpp"

#include "parser_x3/SpiritParser.hpp"

namespace x3 = boost::spirit::x3;

using namespace eddic;

template<typename ForwardIteratorT>
class extended_iterator : public boost::spirit::classic::position_iterator2<ForwardIteratorT> {
    std::size_t current_file;

public:
    extended_iterator() : boost::spirit::classic::position_iterator2<ForwardIteratorT>() {
        //Nothing
    }

    template <typename FileNameT>
    extended_iterator(
        const ForwardIteratorT& begin, const ForwardIteratorT& end,
        FileNameT file, std::size_t current_file)
            : boost::spirit::classic::position_iterator2<ForwardIteratorT>(begin, end, file), current_file(current_file)
    {}

    std::size_t get_current_file() const {
        return current_file;
    }

    extended_iterator(const extended_iterator& iter) = default;
    extended_iterator& operator=(const extended_iterator& iter) = default;
};


namespace x3_ast {

struct position {
    std::size_t file = 0;               /*!< The source file */
    std::size_t line = 0;               /*!< The source line number */
    std::size_t column = 0;             /*!< The source column number */
};

struct function_declaration {
    position pos;
    std::string type;
    std::string name;
};

struct standard_import {
    position pos;
    int fake_;
    std::string file;
};

struct import {
    position pos;
    int fake_;
    std::string file;
};

typedef x3::variant<
        function_declaration,
        standard_import,
        import
    > block;

struct source_file {
    std::vector<block> blocks;
};

} //end of x3_ast namespace

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::source_file,
    (std::vector<x3_ast::block>, blocks)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::function_declaration,
    (std::string, type)
    (std::string, name)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::import,
    (int, fake_)
    (std::string, file)
)

BOOST_FUSION_ADAPT_STRUCT(
    x3_ast::standard_import,
    (int, fake_)
    (std::string, file)
)

namespace x3_grammar {

    //Rule IDs

    typedef x3::identity<struct source_file> source_file_id;
    typedef x3::identity<struct function_declaration> function_declaration_id;
    typedef x3::identity<struct import> import_id;
    typedef x3::identity<struct standard_import> standard_import_id;

    x3::rule<source_file_id, x3_ast::source_file> const source_file("source_file");
    x3::rule<function_declaration_id, x3_ast::function_declaration> const function_declaration("function_declaration");
    x3::rule<standard_import_id, x3_ast::standard_import> const standard_import("standard_import");
    x3::rule<import_id, x3_ast::import> const import("import");

    typedef boost::mpl::vector<import_id, standard_import_id, function_declaration_id> annotated_ids;
    typedef boost::mpl::vector<x3_ast::import, x3_ast::standard_import, x3_ast::function_declaration>::type annotated_asts;

    template <typename iterator_type, typename Attr, typename Context>
    inline void on_success(standard_import_id, const iterator_type& first, const iterator_type&, Attr& attr, Context const&){
        auto& pos = first.get_position();

        attr.pos.file = first.get_current_file();
        attr.pos.line = pos.line;
        attr.pos.column = pos.column;
    }

    auto const standard_import_def = 
            x3::attr(1)
        >>  "import"
        >>  '<' 
        >> *x3::alpha
        >>  '>';
    
    auto const import_def = 
            x3::attr(1)
        >>  "import"
        >>  '"' 
        >> *x3::alpha
        >>  '"';
    
    auto const function_declaration_def = *x3::alpha >> *x3::alpha >> '(' >> ')';
    
    auto const source_file_def = 
         *(
                function_declaration
            |   standard_import
            |   import
         );
    
    auto const parser = x3::grammar(
        "eddi", 
        source_file = source_file_def,
        function_declaration = function_declaration_def, 
        standard_import = standard_import_def,
        import = import_def);

} // end of grammar namespace

bool parser_x3::SpiritParser::parse(const std::string& file/*, ast::SourceFile& , std::shared_ptr<GlobalContext> context*/){
    //timing_timer timer(context->timing(), "parsing_x3");

    std::ifstream in(file.c_str(), std::ios::binary);
    in.unsetf(std::ios::skipws);

    //Collect the size of the file
    in.seekg(0, std::istream::end);
    std::size_t size(static_cast<size_t>(in.tellg()));
    in.seekg(0, std::istream::beg);

    //int current_file = context->new_file(file);

    //std::string& file_contents = context->get_file_content(current_file);
    std::string file_contents;
    file_contents.resize(size);
    in.read(&file_contents[0], size);

    auto& parser = x3_grammar::parser;

    x3_ast::source_file result;
    boost::spirit::x3::ascii::space_type space;

    typedef std::string::iterator base_iterator_type;
    //typedef boost::spirit::classic::position_iterator2<base_iterator_type> pos_iterator_type;
    typedef extended_iterator<base_iterator_type> pos_iterator_type;

    pos_iterator_type it(file_contents.begin(), file_contents.end(), file, 1);
    pos_iterator_type end;

    try {
        bool r = x3::phrase_parse(it, end, parser, space, result);

        if(r && it == end){
            std::cout << "Blocks: " << result.blocks.size() << std::endl;

            return true;
        } else {
            //TODO
            std::cout << std::string(it, end) << std::endl;

            return false;
        }
    } catch(const boost::spirit::x3::expectation_failure<pos_iterator_type>& e){
        //TODO
        return false;
    }
}
