//=======================================================================
// Copyright Baptiste Wicht 2011-2016.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef MTAC_POINTER_PROPAGATION_H
#define MTAC_POINTER_PROPAGATION_H

#include <memory>
#include <unordered_map>

#include "variant.hpp"

#include "mtac/pass_traits.hpp"
#include "mtac/Quadruple.hpp"

namespace eddic {

namespace mtac {

class PointerPropagation {
    public:
        bool optimized = false;

        void clear();

        void operator()(mtac::Quadruple& quadruple);

    private:
        std::unordered_map<std::shared_ptr<Variable>, std::shared_ptr<Variable>> aliases;
        std::unordered_map<std::shared_ptr<Variable>, std::shared_ptr<Variable>> pointer_copies;
};

template<>
struct pass_traits<PointerPropagation> {
    STATIC_CONSTANT(pass_type, type, pass_type::BB);
    STATIC_STRING(name, "pointer_propagation");
    STATIC_CONSTANT(unsigned int, property_flags, 0);
    STATIC_CONSTANT(unsigned int, todo_after_flags, 0);
};

} //end of mtac

} //end of eddic

#endif
