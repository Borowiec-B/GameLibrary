#pragma once

#include <stdexcept>


namespace GameLibrary::Exceptions
{
	class OverflowError : public std::overflow_error {
		using std::overflow_error::overflow_error;
	};
}
