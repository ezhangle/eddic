//=======================================================================
// Copyright Baptiste Wicht 2011-2013.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef PERFS_TIMER_H
#define PERFS_TIMER_H

#include <string>

#include "StopWatch.hpp"

namespace eddic {

class PerfsTimer {
    public:
        PerfsTimer(const std::string& n);
        PerfsTimer(const std::string& n, bool precise);

        ~PerfsTimer();
    
    private:
        StopWatch timer;
        std::string name;
        bool precise = false;
};

} //end of eddic

#endif
