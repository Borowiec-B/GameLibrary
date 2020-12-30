#pragma once

#include <stdexcept>


namespace GameLibrary::Exceptions
{
	class InvalidArgument : public std::invalid_argument
	{
		using std::invalid_argument::invalid_argument;
	};
}
