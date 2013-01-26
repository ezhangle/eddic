//=======================================================================
// Copyright Baptiste Wicht 2011-2013.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef AST_IMPORT_H
#define AST_IMPORT_H

#include <boost/fusion/include/adapt_struct.hpp>

#include "ast/Position.hpp"

namespace eddic {

namespace ast {

/*!
 * \class Import
 * \brief The AST node for an import.    
 */
struct Import {
    Position position;
    std::string file;
};

} //end of ast

} //end of eddic

//Adapt the struct for the AST
BOOST_FUSION_ADAPT_STRUCT(
    eddic::ast::Import, 
    (eddic::ast::Position, position)
    (std::string, file)
)

#endif
