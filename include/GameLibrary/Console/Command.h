#pragma once

#include <cstddef>
#include <variant>
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
		enum class ParamsCount {
			Any,
			ConcatenateArgs
		};
		CommandInfo(String name, const ParamsCount paramsCount, String description = "");
		CommandInfo(String name, const std::size_t paramsCount, String description = "");

		bool countMatchesParamsCount(const std::size_t count) const;

		String name;
		std::variant<ParamsCount, std::size_t> paramsCount;
		String description;
	};

	/*
	 *  To do:
	 *    - Maybe remove parsing constructor, because parsing is a currently a job done by Console.
	 */
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

