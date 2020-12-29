#include "GameLibrary/Console/Cvar.h"

#include <cmath>
#include <sstream>
#include <string>

using namespace GameLibrary::Console;


std::string Cvar::getAsString() const {
	return _value->getAsString();
}

std::string Cvar::FloatValue::getAsString() const {
	std::stringstream stringConversionStream;

	stringConversionStream << std::setprecision(std::numeric_limits<decltype(_value)>::digits10) << _value;

	return stringConversionStream.str();
}

std::string Cvar::IntegerValue::getAsString() const {
	return std::to_string(_value);
}

std::string Cvar::StringValue::getAsString() const {
	return _value;
}

