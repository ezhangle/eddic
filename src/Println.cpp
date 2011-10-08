//=======================================================================
// Copyright Baptiste Wicht 2011.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include <iostream>

#include "Println.hpp"
#include "AssemblyFileWriter.hpp"

using namespace eddic;

Println::Println(std::shared_ptr<Context> context, const std::shared_ptr<Token> token, std::shared_ptr<Value> v) : Print(context, token, v) {}

void Println::write(AssemblyFileWriter& writer) {
    Print::write(writer);

    writer.stream() << "call print_line" << std::endl;
}