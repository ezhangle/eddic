//=======================================================================
// Copyright Baptiste Wicht 2011.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include "Operators.hpp"
#include "Options.hpp"

#include <cassert>

using std::string;

//Constructors

BinaryOperator::BinaryOperator(Value* l, Value* r) : lhs(l), rhs(r) {
	addLast(l);
	addLast(r);
}

//Checks

void BinaryOperator::checkVariables(Variables& variables) throw (CompilerException){
	lhs->checkVariables(variables);
	rhs->checkVariables(variables);

	m_type = checkTypes(lhs->type(), rhs->type());
}

Type BinaryOperator::checkTypes(Type left, Type right) throw (CompilerException){
	if(left != right || left != INT){
		throw new CompilerException("Can only compute two integers");
	}

	return left;
}

Type Addition::checkTypes(Type left, Type right) throw (CompilerException){
	if(left != right){
		throw new CompilerException("Can only add two values of the same type");
	}

	return left;
}

//Writes

void Addition::write(ByteCodeFileWriter& writer){
	lhs->write(writer);
	rhs->write(writer);

	if(m_type == INT){
		writer.writeSimpleCall(IADD);
	} else {
		writer.writeSimpleCall(SADD);
	}
}

void Subtraction::write(ByteCodeFileWriter& writer){
	lhs->write(writer);
	rhs->write(writer);

	writer.writeSimpleCall(ISUB);
}

void Multiplication::write(ByteCodeFileWriter& writer){
	lhs->write(writer);
	rhs->write(writer);

	writer.writeSimpleCall(IMUL);
}

void Division::write(ByteCodeFileWriter& writer){
	lhs->write(writer);
	rhs->write(writer);

	writer.writeSimpleCall(IMUL);
}

void Modulo::write(ByteCodeFileWriter& writer){
	lhs->write(writer);
	rhs->write(writer);

	writer.writeSimpleCall(IMOD);
}

//Constantness

bool BinaryOperator::isConstant(){
	return lhs->isConstant() && rhs->isConstant();
}

//Values

int BinaryOperator::getIntValue(){
	if(type() != INT){
		throw "Invalid type";
	}

	if(!isConstant()){
		throw "Not a constant";
	}

	return compute(lhs->getIntValue(), rhs->getIntValue());
}

string BinaryOperator::getStringValue(){
	if(type() != STRING){
		throw "Invalid type"; 
	}

	if(!isConstant()){
		throw "Not a constant";
	}
	
	return compute(lhs->getStringValue(), rhs->getStringValue());
}

int BinaryOperator::compute(int left, int right){
	throw "Invalid type"; 
}

string BinaryOperator::compute(string left, string right){
	throw "Invalid type"; 
}

int Addition::compute(int left, int right){
	return left + right;
}

string Addition::compute(string left, string right){
	return left + right;
}

int Subtraction::compute(int left, int right){
	return left - right;
}

int Multiplication::compute(int left, int right){
	return left * right;
}

int Division::compute(int left, int right){
	return left / right;
}

int Modulo::compute(int left, int right){
	return left % right;
}

//Optimizations

void BinaryOperator::optimize(){
	if(isConstant()){
		if(Options::isSet(OPTIMIZE_INTEGERS) || Options::isSet(OPTIMIZE_ALL)){
			Value* value = new Integer(getIntValue());

			parent->replace(this, value);
		}
	}
	
	lhs->optimize();
	rhs->optimize();	
}

void Addition::optimize(){
	if(isConstant()){
		if(type() == INT){
			if(Options::isSet(OPTIMIZE_INTEGERS) || Options::isSet(OPTIMIZE_ALL)){
				Value* value = new Integer(getIntValue());

				parent->replace(this, value);
			}
		} else if(type() == STRING){
			if(Options::isSet(OPTIMIZE_STRINGS) || Options::isSet(OPTIMIZE_ALL)){
				//No optimization at this time
			}
		}
	}
	
	lhs->optimize();
	rhs->optimize();	
}
