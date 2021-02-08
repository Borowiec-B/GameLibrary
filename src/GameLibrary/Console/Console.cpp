#include "GameLibrary/Console/Console.h"

using namespace GameLibrary::Console;


ConsoleObject::ConsoleObject(Console& console, Id id) : _console(console), _id(id) {}

bool Console::cvarExists(const String& name) const {
	return _cvars.find(name) != std::cend(_cvars);
}

bool Console::commandInfoExists(const String& name) const {
	return _commandInfos.find(name) != std::cend(_commandInfos);
}

bool Console::commandMatchesRequirements(const Command& cmd) const {
	return (commandInfoExists(cmd.getName())) && (cmd.getArgs().size() == _commandInfos.at(cmd.getName()).paramsCount);
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
	if (commandMatchesRequirements(cmd))
	{
		const CommandSentEvent e{ {}, std::move(cmd) };

		_eventDispatcher.dispatchEvent(e);
	}
}

void Console::printCvar(const String& name) const {
	_out << "Cvar: \"" << name << "\" ";

	if (cvarExists(name))
		_out << "Value: \"" << _cvars.at(name).getAsString() << "\"\n";
	else
		_out << "doesn't exist.\n";
}

void Console::parse(const String& input) {
	const auto firstTokenDelimiters = Utilities::getNthWord(std::cbegin(input), std::cend(input), 0, Utilities::isWhitespace<String::value_type>);
	if (firstTokenDelimiters.first == std::cend(input))
		return;

	const String firstToken = Utilities::fromPair<String>(firstTokenDelimiters);
	const auto secondTokenDelimiters = Utilities::getNthWord(std::cbegin(input), std::cend(input), 1, Utilities::isWhitespace<String::value_type>);

	if (cvarExists(firstToken))
	{

		// Check if there are more tokens than just one. If yes, set cvar to [secondToken.begin, inputEnd]. If not, print Cvar.
		if (secondTokenDelimiters.first != std::cend(input))
		{
			const String secondTokenToEnd(secondTokenDelimiters.first, std::cend(input));
			setCvar(firstToken, secondTokenToEnd);
		}
		else
		{
			printCvar(firstToken);
		}
	}
	else if (commandInfoExists(firstToken))
	{
		// Arbitrary limit.
		constexpr auto maxArgs = 256;
		auto args = Utilities::split<String, std::vector>(secondTokenDelimiters.first, std::cend(input), Utilities::isWhitespace<String::value_type>, maxArgs);
		Command cmd(firstToken, std::move(args));

		dispatchCommand(std::move(cmd));
	}
	else
	{
		_out << "Unrecognized command: \"" << firstToken << "\"\n";
	}
}

