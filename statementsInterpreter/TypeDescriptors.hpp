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

class IntDescriptor: public TypeDescriptor {
public:
    IntDescriptor(std::string variableName, int value);
    IntDescriptor(int value);
    std::string name();  // returns the name of the integer variable.
    int intValue();      // returns its value.
	virtual void print();
    virtual ~IntDescriptor();

private:
	std::string _name;
	int _value;
};

class DblDescriptor: public TypeDescriptor {
public:
	DblDescriptor(std::string variableName, double value);
	DblDescriptor(double value);
	std::string name();
	double doubleValue();
	virtual void print();
	virtual ~DblDescriptor();

private:
	std::string _name;
	double _value;
};

class StrDescriptor: public TypeDescriptor {
public:
	StrDescriptor(std::string variableName, std::string value);
	StrDescriptor(std::string value);
	std::string name();
	std::string stringValue();
	virtual void print();
	virtual ~StrDescriptor();

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
