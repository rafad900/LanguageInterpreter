#include "TypeDescriptors.hpp"
#include <iostream> 

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


/* Type Descriptor operations:
 * These provide the type checking and the functionality 
 * for every type and every operation allowed between the 
 * types according to the grammar rules. Another way to have 
 * done this was to use operator overloading, however, it 
 * probably would've been the same amount of code since the 
 * type checking and the operand functionality would've still
 * needed to be explicitly typed
 */

// Addition operations
std::string addition_of_string_and_string (std::string l, std::string r) {
    return l + r;
}

int addition_of_integer_and_integer(int l, int r) {
    return l + r;
}

double addition_of_integer_and_double(int i, double j) {
    return i + j;
}

double addition_of_double_and_double(double i, double j) {
    return i + j;
}

// Multiplication operations
std::string multiplication_of_string_and_integer(std::string s, int i) {
	std::string rs;
    for (int a = 0; a < i; a++) 
        rs += s;
    return rs;
}

int multiplication_of_integer_and_integer(int i, int j) {
    return i * j;
}

double multiplication_of_integer_and_double(double i, int j) {
    return i * j;
}

double multiplication_of_double_and_double(double i, double j) {
    return i * j;
}

// Subtraction operations
int subtraction_of_integer_and_integer(int i, int j) {
    return i - j;
}

double subtraction_of_integer_and_double(double i, int j, bool reverse) {
    if (reverse) return j - i;
    else return i - j;
}

double subtraction_of_double_and_double(double i, double j) {
    return i - j;
}

// Division operations
int division_of_integer_and_integer(int i, int j) {
    return i / j;
}

double division_of_integer_and_double(double i, int j, bool reverse) {
    if (reverse) return j / i;
    else return i / j;
}

double division_of_double_and_double(double i, double j) {
    return i / j;
}

// String comparison operations
bool string_comparison_string(std::string l, std::string r, int opcode) {
	if (opcode == 5) return l < r;
	else if (opcode == 6) 	return l > r;
	else if (opcode == 7)		return l == r;
	else if (opcode == 9)		return l >= r;
	else if (opcode == 8) 		return l <= r;
	else if (opcode == 10) 		return l != r;
}

bool string_comparison_integer(std::string l, int r,  int opcode) {
	if (opcode == 7) return false;
}

bool string_comparison_double(std::string l, int r,  double op) {
	if (opcode == 7) return false;
}

// Integer and double comparisons 
bool integer_comparison_integer(int l, int r,  int opcode) {
	if (opcode == 5) return l < r;
	else if (opcode == 6) 	return l > r;
	else if (opcode == 7)		return l == r;
	else if (opcode == 9)		return l >= r;
	else if (opcode == 8) 		return l <= r;
	else if (opcode == 10) 		return l != r;
}

bool double_comparison_double(int l, int r,  int opcode) {
	if (opcode == 5) return l < r;
	else if (opcode == 6) 	return l > r;
	else if (opcode == 7)		return l == r;
	else if (opcode == 9)		return l >= r;
	else if (opcode == 8) 		return l <= r;
	else if (opcode == 10) 		return l != r;
}

bool integer_comparison_double(int l, double r,  int opcode, int reverse) {
	// When the integer comes first keep the original operand, when the double comes first, use the opposite operand
	bool res;
	if (opcode == 5) res = l < r;
	else if (opcode == 6) 	res = l > r;
	else if (opcode == 7)		res = l == r;  // add the special syntax like belows
	else if (opcode == 9)		res = reverse ? l <= r : l >= r;
	else if (opcode == 8) 		res = reverse ? 1 >= r : l <= r;
	else if (opcode == 10) 		res = l != r;
	if ((opcode == 5 || opcode == 6) && reverse) res = res ? false: true;
}

TypeDescriptor* perform_operation( TypeDescriptor* lValue, TypeDescriptor* rValue, int opcode) {
	if ((lValue->type() == TypeDescriptor::STRING && rValue->type() == TypeDescriptor::STRING)) {		// STRING and STRING
		StringTypeDescriptor *left  = dynamic_cast<StringTypeDescriptor *>(lValue);
		StringTypeDescriptor *right = dynamic_cast<StringTypeDescriptor *>(rValue);
       	if (opcode == 1) return new StringTypeDescriptor(addition_of_string_and_string ( left->stringValue(), right->stringValue()));
		else if (opcode > 4) return new BooleanTypeDescriptor(string_comparison_string(left->stringValue(), right->stringValue(), opcode));
        else { std::cout << "Only operation supported for string and string is addition\n"; exit(2);}

	} else if ((lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::STRING)) {  	// INTEGER and STRING
		IntegerTypeDescriptor *left = dynamic_cast<IntegerTypeDescriptor *>(lValue);
		StringTypeDescriptor *right = dynamic_cast<StringTypeDescriptor *>(rValue);
        if (opcode == 3) return new StringTypeDescriptor(multiplication_of_string_and_integer(right->stringValue(), left->intValue()));
		else if (opcode == 7) return BooleanTypeDescriptor(false);
		else if (opcode == 10)return BooleanTypeDescriptor(true);
        else { std::cout << "Only operation supported for string and integer is multiplication\n"; exit(2);}

	} else if ((lValue->type() == TypeDescriptor::STRING && rValue->type() == TypeDescriptor::INTEGER)) { 	 // STRING and INTEGER
		StringTypeDescriptor *left   = dynamic_cast<StringTypeDescriptor *>(lValue);
		IntegerTypeDescriptor *right = dynamic_cast<IntegerTypeDescriptor *>(rValue);
        if (opcode == 3) return new StringTypeDescriptor(multiplication_of_string_and_integer(left->stringValue(), right->intValue()));
		else if (opcode == 7) return BooleanTypeDescriptor(false);
		else if (opcode == 10)return BooleanTypeDescriptor(true);
        else { std::cout << "Only operation supported for string and integer is multiplication\n"; exit(2);}

	} else if ((lValue->type() == TypeDescriptor::DOUBLE && rValue->type() == TypeDescriptor::STRING) || 
                (lValue->type() == TypeDescriptor::STRING && rValue->type() == TypeDescriptor::DOUBLE)) {  	// DOUBLE and STRING
       	std::cout << "No operation supported for string and double"; exit(2);

	} else if ((lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::INTEGER)) {	// INTEGER and INTEGER
   		IntegerTypeDescriptor *left  = dynamic_cast<IntegerTypeDescriptor *>(lValue);
   		IntegerTypeDescriptor *right = dynamic_cast<IntegerTypeDescriptor *>(rValue);
        if (opcode == 3) return new IntegerTypeDescriptor(multiplication_of_integer_and_integer(left->intValue(), right->intValue()));
        else if (opcode == 4) return new IntegerTypeDescriptor(division_of_integer_and_integer(left->intValue(), right->intValue()));
        else if (opcode == 2) return new IntegerTypeDescriptor(subtraction_of_integer_and_integer(left->intValue(), right->intValue()));
        else if (opcode == 1) return new IntegerTypeDescriptor(addition_of_integer_and_integer(left->intValue(), right->intValue()));
		else if (opcode > 4) return new BooleanTypeDescriptor(integer_comparison_integer(left->intValue(), right->intValue(), opcode));
        else { std::cout << "Operation: not supported for integer and integer"; exit(2); }

    } else if ((lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::DOUBLE)) {	// INTEGER and DOUBLE 
		IntegerTypeDescriptor *left = dynamic_cast<IntegerTypeDescriptor *>(lValue);
		DoubleTypeDescriptor *right = dynamic_cast<DoubleTypeDescriptor *>(rValue);
        if (opcode == 3) return new DoubleTypeDescriptor(multiplication_of_integer_and_double(left->intValue(), right->doubleValue()));
        else if (opcode == 4) return new DoubleTypeDescriptor(division_of_integer_and_double(left->intValue(), right->doubleValue(), false));
        else if (opcode == 2) return new DoubleTypeDescriptor(subtraction_of_integer_and_double(left->intValue(), right->doubleValue(), false));
        else if (opcode == 1) return new DoubleTypeDescriptor(addition_of_integer_and_double(left->intValue(), right->doubleValue()));
        else { std::cout << "Operation: not supported for integer and double"; exit(2); }

    } else if ((lValue->type() == TypeDescriptor::DOUBLE && rValue->type() == TypeDescriptor::INTEGER)) {	// DOUBLE and INTEGER
		DoubleTypeDescriptor *left   = dynamic_cast<DoubleTypeDescriptor *>(lValue);
		IntegerTypeDescriptor *right = dynamic_cast<IntegerTypeDescriptor *>(rValue);
       	if (opcode == 3) return new DoubleTypeDescriptor(multiplication_of_integer_and_double(left->doubleValue(), right->intValue()));
        else if (opcode == 4) return new DoubleTypeDescriptor(division_of_integer_and_double(left->doubleValue(), right->intValue(), true));
        else if (opcode == 2) return new DoubleTypeDescriptor(subtraction_of_integer_and_double(left->doubleValue(), right->intValue(), true));
        else if (opcode == 1) return new DoubleTypeDescriptor(addition_of_integer_and_double(left->doubleValue(), right->intValue()));
		else if (opcode > 4) return new BooleanTypeDescriptor(integer_comparison_double(left->doubleValue(), right->doubleValue(), opcode));

        else { std::cout << "Operation: not supported for integer and double"; exit(2); }

    } else if ((lValue->type() == TypeDescriptor::DOUBLE && rValue->type() == TypeDescriptor::DOUBLE)) {	// DOUBLE and DOUBLE 
		DoubleTypeDescriptor *left  = dynamic_cast<DoubleTypeDescriptor *>(lValue);
		DoubleTypeDescriptor *right = dynamic_cast<DoubleTypeDescriptor *>(rValue);
       	if (opcode == 3) return new DoubleTypeDescriptor(multiplication_of_double_and_double(left->doubleValue(), right->doubleValue()));
        else if (opcode == 4) return new DoubleTypeDescriptor(division_of_double_and_double(left->doubleValue(), right->doubleValue()));
        else if (opcode == 2) return new DoubleTypeDescriptor(subtraction_of_double_and_double(left->doubleValue(), right->doubleValue()));
        else if (opcode == 1) return new DoubleTypeDescriptor(addition_of_double_and_double(left->doubleValue(), right->doubleValue()));
		else if (opcode > 4) return new BooleanTypeDescriptor(double_comparison_double(left->doubleValue(), right->doubleValue(), opcode));
        else { std::cout << "Operation: not supported for double and double"; exit(2); }
	}
}