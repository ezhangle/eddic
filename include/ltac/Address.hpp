//=======================================================================
// Copyright Baptiste Wicht 2011-2013.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef LTAC_ADDRESS_H
#define LTAC_ADDRESS_H

#include "variant.hpp"

#include <boost/optional.hpp>

#include "ltac/Register.hpp"
#include "ltac/FloatRegister.hpp"
#include "ltac/PseudoRegister.hpp"
#include "ltac/PseudoFloatRegister.hpp"

namespace eddic {

namespace ltac {

typedef boost::variant<
            ltac::Register, 
            ltac::PseudoRegister, 
            ltac::PseudoFloatRegister,  //Not used
            ltac::FloatRegister>        //Not used
    AddressRegister;

struct Address {
    boost::optional<ltac::AddressRegister> base_register;
    boost::optional<ltac::AddressRegister> scaled_register;
    boost::optional<unsigned int> scale;
    boost::optional<int> displacement;

    boost::optional<std::string> absolute;

    Address();
    Address(const std::string& absolute);
    Address(const std::string& absolute, const ltac::AddressRegister& reg);
    Address(const std::string& absolute, int displacement);
    
    Address(int displacement);
    Address(const ltac::AddressRegister& reg, int displacement);
    Address(const ltac::AddressRegister& reg, const ltac::AddressRegister& scaled);
    Address(const ltac::AddressRegister& reg, const ltac::AddressRegister& scaled, unsigned scale, int displacement);
};

bool operator==(ltac::Address& lhs, ltac::Address& rhs);
bool operator!=(ltac::Address& lhs, ltac::Address& rhs);

std::ostream& operator<<(std::ostream& out, const Address& address);

} //end of ltac

} //end of eddic

#endif
