#include "GameLibrary/Console/Console.h"

using namespace GameLibrary::Console;


ConsoleObject::ConsoleObject(Console& console, Id id) : _console(console), _id(id) {}

bool Console::cvarExists(const String& name) const {
	return _cvars.find(name) != std::cend(_cvars);
}

const Cvar& Console::getCvar(const String& name) {
	try {
		return _cvars.at(name);
	} catch (const std::out_of_range&) {
		throw Exceptions::NotFoundError(Utilities::compose("Console::getCvar() failed: Cvar \"", name, "\" not found."));
	}
}

void Console::removeListener(const Event::Dispatcher::Key key) {
	_eventDispatcher.removeCallback(key);
}

void Console::removeObject(const Id id) {
	_idMgr.free(id);
	_objects.erase(id);
	_eventDispatcher.removeCallbacks(id);
}

void Console::dispatchCommand(Command cmd) {
	const CommandSentEvent e{ {}, std::move(cmd) };

	_eventDispatcher.dispatchEvent(e);
}

