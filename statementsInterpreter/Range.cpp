#include "Range.hpp"
#include <vector>

Range::Range(int end) : _start{ 0 }, _end{ end }, _increment{ 1 } {}

Range::Range(int start, int end) : _start{ start }, _end{ end }, _increment{ 1 } {}

Range::Range(int start, int end, int increment) : _start{ start }, _end{ end }, _increment{ increment } {}

bool Range::condition() {
	if ((_start < _end) && _increment > 0)
		return true;
	else if ((_start > _end) && _increment < 0)
		return true;
	else return false;
}

void Range::next() {
	_start += _increment;
}

int Range::current() {
	return _start;
}