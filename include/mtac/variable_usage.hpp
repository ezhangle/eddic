//=======================================================================
// Copyright Baptiste Wicht 2011-2013.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef MTAC_VARIABLE_USAGE_H
#define MTAC_VARIABLE_USAGE_H

#include <unordered_map>

#include "mtac/pass_traits.hpp"
#include "mtac/forward.hpp"
#include "mtac/Loop.hpp"

namespace eddic {

namespace mtac {

struct Usage {
    std::unordered_map<std::shared_ptr<Variable>, unsigned int> written;
    std::unordered_map<std::shared_ptr<Variable>, unsigned int> read;
};

Usage compute_read_usage(mtac::Loop& loop);
Usage compute_read_usage(mtac::Function& function);

Usage compute_write_usage(mtac::Loop& loop);
Usage compute_write_usage(mtac::Function& function);

bool use_variable(mtac::basic_block_p bb, std::shared_ptr<Variable> var);

} //end of mtac

} //end of eddic

#endif
