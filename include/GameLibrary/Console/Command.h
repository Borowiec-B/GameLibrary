#pragma once

#include <vector>

#include "GameLibrary/Console/Types.h"


namespace GameLibrary::Console
{
	class Command
	{
	public:
		Command(const String& stringToParse);

		const String& getName() const;
		const std::vector<String>& getArgs() const;

	private:
		String				_name;
		std::vector<String> _args;
	};
}

