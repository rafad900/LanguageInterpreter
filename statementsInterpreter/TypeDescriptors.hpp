#ifndef TYPEDESCRIPTOR_HPP
#define TYPEDESCRIPTOR_HPP

#include <string> 

class TypeDescriptor {
public:
	enum  types {BOOL, DOUBLE, STRING, INTEGER};
	TypeDescriptor(types type): _type{type} {}
	types &type() { return _type; }
	virtual void print() = 0;
	virtual ~TypeDescriptor() {}
	
private:
	types _type; 
};

class IntegerTypeDescriptor: public TypeDescriptor {
public:
    IntegerTypeDescriptor(std::string variableName, int value);
    IntegerTypeDescriptor(int value);
    std::string name();  // returns the name of the integer variable.
    int intValue();      // returns its value.
	virtual void print();
    virtual ~IntegerTypeDescriptor();

private:
	std::string _name;
	int _value;
};

class DoubleTypeDescriptor: public TypeDescriptor {
public:
	DoubleTypeDescriptor(std::string variableName, double value);
	DoubleTypeDescriptor(double value);
	std::string name();
	double doubleValue();
	virtual void print();
	virtual ~DoubleTypeDescriptor();

private:
	std::string _name;
	double _value;
};

class StringTypeDescriptor: public TypeDescriptor {
public:
	StringTypeDescriptor(std::string variableName, std::string value);
	StringTypeDescriptor(std::string value);
	std::string name();
	std::string stringValue();
	virtual void print();
	virtual ~StringTypeDescriptor();

private:
	std::string _name;
	std::string _value;
};

/*
 *
 *  THIS CLASS IS UNECESSARY AT THE MOMENT 
class BooleanTypeDescriptor: public TypeDescriptor {
public:
	BooleanTypeDescriptor(std::string variableName, bool value);
	BooleanTypeDescriptor(std::string variableName);
	std::string name();
	bool boolValue();
	virtual ~BooleanTypeDescriptor() {}

private:
	std::string _name;
	bool _value;
};
*/ 

#endif
