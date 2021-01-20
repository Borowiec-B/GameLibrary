#include "GameLibrary/Utilities/Conversions/ArithmeticString.h"

#include <limits>
#include <string>

#include "catch2/catch.hpp"

using namespace GameLibrary;
using namespace GameLibrary::Utilities::Conversions;


TEST_CASE("arithmeticOrStringCast() sticks to guidelines (calls correct casting functions).")
{
	REQUIRE(arithmeticOrStringCast<int>(1.9999) == safeArithmeticCast<int>(1.9999));
	REQUIRE(arithmeticOrStringCast<std::wstring>(std::numeric_limits<float>::infinity(), FloatPrecision(10)) ==
			toString<std::wstring>(std::numeric_limits<float>::infinity(), FloatPrecision::max()));

	REQUIRE(arithmeticOrStringCast<std::string>(12.345678, FloatPrecision(5)) == "12.34568");

	const auto toWStringLDMax = toString<std::wstring>(std::numeric_limits<long double>::max(), FloatPrecision::max());
	const auto fromWStringLDMax = fromString<long double>(toWStringLDMax);
	REQUIRE(arithmeticOrStringCast<long double>(toWStringLDMax) == Approx(fromWStringLDMax));

	REQUIRE_THROWS_AS(arithmeticOrStringCast<uint8_t>(std::numeric_limits<uint16_t>::max()), Exceptions::ConversionError);
	REQUIRE_THROWS_AS(arithmeticOrStringCast<int>(std::numeric_limits<float>::quiet_NaN()), Exceptions::ConversionError);
	REQUIRE_THROWS_AS(arithmeticOrStringCast<int>(std::wstring(L"1 invalid")), Exceptions::ConversionError);
}
