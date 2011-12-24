//=======================================================================
// Copyright Baptiste Wicht 2011.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef DEPENDENCIES_RESOLVER_H
#define DEPENDENCIES_RESOLVER_H

#include "ast/source_def.hpp"

namespace eddic {

class SpiritParser;

struct DependenciesResolver {
    DependenciesResolver(SpiritParser& parser);

    void resolve(ast::SourceFile& program) const;

    private:
        SpiritParser& parser;
};

} //end of eddic

#endif