//=======================================================================
// Copyright Baptiste Wicht 2011.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef TAC_PROGRAM_H
#define TAC_PROGRAM_H

#include <memory>
#include <vector>

#include "mtac/Function.hpp"

namespace eddic {

struct GlobalContext;

namespace mtac {

struct Program {
    std::shared_ptr<GlobalContext> context;

    std::vector<std::shared_ptr<Function>> functions;
};

} //end of mtac

} //end of eddic

#endif
