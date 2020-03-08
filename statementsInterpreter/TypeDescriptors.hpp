#include <string> 

class TypeDescriptor {
public:
	enum types {BOOL, DOUBLE, STRING, INTEGER};
	TypeDescriptor(types type): _type{type} {}
	types &type() { return _type; }
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
    //virtual ~TypeDescriptor() {}

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
	//virtual ~TypeDescriptor() {}

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
	//virtual ~TypeDescriptor() {}

private:
	std::string _name;
	std::string _value;
};

class BooleanTypeDescriptor: public TypeDescriptor {
public:
	BooleanTypeDescriptor(std::string variableName, bool value);
	BooleanTypeDescriptor(std::string variableName);
	std::string name();
	bool boolValue();
	//virtual ~TypeDescriptor() {}

private:
	std::string _name;
	bool _value;
};


