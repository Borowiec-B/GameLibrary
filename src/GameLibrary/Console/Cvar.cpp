#include "GameLibrary/Console/Cvar.h"

#include <cmath>
#include <sstream>
#include <string>

#include "GameLibrary/Utilities/Conversions/String.h"

using namespace GameLibrary::Console;
using namespace GameLibrary::Utilities;


std::string Cvar::getAsString() const {
	return _value->getAsString();
}

std::string Cvar::FloatValue::getAsString() const {
	return Conversions::toString(_value, Conversions::FloatPrecisionPreset::Normal);
}

std::string Cvar::IntegerValue::getAsString() const {
	return Conversions::toString(_value);
}

std::string Cvar::StringValue::getAsString() const {
	return _value;
}

