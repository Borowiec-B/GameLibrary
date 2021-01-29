#include "GameLibrary/Console/Console.h"

using namespace GameLibrary::Console;


ConsoleObject::ConsoleObject(Console& console, Id id) : _console(console), _id(id) {}

const Cvar& Console::getCvar(const std::string& name) {
	try {
		return _cvars.at(name);
	} catch (const std::out_of_range&) {
		throw Exceptions::NotFoundError(Utilities::compose("Console::getCvar() failed: Cvar \"", name, "\" not found."));
	}
}

void Console::removeObject(const Id id) {
	_idMgr.free(id);
	_objects.erase(id);
}

