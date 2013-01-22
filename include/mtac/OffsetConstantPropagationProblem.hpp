//=======================================================================
// Copyright Baptiste Wicht 2011-2012.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#ifndef MTAC_OFFSET_CONSTANT_PROPAGATION_PROBLEM_H
#define MTAC_OFFSET_CONSTANT_PROPAGATION_PROBLEM_H

#include <memory>
#include <unordered_map>
#include <ostream>

#include <boost/utility.hpp>

#define STATIC_CONSTANT(type,name,value) BOOST_STATIC_CONSTANT(type, name = value)

#include "variant.hpp"
#include "assert.hpp"
#include "Platform.hpp"

#include "mtac/pass_traits.hpp"
#include "mtac/DataFlowProblem.hpp"
#include "mtac/Offset.hpp"      
#include "mtac/EscapeAnalysis.hpp"

namespace eddic {

struct StringPool;

namespace mtac {

typedef boost::variant<std::string, double, int, std::shared_ptr<Variable>> OffsetConstantValue;
typedef std::unordered_map<Offset, OffsetConstantValue, mtac::OffsetHash> OffsetConstantPropagationValues;

class OffsetConstantPropagationProblem {
    public:
        //The type of data managed
        typedef Domain<OffsetConstantPropagationValues> ProblemDomain;

        //The direction
        STATIC_CONSTANT(DataFlowType, Type, DataFlowType::Forward);

        std::unordered_set<Offset, mtac::OffsetHash> escaped;
        mtac::EscapedVariables pointer_escaped;

        void set_pool(std::shared_ptr<StringPool> string_pool);
        void set_platform(Platform platform);

        ProblemDomain Boundary(mtac::Function& function);
        ProblemDomain Init(mtac::Function& function) {
            //By default, return the top element
            return top_element();
        }

        void meet(ProblemDomain& in, const ProblemDomain& out);
        ProblemDomain transfer(mtac::basic_block_p basic_block, mtac::Quadruple& statement, ProblemDomain& in);
        bool optimize(mtac::Function& function, std::shared_ptr<DataFlowResults<ProblemDomain>> results);

    private:
        ProblemDomain top_element();
        ProblemDomain default_element();

        std::shared_ptr<StringPool> string_pool;
        Platform platform;
};

template<>
struct pass_traits<OffsetConstantPropagationProblem> {
    STATIC_CONSTANT(pass_type, type, pass_type::DATA_FLOW);
    STATIC_STRING(name, "offset_constant_propagation");
    STATIC_CONSTANT(unsigned int, property_flags, PROPERTY_POOL | PROPERTY_PLATFORM);
    STATIC_CONSTANT(unsigned int, todo_after_flags, 0);
};

} //end of mtac

} //end of eddic

#endif
