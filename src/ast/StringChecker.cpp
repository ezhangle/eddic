//=======================================================================
// Copyright Baptiste Wicht 2011-2016.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#include "variant.hpp"
#include "StringPool.hpp"

#include "ast/StringChecker.hpp"
#include "ast/SourceFile.hpp"
#include "ast/ASTVisitor.hpp"

using namespace eddic;

namespace {

struct StringCheckerVisitor : public boost::static_visitor<> {
    StringPool& pool;

    StringCheckerVisitor(StringPool& p) : pool(p) {}

    AUTO_RECURSE_PROGRAM()
    AUTO_RECURSE_FUNCTION_DECLARATION()
    AUTO_RECURSE_CONSTRUCTOR()
    AUTO_RECURSE_DESTRUCTOR()
    AUTO_RECURSE_GLOBAL_DECLARATION()
    AUTO_RECURSE_FUNCTION_CALLS()
    AUTO_RECURSE_BUILTIN_OPERATORS()
    AUTO_RECURSE_SIMPLE_LOOPS()
    AUTO_RECURSE_FOREACH()
    AUTO_RECURSE_BRANCHES()
    AUTO_RECURSE_COMPOSED_VALUES()
    AUTO_RECURSE_RETURN_VALUES()
    AUTO_RECURSE_VARIABLE_OPERATIONS()
    AUTO_RECURSE_STRUCT_DECLARATION()
    AUTO_RECURSE_TERNARY()
    AUTO_RECURSE_SWITCH()
    AUTO_RECURSE_SWITCH_CASE()
    AUTO_RECURSE_DEFAULT_CASE()
    AUTO_RECURSE_NEW()
    AUTO_RECURSE_DELETE()
    AUTO_RECURSE_SCOPE()

    void operator()(ast::struct_definition& struct_){
        if(!struct_.is_template_declaration()){
            visit_each(*this, struct_.blocks);
        }
    }

    void operator()(ast::Literal& literal){
        literal.label = pool.label(literal.value);
    }

    AUTO_FORWARD()
    AUTO_IGNORE_OTHERS()
};

} //end of anonymous namespace

void ast::StringCollectionPass::apply_program(ast::SourceFile& program, bool){
   StringCheckerVisitor visitor(*pool);
   visitor(program);
}

bool ast::StringCollectionPass::is_simple(){
    return true;
}
