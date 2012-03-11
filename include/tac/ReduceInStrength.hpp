//=======================================================================
// Copyright Baptiste Wicht 2011.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef TAC_REDUCE_IN_STRENGTH_H
#define TAC_REDUCE_IN_STRENGTH_H

#include <memory>

#include <boost/variant/static_visitor.hpp>

#include "tac/Quadruple.hpp"

namespace eddic {

namespace tac {

struct ReduceInStrength : public boost::static_visitor<void> {
    bool optimized;

    ReduceInStrength() : optimized(false) {}

    void operator()(std::shared_ptr<tac::Quadruple>& quadruple);

    template<typename T>
    void operator()(T&) const { 
        //Nothing to optimize
    }
};

} //end of tac

} //end of eddic

#endif