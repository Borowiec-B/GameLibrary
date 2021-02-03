#include "GameLibrary/Console/Console.h"

using namespace GameLibrary::Console;


ConsoleObject::ConsoleObject(Console& console, Id id) : _console(console), _id(id) {}

const Cvar& Console::getCvar(const String& name) {
	try {
		return _cvars.at(name);
	} catch (const std::out_of_range&) {
		throw Exceptions::NotFoundError(Utilities::compose("Console::getCvar() failed: Cvar \"", name, "\" not found."));
	}
}

void Console::removeCvarListener(const Event::Dispatcher::Key key) {
	_eventDispatcher.removeCallback(key);
}

void Console::removeObject(const Id id) {
	_idMgr.free(id);
	_objects.erase(id);
	_eventDispatcher.removeCallbacks(id);
}
