#include "GameLibrary/Utilities/Limits.h"

#include <cstdint>

#include "catch2/catch.hpp"

using namespace GameLibrary::Utilities;


TEST_CASE("additionWillOverflow(x, y) returns true if (x + y) would over- or underflow x's type; false otherwise")
{
	const std::int8_t minusSixToUnderflow = std::numeric_limits<std::int8_t>::lowest() + 5;
	const unsigned short sixToOverflow = std::numeric_limits<unsigned short>::max() - 5;

	REQUIRE(additionWillOverflow(minusSixToUnderflow, -100));
	REQUIRE(additionWillOverflow(minusSixToUnderflow, -6));
	REQUIRE_FALSE(additionWillOverflow(minusSixToUnderflow, -5));
	REQUIRE_FALSE(additionWillOverflow(minusSixToUnderflow, 100));

	REQUIRE(additionWillOverflow(sixToOverflow, 6));
	REQUIRE(additionWillOverflow(sixToOverflow, 100));
	REQUIRE_FALSE(additionWillOverflow(sixToOverflow, 5));
	REQUIRE_FALSE(additionWillOverflow(sixToOverflow, -100));
}

