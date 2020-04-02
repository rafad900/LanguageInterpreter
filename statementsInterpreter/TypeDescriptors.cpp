#include "TypeDescriptors.hpp"
#include <cmath>
#include <iostream> 

// Integer Type Descriptor
IntegerTypeDescriptor::IntegerTypeDescriptor(std::string variableName, int value)
:TypeDescriptor (TypeDescriptor::INTEGER) 
{
	_name = variableName;
	_value = value;
}

IntegerTypeDescriptor::IntegerTypeDescriptor(int value)
:TypeDescriptor (TypeDescriptor::INTEGER)
{
	_name = "\0";
	_value = value;
}

int IntegerTypeDescriptor::intValue() {
	return _value;
}

std::string IntegerTypeDescriptor::name() {
	return _name;
}

void IntegerTypeDescriptor::print() {
	std::cout << _value;
}

IntegerTypeDescriptor::~IntegerTypeDescriptor() {}

// Double Type Descriptor
DoubleTypeDescriptor::DoubleTypeDescriptor(std::string variableName, double value) 
:TypeDescriptor(TypeDescriptor::DOUBLE)
{
	_name = variableName; 
	_value = value;
}

DoubleTypeDescriptor::DoubleTypeDescriptor(double value) 
:TypeDescriptor(TypeDescriptor::DOUBLE)
{
	_name = "\0"; 
	_value = value;
}

double DoubleTypeDescriptor::doubleValue() {
	return _value;
}

std::string DoubleTypeDescriptor::name() {
	return _name;
}

void DoubleTypeDescriptor::print() {
	std::cout << _value;
}

DoubleTypeDescriptor::~DoubleTypeDescriptor() {}

// String Type Descriptor
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


StringTypeDescriptor::~StringTypeDescriptor() {}

void StringTypeDescriptor::print() {
	std::cout << _value;
}

/* 
 *
 * THESE DEFINITIONS ARE UNECESSARY AT THE MOMENT BUT MIGHT BE USEFUL LATER
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
} */


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
double division_of_integer_and_integer(int i, int j) {
    return  (double)i / (double)j;
}

double division_of_integer_and_double(int i, double j, bool reverse) {
    if (reverse) return j / i;
    else return i / j;
}

double division_of_double_and_double(double i, double j) {
    return i / j;
}

// Modulus operaions
int modulo_of_int_and_int(int i, int j) {
	return i % j;
}

double modulo_of_int_and_double(int i, double j, bool reversed) {
	double q = (double)i;
	double qp = 0.0;
	if (!reversed) {
		q = j;
		while (q <= i) {
			qp = q;
			q += j;
		}
		return i - qp;
	} 
	while ( q <= j) {
		qp = q;
		q += i;
	}
	return j - qp;
}

double modulo_of_double_and_double(double i, double j) {
	double q = j;
	double qp = 0.0;
	while ( q <= i ) {
		qp = q;
		q += j;
	}
	return i - qp;
}

// String comparison operations
int string_comparison_string(std::string l, std::string r, int opcode) {
	if (opcode == 5) return l < r;
	else if (opcode == 6) 	return l > r;
	else if (opcode == 7)		return l == r;
	else if (opcode == 9)		return l >= r;
	else if (opcode == 8) 		return l <= r;
	else if (opcode == 10) 		return l != r;
}

int string_comparison_integer(std::string l, int r,  int opcode) {
	if (opcode == 7) return false;
}

int string_comparison_double(std::string l, int r,  int opcode) {
	if (opcode == 7) return false;
}

// Integer and double comparisons 
int integer_comparison_integer(int l, int r,  int opcode) {
	if (opcode == 5) return l < r;
	else if (opcode == 6) 	return l > r;
	else if (opcode == 7)		return l == r;
	else if (opcode == 9)		return l >= r;
	else if (opcode == 8) 		return l <= r;
	else if (opcode == 10) 		return l != r;
}

int double_comparison_double(int l, int r,  int opcode) {
	if (opcode == 5) return l < r;
	else if (opcode == 6) 	return l > r;
	else if (opcode == 7)		return l == r;
	else if (opcode == 9)		return l >= r;
	else if (opcode == 8) 		return l <= r;
	else if (opcode == 10) 		return l != r;
}

int integer_comparison_double(int l, double r,  int opcode, int reverse) {
	// When the integer comes first keep the original operand, when the double comes first, use the opposite operand
	bool res = false;
	if (opcode == 5) res = reverse ? l < r : l > r;
	else if (opcode == 6) 	res = reverse ? l > r : l < r;
	else if (opcode == 7)		res = l == r; 
	else if (opcode == 9)		res = reverse ? l <= r : l >= r;
	else if (opcode == 8) 		res = reverse ? l >= r : l <= r;
	else if (opcode == 10) 		res = l != r;
	return res;
}

/* THIS PERFORMS THE EVALUATION OF THE INFIX EXPRESSION */
/* It uses an opcode to know which operation to peform on the values
 * 1. It first checks the type of the typedescriptor variable 
 * 2. After it confirms the types, it casts them to the correct child class
 * 3. Depending on the opcode, it performs the operation
 * 4. If the opcode is not an option, then it prints and error and exits
 */
TypeDescriptor* perform_operation( TypeDescriptor* lValue, TypeDescriptor* rValue, int opcode) {
	if ((lValue->type() == TypeDescriptor::STRING && rValue->type() == TypeDescriptor::STRING)) {		// STRING and STRING
		StringTypeDescriptor *left  = dynamic_cast<StringTypeDescriptor *>(lValue);
		StringTypeDescriptor *right = dynamic_cast<StringTypeDescriptor *>(rValue);
       	if (opcode == 1) return new StringTypeDescriptor(addition_of_string_and_string ( left->stringValue(), right->stringValue()));
		else if (opcode > 4) return new IntegerTypeDescriptor(string_comparison_string(left->stringValue(), right->stringValue(), opcode));
        else { std::cout << "Only operation supported for string and string is addition\n"; exit(2);}

	} else if ((lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::STRING)) {  	// INTEGER and STRING
		IntegerTypeDescriptor *left = dynamic_cast<IntegerTypeDescriptor *>(lValue);
		StringTypeDescriptor *right = dynamic_cast<StringTypeDescriptor *>(rValue);
        if (opcode == 3) return new StringTypeDescriptor(multiplication_of_string_and_integer(right->stringValue(), left->intValue()));
		else if (opcode == 7) return new IntegerTypeDescriptor(0);
		else if (opcode == 10)return new IntegerTypeDescriptor(1);
        else { std::cout << "Only operation supported for string and integer is multiplication\n"; exit(2);}

	} else if ((lValue->type() == TypeDescriptor::STRING && rValue->type() == TypeDescriptor::INTEGER)) { 	 // STRING and INTEGER
		StringTypeDescriptor *left   = dynamic_cast<StringTypeDescriptor *>(lValue);
		IntegerTypeDescriptor *right = dynamic_cast<IntegerTypeDescriptor *>(rValue);
        if (opcode == 3) return new StringTypeDescriptor(multiplication_of_string_and_integer(left->stringValue(), right->intValue()));
		else if (opcode == 7) return new IntegerTypeDescriptor(0);
		else if (opcode == 10)return new IntegerTypeDescriptor(1);
        else { std::cout << "Only operation supported for string and integer is multiplication\n"; exit(2);}

	} else if ((lValue->type() == TypeDescriptor::DOUBLE && rValue->type() == TypeDescriptor::STRING) || 
                (lValue->type() == TypeDescriptor::STRING && rValue->type() == TypeDescriptor::DOUBLE)) {  	// DOUBLE and STRING
       	std::cout << "No operation supported for string and double"; exit(2);

	} else if ((lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::INTEGER)) {	// INTEGER and INTEGER
   		IntegerTypeDescriptor *left  = dynamic_cast<IntegerTypeDescriptor *>(lValue);
   		IntegerTypeDescriptor *right = dynamic_cast<IntegerTypeDescriptor *>(rValue);
        if (opcode == 3) return new IntegerTypeDescriptor(multiplication_of_integer_and_integer(left->intValue(), right->intValue()));
        else if (opcode == 12) return new IntegerTypeDescriptor( floor(division_of_integer_and_integer(left->intValue(), right->intValue())));
        else if (opcode == 11) return new IntegerTypeDescriptor(modulo_of_int_and_int(left->intValue(), right->intValue()));
        else if (opcode == 4) return new DoubleTypeDescriptor(division_of_integer_and_integer(left->intValue(), right->intValue()));
        else if (opcode == 2) return new DoubleTypeDescriptor(subtraction_of_integer_and_integer(left->intValue(), right->intValue()));
        else if (opcode == 1) return new IntegerTypeDescriptor(addition_of_integer_and_integer(left->intValue(), right->intValue()));
		else if (opcode > 4) return new IntegerTypeDescriptor(integer_comparison_integer(left->intValue(), right->intValue(), opcode));
        else { std::cout << "Operation: not supported for integer and integer"; exit(2); }

    } else if ((lValue->type() == TypeDescriptor::INTEGER && rValue->type() == TypeDescriptor::DOUBLE)) {	// INTEGER and DOUBLE 
		IntegerTypeDescriptor *left = dynamic_cast<IntegerTypeDescriptor *>(lValue);
		DoubleTypeDescriptor *right = dynamic_cast<DoubleTypeDescriptor *>(rValue);
        if (opcode == 3) return new DoubleTypeDescriptor(multiplication_of_integer_and_double(left->intValue(), right->doubleValue()));
		else if (opcode == 11) return new DoubleTypeDescriptor(modulo_of_int_and_double(left->intValue(), right->doubleValue(), false));
		else if (opcode == 12) return new IntegerTypeDescriptor(division_of_integer_and_double(left->intValue(), right->doubleValue(), false));
        else if (opcode == 4) return new DoubleTypeDescriptor(division_of_integer_and_double(left->intValue(), right->doubleValue(), false));
        else if (opcode == 2) return new DoubleTypeDescriptor(subtraction_of_integer_and_double(left->intValue(), right->doubleValue(), false));
        else if (opcode == 1) return new DoubleTypeDescriptor(addition_of_integer_and_double(left->intValue(), right->doubleValue()));
 		else if (opcode > 4) return new IntegerTypeDescriptor(integer_comparison_double(left->intValue(), right->doubleValue(), opcode, false));
 		else { std::cout << "Operation: not supported for integer and double"; exit(2); }

    } else if ((lValue->type() == TypeDescriptor::DOUBLE && rValue->type() == TypeDescriptor::INTEGER)) {	// DOUBLE and INTEGER
		DoubleTypeDescriptor *left   = dynamic_cast<DoubleTypeDescriptor *>(lValue);
		IntegerTypeDescriptor *right = dynamic_cast<IntegerTypeDescriptor *>(rValue);
       	if (opcode == 3) return new DoubleTypeDescriptor(multiplication_of_integer_and_double(left->doubleValue(), right->intValue()));
		else if (opcode == 11) return new DoubleTypeDescriptor(modulo_of_int_and_double(left->doubleValue(), right->intValue(), true));
		else if (opcode == 12) return new IntegerTypeDescriptor(division_of_integer_and_double(right->intValue(), left->doubleValue(), true));
        else if (opcode == 4) return new DoubleTypeDescriptor(division_of_integer_and_double(left->doubleValue(), right->intValue(), true));
        else if (opcode == 2) return new DoubleTypeDescriptor(subtraction_of_integer_and_double(left->doubleValue(), right->intValue(), true));
        else if (opcode == 1) return new DoubleTypeDescriptor(addition_of_integer_and_double(left->doubleValue(), right->intValue()));
		else if (opcode > 4) return new IntegerTypeDescriptor(integer_comparison_double(left->doubleValue(), right->intValue(), opcode, true));

        else { std::cout << "Operation: not supported for integer and double"; exit(2); }

    } else if ((lValue->type() == TypeDescriptor::DOUBLE && rValue->type() == TypeDescriptor::DOUBLE)) {	// DOUBLE and DOUBLE 
		DoubleTypeDescriptor *left  = dynamic_cast<DoubleTypeDescriptor *>(lValue);
		DoubleTypeDescriptor *right = dynamic_cast<DoubleTypeDescriptor *>(rValue);
       	if (opcode == 3) return new DoubleTypeDescriptor(multiplication_of_double_and_double(left->doubleValue(), right->doubleValue()));
		else if (opcode == 11) return new DoubleTypeDescriptor(modulo_of_double_and_double(left->doubleValue(), right->doubleValue()));
		else if (opcode == 12) return new IntegerTypeDescriptor(division_of_double_and_double(left->doubleValue(), right->doubleValue()));
        else if (opcode == 4) return new DoubleTypeDescriptor(division_of_double_and_double(left->doubleValue(), right->doubleValue()));
        else if (opcode == 2) return new DoubleTypeDescriptor(subtraction_of_double_and_double(left->doubleValue(), right->doubleValue()));
        else if (opcode == 1) return new DoubleTypeDescriptor(addition_of_double_and_double(left->doubleValue(), right->doubleValue()));
		else if (opcode > 4) return new IntegerTypeDescriptor(double_comparison_double(left->doubleValue(), right->doubleValue(), opcode));
        else { std::cout << "Operation: not supported for double and double"; exit(2); }
	}
}
