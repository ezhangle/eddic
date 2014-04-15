//=======================================================================
// Copyright Baptiste Wicht 2011-2014.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#include "FunctionContext.hpp"
#include "Variable.hpp"
#include "Utils.hpp"
#include "VisitorUtils.hpp"
#include "Type.hpp"
#include "Options.hpp"
#include "logging.hpp"
#include "GlobalContext.hpp"

#include "ast/GetConstantValue.hpp"

using namespace eddic;

FunctionContext::FunctionContext(std::shared_ptr<Context> parent, std::shared_ptr<GlobalContext> global_context, Platform platform, std::shared_ptr<Configuration> configuration) :
        Context(parent, global_context), platform(platform) {
    //TODO Should not be done here
    if(configuration->option_defined("fomit-frame-pointer")){
        currentParameter = INT->size(platform);
    } else {
        currentParameter = 2 * INT->size(platform);
    }
}

int FunctionContext::size() const {
    int size = -currentPosition;

    if(size == static_cast<int>(INT->size(platform))){
        return 0;
    }

    return size;
}

int FunctionContext::stack_position(){
    return currentPosition;
}

void FunctionContext::set_stack_position(int current){
    currentPosition = current;
}

std::shared_ptr<Variable> FunctionContext::newParameter(const std::string& variable, std::shared_ptr<const Type> type){
    Position position(PositionType::PARAMETER, currentParameter);

    LOG<Info>("Variables") << "New parameter " << variable << " at position " << currentParameter << log::endl;

    currentParameter += type->size(platform);

    return std::make_shared<Variable>(variable, type, position);
}

std::shared_ptr<Variable> FunctionContext::newVariable(const std::string& variable, std::shared_ptr<const Type> type){
    auto var = std::make_shared<Variable>(variable, type, Position(PositionType::VARIABLE));

    storage.push_back(var);

    return var;
}

Storage FunctionContext::stored_variables(){
    return storage;
}

std::shared_ptr<Variable> FunctionContext::addVariable(const std::string& variable, std::shared_ptr<const Type> type){
    return variables[variable] = newVariable(variable, type);
}

std::shared_ptr<Variable> FunctionContext::newVariable(std::shared_ptr<Variable> source){
    std::string name = "g_" + source->name() + "_" + toString(temporary++);

    if(source->position().is_temporary()){
        Position position(PositionType::TEMPORARY);

        auto var = std::make_shared<Variable>(name, source->type(), position);
        storage.push_back(var);
        return variables[name] = var;
    } else {
        return addVariable(name, source->type());
    }
}

std::shared_ptr<Variable> FunctionContext::addVariable(const std::string& variable, std::shared_ptr<const Type> type, ast::Value& value){
    assert(type->is_const());

    Position position(PositionType::CONST);

    auto val = visit(ast::GetConstantValue(), value);

    auto var = std::make_shared<Variable>(variable, type, position, val);
    return variables[variable] = var;
}

std::shared_ptr<Variable> FunctionContext::generate_variable(const std::string& prefix, std::shared_ptr<const Type> type){
    std::string name = prefix + "_" + toString(generated++);
    return addVariable(name, type);
}

std::shared_ptr<Variable> FunctionContext::addParameter(const std::string& parameter, std::shared_ptr<const Type> type){
    return variables[parameter] = newParameter(parameter, type);
}

std::shared_ptr<Variable> FunctionContext::new_temporary(std::shared_ptr<const Type> type){
    eddic_assert((type->is_standard_type() && type != STRING) || type->is_pointer() || type->is_dynamic_array(), "Invalid temporary");

    Position position(PositionType::TEMPORARY);

    std::string name = "t_" + toString(temporary++);
    auto var = std::make_shared<Variable>(name, type, position);
    storage.push_back(var);
    return variables[name] = var;
}

std::shared_ptr<Variable> FunctionContext::new_reference(std::shared_ptr<const Type> type, std::shared_ptr<Variable> var, Offset offset){
    std::string name = "t_" + toString(temporary++);
    auto variable = std::make_shared<Variable>(name, type, var, offset);
    storage.push_back(variable);
    return variables[name] = variable;
}

void FunctionContext::allocate_in_param_register(std::shared_ptr<Variable> variable, unsigned int register_){
    assert(variable->position().isParameter());

    Position position(PositionType::PARAM_REGISTER, register_);
    variable->setPosition(position);
}

void FunctionContext::removeVariable(std::shared_ptr<Variable> variable){
    auto iter_var = std::find(storage.begin(), storage.end(), variable);
    auto platform = global()->target_platform();

    if(variable->position().isParameter()){
        variables.erase(variable->name());

        for(auto& v : variables){
            if(v.second->position().isParameter()){
                if(v.second->position().offset() > variable->position().offset()){
                    Position position(PositionType::PARAMETER, v.second->position().offset() - variable->type()->size(platform));
                    v.second->setPosition(position);
                }
            }
        }

        currentParameter -= variable->type()->size(platform);

        LOG<Info>("Variables") << "Remove parameter " << variable->name() << log::endl;
    } else {
        variables.erase(variable->name());
        storage.erase(iter_var);

        LOG<Info>("Variables") << "Remove variable " << variable->name() << log::endl;
    }
}

std::shared_ptr<FunctionContext> FunctionContext::function(){
    return shared_from_this();
}
