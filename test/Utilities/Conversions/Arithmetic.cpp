#include <catch2/catch.hpp>

#include "GameLibrary/Exceptions/Conversions.h"
#include "GameLibrary/Utilities/Conversions/Arithmetic.h"

using namespace GameLibrary;
using namespace GameLibrary::Utilities::Conversions;


TEST_CASE("safeArithmeticCast() returns expected results.")
{
	constexpr uint64_t int8Max = std::numeric_limits<int8_t>::max();
	constexpr int32_t int16Min = std::numeric_limits<int16_t>::min();

	REQUIRE(safeArithmeticCast<int16_t>(int16Min) == int16Min);
	REQUIRE(safeArithmeticCast<int8_t>(int8Max) == int8Max);
	REQUIRE(safeArithmeticCast<int>(128.999) == 128);

	REQUIRE_THROWS_AS(safeArithmeticCast<int8_t>(int16Min), Exceptions::ConversionError);
}

