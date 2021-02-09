#include "GameLibrary/Console/Command.h"

#include "GameLibrary/Utilities/String.h"


namespace GameLibrary::Console
{
	CommandInfo::CommandInfo(String name, const ParamsCount paramsCount, String description)
								: name(std::move(name)), paramsCount(paramsCount), description(std::move(description)) {}

	CommandInfo::CommandInfo(String name, const std::size_t paramsCount, String description)
								: name(std::move(name)), paramsCount(paramsCount), description(std::move(description)) {}

	bool CommandInfo::countMatchesParamsCount(const std::size_t count) const {
		if (std::holds_alternative<std::size_t>(paramsCount))
		{
			return std::get<std::size_t>(paramsCount) == count;
		}
		else if (std::holds_alternative<ParamsCount>(paramsCount))
		{
			const auto specialParamsCount = std::get<ParamsCount>(paramsCount);
			
			if (specialParamsCount == ParamsCount::Any)
				return true;
			else if (specialParamsCount == ParamsCount::ConcatenateArgs)
				return (count == 1);
		}

		return false;
	}

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

