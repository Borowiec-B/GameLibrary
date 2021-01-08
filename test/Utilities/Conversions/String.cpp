#include "GameLibrary/Utilities/Conversions/String.h"

#include <cmath>
#include <limits>
#include <vector>

#include "catch2/catch.hpp"

using namespace GameLibrary::Utilities::Conversions;

TEST_CASE("stringstreamCast() passes flags and objects to a stream, and returns resulting std::string/std::wstring.")
{
	SECTION("Float to String")
	{
		REQUIRE(stringstreamCast(1.234, std::setprecision(5), std::fixed) ==  "1.23400");
		REQUIRE(stringstreamCast<std::wstring>(1.234, std::setprecision(5), std::fixed) ==  L"1.23400");
	}

	SECTION("Integer to String")
	{
		REQUIRE(stringstreamCast(0) == "0");
		REQUIRE(stringstreamCast<std::wstring>(1) == L"1");
	}

	SECTION("String to String")
	{
		REQUIRE(stringstreamCast("str") == "str");
		REQUIRE(stringstreamCast<std::wstring>(L"wstr") == L"wstr");
		REQUIRE(stringstreamCast<std::wstring>("str") == L"str");
	}
}

TEST_CASE("From/to String converters return expected values, operating on std::string/std::wstring.")
{
	SECTION("To String")
	{
		SECTION("From String")
		{
			REQUIRE(toString("string") == "string");
			REQUIRE(toString<std::wstring>("") == L"");
		}

		SECTION("From Integer")
		{
			REQUIRE(toString(0) == "0");
			constexpr unsigned int max_uint32 = 4294967295;
			REQUIRE(toString(max_uint32) == "4294967295");
		}

		SECTION("From Float")
		{
			REQUIRE(toString(0.123456789) == "0.123456789");
			REQUIRE(toString<std::wstring>(-4321.1234) == L"-4321.1234");
			REQUIRE(toString(10.123456789, 6) == "10.1235");
			REQUIRE(toString<std::wstring>(-10.123456789, 6) == L"-10.1235");

			const std::vector<long double> extremeDoubles = { -M_PI, std::numeric_limits<long double>::max(), std::numeric_limits<long double>::lowest(),
															  -std::numeric_limits<long double>::min(), 0.0 };

			// Require that FloatPrecisionPreset::Max guarantees to keep exact same value Float->String->Float.
			for (const auto& d : extremeDoubles) {
				const auto s = toString(d, FloatPrecisionPreset::Max);
				REQUIRE(std::stold(s) == d);
			}
		}
	}

		const std::vector<long double> doubles = { -M_PI, std::numeric_limits<long double>::max(), std::numeric_limits<long double>::lowest(),
											 	   -std::numeric_limits<long double>::min(), 0.0 };

		for (const auto& d : doubles) {
			const auto s = toString(d, FloatPrecisionPreset::Max);
			REQUIRE(std::stold(s) == d);
		}
	}
}
