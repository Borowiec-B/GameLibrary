#include "GameLibrary/Console/Cvar.h"

#include <string>

#include "GameLibrary/Utilities/Conversions/String.h"

using namespace GameLibrary::Console;
using namespace GameLibrary::Utilities;


String Cvar::getAsString() const {
	return _value.getAsString();
}

const String& Cvar::getName() const {
	return _name;
}

void Cvar::setDescription(String newDescription) {
	_description = std::move(newDescription);
}

const String& Cvar::getDescription() const {
	return _description;
}

