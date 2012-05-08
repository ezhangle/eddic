//=======================================================================
// Copyright Baptiste Wicht 2011.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef INTEL_CODE_GENERATOR_H
#define INTEL_CODE_GENERATOR_H

#include <memory>
#include <string>

#include <boost/utility/enable_if.hpp>

#include "asm/CodeGenerator.hpp"

namespace eddic {

class AssemblyFileWriter;
class GlobalContext;

namespace as {

/*!
 * \class IntelCodeGenerator
 * \brief Base class for code generator on Intel platform. 
 */
class IntelCodeGenerator : public CodeGenerator {
    public:
        IntelCodeGenerator(AssemblyFileWriter& writer);
        
        void generate(ltac::Program& program, StringPool& pool, std::shared_ptr<FloatPool> float_pool);

    protected:
        void addGlobalVariables(std::shared_ptr<GlobalContext> context, StringPool& pool, std::shared_ptr<FloatPool> float_pool);
        
        virtual void writeRuntimeSupport() = 0;
        virtual void addStandardFunctions() = 0;
        virtual void compile(std::shared_ptr<ltac::Function> function) = 0;

        virtual void defineDataSection() = 0;

        virtual void declareIntArray(const std::string& name, unsigned int size) = 0;
        virtual void declareStringArray(const std::string& name, unsigned int size) = 0;
        virtual void declareFloatArray(const std::string& name, unsigned int size) = 0;

        virtual void declareIntVariable(const std::string& name, int value) = 0;
        virtual void declareStringVariable(const std::string& name, const std::string& label, int size) = 0;
        virtual void declareString(const std::string& label, const std::string& value) = 0;
        virtual void declareFloat(const std::string& label, double value) = 0;
};

} //end of as

} //end of eddic

#endif
