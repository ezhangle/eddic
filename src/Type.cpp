//=======================================================================
// Copyright Baptiste Wicht 2011-2016.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#include "cpp_utils/assert.hpp"

#include "mangling.hpp"
#include "Type.hpp"
#include "Platform.hpp"
#include "GlobalContext.hpp"

using namespace eddic;

/* Standard Types */

std::shared_ptr<const Type> eddic::BOOL = std::make_shared<StandardType>(BaseType::BOOL, false);
std::shared_ptr<const Type> eddic::INT = std::make_shared<StandardType>(BaseType::INT, false);
std::shared_ptr<const Type> eddic::CHAR = std::make_shared<StandardType>(BaseType::CHAR, false);
std::shared_ptr<const Type> eddic::FLOAT = std::make_shared<StandardType>(BaseType::FLOAT, false);
std::shared_ptr<const Type> eddic::STRING = std::make_shared<StandardType>(BaseType::STRING, false);
std::shared_ptr<const Type> eddic::VOID = std::make_shared<StandardType>(BaseType::VOID, false);

/* Const versions */

const std::shared_ptr<const Type> CBOOL = std::make_shared<StandardType>(BaseType::BOOL, true);
const std::shared_ptr<const Type> CINT = std::make_shared<StandardType>(BaseType::INT, true);
const std::shared_ptr<const Type> CCHAR = std::make_shared<StandardType>(BaseType::CHAR, true);
const std::shared_ptr<const Type> CFLOAT = std::make_shared<StandardType>(BaseType::FLOAT, true);
const std::shared_ptr<const Type> CSTRING = std::make_shared<StandardType>(BaseType::STRING, true);
const std::shared_ptr<const Type> CVOID = std::make_shared<StandardType>(BaseType::VOID, true);

/* Implementation of Type */ 

Type::Type() {}

bool Type::is_array() const {
    return false;
}

bool Type::is_dynamic_array() const {
    return is_array() && !has_elements();
}

bool Type::is_structure() const {
    return is_custom_type() || is_template_type();
}

bool Type::is_pointer() const {
    return false;
}
        
bool Type::is_custom_type() const {
    return false;
}

bool Type::is_standard_type() const {
    return false;
}

bool Type::is_const() const {
    return false;
}

bool Type::is_template_type() const {
    return false;
}

unsigned int Type::size(Platform) const {
    cpp_unreachable("Not specialized type");
}

unsigned int Type::elements() const {
    cpp_unreachable("Not an array type");
}

bool Type::has_elements() const {
    cpp_unreachable("Not an array type");
}

std::string Type::type() const {
    cpp_unreachable("Not a custom type");
}

std::shared_ptr<const Type> Type::data_type() const {
    cpp_unreachable("No data type");
}

std::vector<std::shared_ptr<const Type>> Type::template_types() const {
    cpp_unreachable("No template types");
}

BaseType Type::base() const {
    cpp_unreachable("Not a standard type");
}

std::string Type::mangle() const {
    return ::mangle(shared_from_this());
}

bool eddic::operator==(std::shared_ptr<const Type> lhs, std::shared_ptr<const Type> rhs){
    if(lhs->is_array()){
        return rhs->is_array() && lhs->data_type() == rhs->data_type() ;//&& lhs->elements() == rhs->elements();
    }

    if(lhs->is_pointer()){
        return rhs->is_pointer() && lhs->data_type() == rhs->data_type();
    }

    if(lhs->is_custom_type()){
        return rhs->is_custom_type() && lhs->type() == rhs->type();
    }

    if(lhs->is_standard_type()){
        return rhs->is_standard_type() && lhs->base() == rhs->base();
    }

    if(lhs->is_template_type()){
        if(rhs->is_template_type() && lhs->type() == rhs->type()){
            return lhs->template_types() == rhs->template_types();
        }
    }

    return false;
}

bool eddic::operator!=(std::shared_ptr<const Type> lhs, std::shared_ptr<const Type> rhs){
    return !(lhs == rhs); 
}

/* Implementation of StandardType  */

StandardType::StandardType(BaseType type, bool const_) : base_type(type), const_(const_) {}

BaseType StandardType::base() const {
    return base_type;
}

bool StandardType::is_standard_type() const {
    return true;
}

bool StandardType::is_const() const {
    return const_;
}

unsigned int StandardType::size(Platform platform) const {
    auto descriptor = getPlatformDescriptor(platform);
    return descriptor->size_of(base());
}

/* Implementation of CustomType */

CustomType::CustomType(std::shared_ptr<GlobalContext> context, const std::string& type) : 
    context(context), m_type(type) {}

std::string CustomType::type() const {
    return m_type;
}

bool CustomType::is_custom_type() const {
    return true;
}

unsigned int CustomType::size(Platform) const {
    return context->total_size_of_struct(context->get_struct(shared_from_this()));
}
        
/* Implementation of ArrayType  */

ArrayType::ArrayType(std::shared_ptr<const Type> sub_type) : sub_type(sub_type) {}
ArrayType::ArrayType(std::shared_ptr<const Type> sub_type, int size) : sub_type(sub_type), m_elements(size) {}

unsigned int ArrayType::elements() const {
    assert(has_elements());

    return *m_elements;
}

bool ArrayType::has_elements() const {
    return static_cast<bool>(m_elements);
}

std::shared_ptr<const Type> ArrayType::data_type() const {
    return sub_type;
}

bool ArrayType::is_array() const {
    return true;
}

unsigned int ArrayType::size(Platform platform) const {
    if(has_elements()){
        return data_type()->size(platform) * elements() + INT->size(platform); 
    } else {
        return INT->size(platform); 
    }
}
        
/* Implementation of PointerType  */

PointerType::PointerType(std::shared_ptr<const Type> sub_type) : sub_type(sub_type) {} 

std::shared_ptr<const Type> PointerType::data_type() const {
    return sub_type;
}

bool PointerType::is_pointer() const {
    return true;
}

unsigned int PointerType::size(Platform platform) const {
    return INT->size(platform);
}
        
/* Implementation of TemplateType  */

TemplateType::TemplateType(std::shared_ptr<GlobalContext> context, std::string main_type, std::vector<std::shared_ptr<const Type>> sub_types) : 
    context(context), main_type(main_type), sub_types(sub_types) {}

std::string TemplateType::type() const {
    return main_type;
}

std::vector<std::shared_ptr<const Type>> TemplateType::template_types() const {
    return sub_types;
}

bool TemplateType::is_template_type() const {
    return true;
}

unsigned int TemplateType::size(Platform) const {
    return context->total_size_of_struct(context->get_struct(shared_from_this()));
}

/* Implementation of factories  */

std::shared_ptr<const Type> eddic::new_type(std::shared_ptr<GlobalContext> context, const std::string& type, bool const_){
    //Parse standard and custom types
    if(is_standard_type(type)){
        if(const_){
            if (type == "int") {
                return CINT;
            } else if (type == "char") {
                return CCHAR;
            } else if (type == "bool") {
                return CBOOL;
            } else if (type == "float"){
                return CFLOAT;
            } else if (type == "str"){
                return CSTRING;
            } else {
                return CVOID;
            }
        } else {
            if (type == "int") {
                return INT;
            } else if (type == "char") {
                return CHAR;
            } else if (type == "bool") {
                return BOOL;
            } else if (type == "float"){
                return FLOAT;
            } else if (type == "str"){
                return STRING;
            } else {
                return VOID;
            }
        }
    } else {
        cpp_assert(!const_, "Only standard type can be const");
        return std::make_shared<CustomType>(context, type);
    }
}

std::shared_ptr<const Type> eddic::new_array_type(std::shared_ptr<const Type> data_type){
    return std::make_shared<ArrayType>(data_type);
}

std::shared_ptr<const Type> eddic::new_array_type(std::shared_ptr<const Type> data_type, int size){
    return std::make_shared<ArrayType>(data_type, size);
}

std::shared_ptr<const Type> eddic::new_pointer_type(std::shared_ptr<const Type> data_type){
    return std::make_shared<PointerType>(data_type);
}

std::shared_ptr<const Type> eddic::new_template_type(std::shared_ptr<GlobalContext> context, std::string data_type, std::vector<std::shared_ptr<const Type>> template_types){
    return std::make_shared<TemplateType>(context, data_type, template_types);
}

bool eddic::is_standard_type(const std::string& type){
    return type == "int" || type == "char" || type == "void" || type == "str" || type == "bool" || type == "float";
}
