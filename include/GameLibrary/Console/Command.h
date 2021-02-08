#pragma once

#include <cstddef>
#include <vector>

#include "GameLibrary/Console/Types.h"


namespace GameLibrary::Console
{
	/*
	 *  CommandInfo: Struct describing Command's metadata and requirements.
	 *
	 *				 To be used by Console in a list storing all registered Commands.
	 */
	struct CommandInfo {
		CommandInfo(String name, std::size_t paramsCount, String description = "");

		String name;
		std::size_t paramsCount;
		String description;
	};

	class Command
	{
	public:
		Command(const String& stringToParse);
		Command(String name, std::vector<String> args);

		const String& getName() const;
		const std::vector<String>& getArgs() const;

	private:
		String				_name;
		std::vector<String> _args;
	};
}

