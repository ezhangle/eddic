//=======================================================================
// Copyright Baptiste Wicht 2011.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef INTERMEDIATE_COMPILER_H
#define INTERMEDIATE_COMPILER_H

#include "ast/Program.hpp"

namespace eddic {

class StringPool;
class IntermediateProgram;

struct IntermediateCompiler {
    void compile(ASTProgram& program, StringPool& pool, IntermediateProgram& intermediateProgram);
};

} //end of eddic

#endif