//=======================================================================
// Copyright Baptiste Wicht 2011.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef MTAC_REMOVE_MULTIPLE_ASSIGN_H
#define MTAC_REMOVE_MULTIPLE_ASSIGN_H

#include <memory>
#include <unordered_set>
#include <unordered_map>

#include <boost/variant/static_visitor.hpp>

#include "mtac/Pass.hpp"
#include "mtac/Quadruple.hpp"
#include "mtac/IfFalse.hpp"
#include "mtac/If.hpp"
#include "mtac/Param.hpp"

namespace eddic {

namespace mtac {

class RemoveMultipleAssign : public boost::static_visitor<void> {
    public:
        bool optimized = false;
        Pass pass;

        void operator()(std::shared_ptr<mtac::Quadruple>& quadruple);
        void operator()(std::shared_ptr<mtac::Param>& param);
        void operator()(std::shared_ptr<mtac::IfFalse>& ifFalse);
        void operator()(std::shared_ptr<mtac::If>& if_);

        template<typename T>
        void operator()(T&) const { 
            //Nothing to do
        }
    
        void collect(mtac::Argument* arg);
        void collect(boost::optional<mtac::Argument>& arg);

    private:
        std::unordered_set<std::shared_ptr<Variable>> used;
        std::unordered_map<std::shared_ptr<Variable>, std::shared_ptr<mtac::Quadruple>> lastAssign;
        std::unordered_set<std::shared_ptr<mtac::Quadruple>> removed;
};

} //end of mtac

} //end of eddic

#endif