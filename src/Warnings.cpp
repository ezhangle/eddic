//=======================================================================
// Copyright Baptiste Wicht 2011-2016.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#include <iostream>

#include "Warnings.hpp"

void eddic::warn(const std::string& warning){
    std::cout << "warning: " << warning << std::endl;
}

void eddic::warn(const std::string& position, const std::string& warning){
    std::cout << position << std::endl << "warning: " << warning << std::endl;
}
