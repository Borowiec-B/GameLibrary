#include "GameLibrary/Utilities/Conversions/Arithmetic.h"

#include <limits>

#include <catch2/catch.hpp>

#include "GameLibrary/Exceptions/Conversions.h"

using namespace GameLibrary;
using namespace GameLibrary::Utilities::Conversions;

template<typename T>
using nl = std::numeric_limits<T>;


TEST_CASE("safeArithmeticCast() returns expected results.", "[conversions][utilities]")
{
	constexpr uint64_t int8Max = nl<int8_t>::max();
	constexpr int32_t int16Min = nl<int16_t>::min();

	REQUIRE(safeArithmeticCast<int16_t>(int16Min) == int16Min);
	REQUIRE(safeArithmeticCast<int8_t>(int8Max) == int8Max);
	REQUIRE(safeArithmeticCast<int>(128.999) == 128);

	REQUIRE_THROWS_AS(safeArithmeticCast<int8_t>(int16Min), Exceptions::ConversionError);

	const auto invalidNumbers = { nl<long double>::quiet_NaN(), nl<long double>::signaling_NaN(), nl<long double>::infinity() };
	for (const auto invalid : invalidNumbers) {
		REQUIRE_THROWS_AS(safeArithmeticCast<long double>(invalid), Exceptions::ConversionError);
	}
}

