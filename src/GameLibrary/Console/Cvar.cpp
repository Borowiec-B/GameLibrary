#include "GameLibrary/Console/Cvar.h"

#include <cmath>

using namespace GameLibrary::Console;


std::string Cvar::getAsString() const {
	return _value->getAsString();
}

std::string Cvar::StringValue::getAsString() const {
	return _value;
}

