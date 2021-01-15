#include "GameLibrary/Utilities/Conversions/String.h"

#include <cmath>
#include <cstdint>
#include <limits>
#include <vector>

#include "catch2/catch.hpp"

#include "GameLibrary/Exceptions/Conversions.h"

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
			// Default precision
			REQUIRE(toString(0.123456789) == "0.123456789");
			REQUIRE(toString<std::wstring>(-4321.1234) == L"-4321.1234");

			// Precision: 6
			REQUIRE(toString(10.123456789, 6) == "10.1235");
			REQUIRE(toString<std::wstring>(-10.123456789, 6) == L"-10.1235");

			// Require that FloatPrecisionPreset::Max guarantees to keep exact same value Float->String->Float.
			const std::vector<long double> extremeDoubles = { -M_PI, std::numeric_limits<long double>::max(), std::numeric_limits<long double>::lowest(),
															  -std::numeric_limits<long double>::min(), 0.0 };

			for (const auto& d : extremeDoubles) {
				const auto s = toString(d, FloatPrecisionPreset::Max);
				REQUIRE(std::stold(s) == d);
			}

			REQUIRE(toString(std::numeric_limits<float>::infinity()) == "inf");
			REQUIRE(toString<std::wstring>(std::numeric_limits<double>::quiet_NaN()) == L"nan");
			REQUIRE(toString(std::numeric_limits<long double>::signaling_NaN()) == "nan");
		}
	}

	SECTION("From String")
	{
		SECTION("To Float")
		{
			// To do: Remove the toString() dependency.
			constexpr auto lowestFloat = std::numeric_limits<float>::lowest();
			constexpr auto minDouble = std::numeric_limits<double>::min();
			constexpr auto maxLongDouble = std::numeric_limits<long double>::max();

			REQUIRE(fromString<float>(toString(lowestFloat, FloatPrecisionPreset::Max)) == lowestFloat);
			REQUIRE(fromString<double>(toString<std::wstring>(minDouble, FloatPrecisionPreset::Max)) == minDouble);
			REQUIRE(fromString<long double>(toString(maxLongDouble, FloatPrecisionPreset::Max)) == maxLongDouble);
			REQUIRE(fromString<long double>("100") == Approx(100));

			REQUIRE_THROWS_AS(fromString<float>(L"invalid"), GameLibrary::Exceptions::ConversionError);
			// Throw in out-of-range situations.
			REQUIRE_THROWS_AS(fromString<float>(std::string("9", 100)), GameLibrary::Exceptions::ConversionError);
			// Don't throw on imprecise conversions.
			REQUIRE_NOTHROW(fromString<float>("1.1234567891111213141516171819"));
		}

		SECTION("To Integer")
		{
			REQUIRE(fromString<std::uint32_t>("4294967295") == 4294967295);
			REQUIRE(fromString<std::int16_t>(L"-32768") == -32768);
			REQUIRE(fromString<int>("123.456") == 123);

			// Throw in out-of-range situations.
			REQUIRE_THROWS_AS(fromString<int>(std::wstring(L"9", 100)), GameLibrary::Exceptions::ConversionError);
			// Throw on floating-point only if, after truncation, it's out of range.
			REQUIRE_THROWS_AS(fromString<std::int16_t>("1000000.12346789"), GameLibrary::Exceptions::ConversionError);
			REQUIRE_NOTHROW(fromString<std::int16_t>(L"32767.9999999999"));
			REQUIRE_NOTHROW(fromString<std::int16_t>("-32768.9999999999"));
			REQUIRE_THROWS_AS(fromString<int>(L"1.2.3"), GameLibrary::Exceptions::ConversionError);
		}

		SECTION("To String")
		{
			// std::string -> std::wstring and std::wstring->std::string conversions aren't implemented.
			// However, const char* argument is converted to std::wstring just fine.
			REQUIRE(fromString<std::string>("str") == "str");
			REQUIRE(fromString<std::wstring>(L"str") == L"str");
			REQUIRE(fromString<std::wstring>("str") == L"str");
		}
	}
}
