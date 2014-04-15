//=======================================================================
// Copyright Baptiste Wicht 2011-2014.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#include "assert.hpp"
#include "logging.hpp"

#ifdef LOGGING_DISABLE

void eddic::configure_logging(int){}

#else

void eddic::configure_logging(int level){
    if(level == 0){
        ::logging::detail::Logger<Level>::logging()._level.l = Level::disable;
    } else if(level == 1){
        ::logging::detail::Logger<Level>::logging()._level.l = Level::info;
    } else if(level == 2){
        ::logging::detail::Logger<Level>::logging()._level.l = Level::trace;
    } else if(level == 3){
        ::logging::detail::Logger<Level>::logging()._level.l = Level::debug;
    } else if(level == 4){
        ::logging::detail::Logger<Level>::logging()._level.l = Level::user;
    } else {
        eddic_unreachable("Invalid log level");
    }
}

#endif
