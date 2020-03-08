#include "TypeDescriptors.hpp"

IntegerTypeDescriptor::IntegerTypeDescriptor(std::string variableName, int value)
:TypeDescriptor (TypeDescriptor::INTEGER) 
{
	_name = variableName;
	_value = value;
}

IntegerTypeDescriptor::IntegerTypeDescriptor(int value)
:TypeDescriptor (TypeDescriptor::INTEGER)
{
	_name = false;
	_value = value;
}

int IntegerTypeDescriptor::intValue() {
	return _value;
}

std::string IntegerTypeDescriptor::name() {
	return _name;
}

DoubleTypeDescriptor::DoubleTypeDescriptor(std::string variableName, double value) 
:TypeDescriptor(TypeDescriptor::DOUBLE)
{
	_name = variableName; 
	_value = value;
}

DoubleTypeDescriptor::DoubleTypeDescriptor(double value) 
:TypeDescriptor(TypeDescriptor::DOUBLE)
{
	_name = false; 
	_value = value;
}

double DoubleTypeDescriptor::doubleValue() {
	return _value;
}

std::string DoubleTypeDescriptor::name() {
	return _name;
}

StringTypeDescriptor::StringTypeDescriptor(std::string variableName, std::string value) 
:TypeDescriptor(TypeDescriptor::STRING)
{
	_name = variableName; 
	_value = value;
}

StringTypeDescriptor::StringTypeDescriptor(std::string value) 
:TypeDescriptor(TypeDescriptor::STRING)
{
	_name = '\0'; 
	_value = value;
}

std::string StringTypeDescriptor::stringValue() {
	return _value;
}

std::string StringTypeDescriptor::name() {
	return _name;
}

BooleanTypeDescriptor::BooleanTypeDescriptor(std::string variableName, bool value) 
:TypeDescriptor(TypeDescriptor::BOOL)
{
	_name = variableName; 
	_value = value;
}

BooleanTypeDescriptor::BooleanTypeDescriptor(std::string variableName) 
:TypeDescriptor(TypeDescriptor::BOOL)
{
	_name = variableName; 
	_value = NULL;
}

bool BooleanTypeDescriptor::boolValue() {
	return _value;
}

std::string BooleanTypeDescriptor::name() {
	return _name;
}
