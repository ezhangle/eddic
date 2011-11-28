//=======================================================================
// Copyright Baptiste Wicht 2011.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef GLOBAL_CONTEXT_H
#define GLOBAL_CONTEXT_H

#include <string>
#include <memory>

#include "Types.hpp"

#include "Context.hpp"

namespace eddic {

class GlobalContext : public Context {
    public:
        GlobalContext();
        
        void writeIL(IntermediateProgram& writer);
        
        std::shared_ptr<Variable> addVariable(const std::string& a, Type type);
        std::shared_ptr<Variable> addVariable(const std::string& a, Type type, ast::Value& value);
};

} //end of eddic

#endif
