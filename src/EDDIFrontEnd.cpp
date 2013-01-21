//=======================================================================
// Copyright Baptiste Wicht 2011-2012.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include "EDDIFrontEnd.hpp"
#include "SemanticalException.hpp"
#include "Options.hpp"
#include "StringPool.hpp"
#include "Type.hpp"
#include "GlobalContext.hpp"
#include "PerfsTimer.hpp"

#include "parser/SpiritParser.hpp"

#include "ast/SourceFile.hpp"
#include "ast/PassManager.hpp"
#include "ast/DependenciesResolver.hpp"
#include "ast/Printer.hpp"

#include "mtac/Compiler.hpp"

using namespace eddic;

void check_for_main(std::shared_ptr<GlobalContext> context);
void generate_program(ast::SourceFile& program, std::shared_ptr<Configuration> configuration, Platform platform, std::shared_ptr<StringPool> pool);

std::unique_ptr<mtac::Program> EDDIFrontEnd::compile(const std::string& file, Platform platform){
    //The program to build
    ast::SourceFile source;

    //Parse the file into the program
    parser::SpiritParser parser;
    bool parsing = parser.parse(file, source); 

    //If the parsing was successfully
    if(parsing){
        set_string_pool(std::make_shared<StringPool>());

        //Read dependencies
        resolveDependencies(source, parser);
        
        //If the user asked for it, print the Abstract Syntax Tree coming from the parser
        if(configuration->option_defined("ast-raw")){
            ast::Printer printer;
            printer.print(source);
        }
        
        //AST Passes
        generate_program(source, configuration, platform, pool);

        //If the user asked for it, print the Abstract Syntax Tree
        if(configuration->option_defined("ast") || configuration->option_defined("ast-only")){
            ast::Printer printer;
            printer.print(source);
        }
        
        //If the user wants only the AST prints, it is not necessary to compile the AST
        if(configuration->option_defined("ast-only")){
            return nullptr;
        }

        std::unique_ptr<mtac::Program> program(new mtac::Program());

        //Generate Three-Address-Code language
        mtac::Compiler compiler;
        compiler.compile(source, pool, *program);

        return program;
    }

    //If the parsing fails, the error is already printed to the console
    return nullptr;
}

void generate_program(ast::SourceFile& source, std::shared_ptr<Configuration> configuration, Platform platform, std::shared_ptr<StringPool> pool){
    PerfsTimer timer("AST Passes");

    //Initialize the passes
    ast::PassManager pass_manager(platform, configuration, source, pool);
    pass_manager.init_passes();

    //Run all the passes on the program
    pass_manager.run_passes();

    //Check that there is a main in the program
    check_for_main(source.Content->context);
}

void check_for_main(std::shared_ptr<GlobalContext> context){
    if(!context->exists("_F4main") && !context->exists("_F4mainAS")){
        throw SemanticalException("The program does not contain a valid main function"); 
    }
}
