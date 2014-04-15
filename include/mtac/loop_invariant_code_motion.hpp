//=======================================================================
// Copyright Baptiste Wicht 2011-2014.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef MTAC_LOOP_INVARIANT_CODE_MOTION_H
#define MTAC_LOOP_INVARIANT_CODE_MOTION_H

#include "mtac/pass_traits.hpp"
#include "mtac/forward.hpp"

namespace eddic {

namespace mtac {

struct loop_invariant_code_motion {
    bool operator()(mtac::Function& function);
};

template<>
struct pass_traits<loop_invariant_code_motion> {
    STATIC_STRING(name, "loop_invariant_motion");
    STATIC_CONSTANT(pass_type, type, pass_type::CUSTOM);
    STATIC_CONSTANT(unsigned int, property_flags, 0);
    STATIC_CONSTANT(unsigned int, todo_after_flags, 0);
};

} //end of mtac

} //end of eddic

#endif
