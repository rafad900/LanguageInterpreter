#ifndef RANGE_HPP
#define RANGE_HPP

#include <iostream> 
#include <vector>

class Range {
public:
	Range(int end);
	Range(int start, int end);
	Range(int start, int end, int increment);

	bool condition();
	void next();
	int current();

private:
	int _start, _end, _increment;
};

#endif