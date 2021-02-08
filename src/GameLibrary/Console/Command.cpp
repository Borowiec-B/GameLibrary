#include "GameLibrary/Console/Command.h"

#include "GameLibrary/Utilities/String.h"


namespace GameLibrary::Console
{
	CommandInfo::CommandInfo(String name, std::size_t paramsCount, String description) : name(name), paramsCount(paramsCount), description(description) {}

	Command::Command(const String& stringToParse) {
		const auto nameDelimiters = Utilities::getNthWord(std::cbegin(stringToParse), std::cend(stringToParse), 0, Utilities::isWhitespace<String::value_type>);
		if (nameDelimiters.first == std::cend(stringToParse))
			throw Exceptions::InvalidArgument("Command::Command() failed: Empty string.");

		_name = String(nameDelimiters.first, nameDelimiters.second);

		// Arbitrary limit.
		const std::size_t maxArgs = 1000;
		_args = Utilities::split<String, std::vector>(nameDelimiters.second, std::cend(stringToParse), Utilities::isWhitespace<String::value_type>, maxArgs);
	}

	Command::Command(String name, std::vector<String> args) : _name(std::move(name)), _args(std::move(args)) {}

	const String& Command::getName() const {
		return _name;
	}

	const std::vector<String>& Command::getArgs() const {
		return _args;
	}
}

