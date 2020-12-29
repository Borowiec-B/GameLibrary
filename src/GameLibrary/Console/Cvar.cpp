#include "GameLibrary/Console/Cvar.h"

#include <cmath>
#include <sstream>

using namespace GameLibrary::Console;


std::string Cvar::getAsString() const {
	return _value->getAsString();
}

std::string Cvar::FloatValue::getAsString() const {
	std::stringstream stringConversionStream;

	stringConversionStream << std::setprecision(std::numeric_limits<decltype(_value)>::digits10) << _value;

	return stringConversionStream.str();
}

std::string Cvar::StringValue::getAsString() const {
	return _value;
}

