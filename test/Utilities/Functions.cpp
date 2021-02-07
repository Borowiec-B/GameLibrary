#include "GameLibrary/Utilities/Functions.h"

#include "catch2/catch.hpp"

using namespace GameLibrary;


TEST_CASE("bindMemberFunction() takes a method pointer, and returns a regular function object calling the method.")
{
	struct S {
		auto sumValueAndArguments(char x, short y, int z, long long w) {
			return value + x + y + z + w;
		}

		int value;
	};

	S s { 10 };
	
	auto boundMethod = Utilities::bindMemberFunction(&s, &S::sumValueAndArguments);
	
	REQUIRE(boundMethod(1, 2, 3, 4) == (s.value + 1 + 2 + 3 + 4));
}

