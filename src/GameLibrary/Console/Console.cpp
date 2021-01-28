#include "GameLibrary/Console/Console.h"

using namespace GameLibrary::Console;


void Console::removeObject(const Id id) {
	_idMgr.free(id);
	_objects.erase(id);
}

ConsoleObject::ConsoleObject(Console& console, Id id) : _console(console), _id(id) {}

