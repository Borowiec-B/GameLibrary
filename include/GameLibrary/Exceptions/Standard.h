#pragma once

#include <stdexcept>


namespace GameLibrary::Exceptions
{
	class OverflowError : public std::overflow_error {
		using std::overflow_error::overflow_error;
	};

	class NotFoundError : public std::runtime_error {
		using std::runtime_error::runtime_error;
	};

	class InvalidArgument : public std::invalid_argument {
		using std::invalid_argument::invalid_argument;
	};
}
