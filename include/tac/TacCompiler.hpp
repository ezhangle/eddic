//=======================================================================
// Copyright Baptiste Wicht 2011.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef TAC_COMPILER_H
#define TAC_COMPILER_H

#include "ast/source_def.hpp"

namespace eddic {

class StringPool;

namespace tac {

class Program;

struct TacCompiler {
    void compile(ast::SourceFile& program, StringPool& pool, tac::Program& tacProgram) const ;
};

} //end of tac

} //end of eddic

#endif