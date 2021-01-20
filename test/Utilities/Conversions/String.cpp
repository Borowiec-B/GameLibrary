#include "GameLibrary/Utilities/Conversions/String.h"

#include <cmath>
#include <cstdint>
#include <limits>
#include <vector>

#include "catch2/catch.hpp"

#include "GameLibrary/Exceptions/Conversions.h"

using namespace GameLibrary::Utilities::Conversions;


TEST_CASE("FloatPrecision sets and returns custom precision, or max precision; throws on invalid precision.")
{
	FloatPrecision one(1);

	REQUIRE_FALSE(one.isMax());
	REQUIRE(one.get<float>() == 1);

	FloatPrecision max = FloatPrecision::max();

	REQUIRE(max.isMax());
	REQUIRE(max.get<long double>() == std::numeric_limits<long double>::max_digits10);

	REQUIRE_THROWS_AS(FloatPrecision(-1), GameLibrary::Exceptions::InvalidArgument);
}

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

TEST_CASE("From/to String converters return expected values, operating on std::string/std::wstring and arithmetic types.")
{
	SECTION("To String")
	{
		SECTION("From Float")
		{
			REQUIRE(toString(10.123456789, FloatPrecision(6)) == "10.123457");
			REQUIRE(toString<std::wstring>(-10.123456789, FloatPrecision(6)) == L"-10.123457");

			// Require that FloatPrecision::max() guarantees to keep exact same value Float->String->Float.
			// std::numeric_limits<T>::min() is too problematic, and always seem to show up simply as zeros in string form.
			const std::vector<long double> extremeDoubles = { -M_PI, std::numeric_limits<long double>::max(), std::numeric_limits<long double>::lowest(),
															  0.0 };
			for (const auto& originalFloat : extremeDoubles) {
				const auto floatToString = toString(originalFloat, FloatPrecision::max());
				const auto floatToStringToFloat = std::stold(floatToString);
				REQUIRE(floatToStringToFloat == originalFloat);
			}

			REQUIRE(toString(std::numeric_limits<float>::infinity()) == "inf");
			REQUIRE(toString<std::wstring>(std::numeric_limits<double>::quiet_NaN()) == L"nan");
			REQUIRE(toString(std::numeric_limits<long double>::signaling_NaN()) == "nan");
		}

		SECTION("From Integer")
		{
			REQUIRE(toString(0) == "0");
			constexpr unsigned int max_uint32 = 4294967295;
			REQUIRE(toString(max_uint32) == "4294967295");
		}

		SECTION("From String")
		{
			REQUIRE(toString("string") == "string");
			REQUIRE(toString<std::wstring>("") == L"");
		}
	}

	SECTION("From String")
	{
		SECTION("To Float")
		{
			constexpr auto lowestFloat          = std::numeric_limits<float>::lowest();
			const auto     lowestFloatString    = toString(lowestFloat, FloatPrecision::max());
			constexpr auto maxLongDouble        = std::numeric_limits<long double>::max();
			const auto     maxLongDoubleWstring = toString<std::wstring>(maxLongDouble, FloatPrecision::max());

			REQUIRE(fromString<float>(lowestFloatString) == lowestFloat);
			REQUIRE(fromString<long double>(maxLongDoubleWstring) == maxLongDouble);
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
			REQUIRE(fromString<int>("123.99999") == 123);

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
