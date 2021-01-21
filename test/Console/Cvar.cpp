#include "GameLibrary/Console/Cvar.h"

#include <cstdint>
#include <limits>

#include "catch2/catch.hpp"

#include "GameLibrary/Exceptions/Conversions.h"

using namespace GameLibrary;
using namespace GameLibrary::Console;

using Catch::Matchers::StartsWith;


TEST_CASE("Cvar sets and returns the correct value. (Wstring and char-pointer getters not implemented yet.)")
{
	SECTION("Float Cvar")
	{
		Cvar c(Cvar::ValueType::Float);
		REQUIRE(c.getAs<long double>() == Approx(0));

		c.set(std::numeric_limits<std::int16_t>::min());

		REQUIRE_THAT(c.getAsString(), StartsWith("-32768"));
		REQUIRE(c.getAs<int>() == -32768);
		REQUIRE(c.getAs<long double>() == Approx(-32768));

		c.set(L"100.123");

		REQUIRE_THAT(c.getAsString(), StartsWith("100.123"));
		REQUIRE(c.getAs<char>() == 100);

		c.set(-1024.256);

		REQUIRE_THAT(c.getAs<std::string>(), StartsWith("-1024.256"));
		REQUIRE(c.getAs<int>() == -1024);
	}

	SECTION("Integer Cvar")
	{
		Cvar c(Cvar::ValueType::Integer);
		REQUIRE(c.getAs<int>() == 0);

		c.set(-1.999);

		REQUIRE(c.getAsString() == "-1");
		REQUIRE(c.getAs<char>() == -1);

		c.set("256");
		REQUIRE(c.getAs<int>() == 256);

		c.set(1024);
		REQUIRE(c.getAs<int>() == 1024);
	}

	SECTION("String Cvar")
	{
		Cvar c(Cvar::ValueType::String);
		REQUIRE(c.getAsString() == "");

		c.set(128.999);

		REQUIRE_THAT(c.getAsString(), StartsWith("128.999"));
		REQUIRE(c.getAs<int>() == 128);
		REQUIRE(c.getAs<double>() == Approx(128.999));

		c.set("String");
		REQUIRE(c.getAsString() == "String");

		c.set(std::numeric_limits<float>::infinity());
		REQUIRE(c.getAsString() == "inf");
		c.set(std::numeric_limits<float>::quiet_NaN());
		REQUIRE(c.getAsString() == "nan");
		c.set(std::numeric_limits<float>::signaling_NaN());
		REQUIRE(c.getAsString() == "nan");
	}

	SECTION("Invalid inputs throw ConversionError and keep the value untouched")
	{
		REQUIRE_THROWS_AS(Cvar(Cvar::ValueType::Float, std::numeric_limits<float>::quiet_NaN()), Exceptions::ConversionError);
		REQUIRE_THROWS_AS(Cvar(Cvar::ValueType::Integer, std::numeric_limits<double>::infinity()), Exceptions::ConversionError);

		Cvar floatCvar(Cvar::ValueType::Float, 0);

		REQUIRE_THROWS_AS(floatCvar.set(std::numeric_limits<long double>::quiet_NaN()), Exceptions::ConversionError);
		REQUIRE_THROWS_AS(floatCvar.set(std::numeric_limits<long double>::infinity()), Exceptions::ConversionError);
		REQUIRE(floatCvar.getAs<int>() == 0);

		Cvar integerCvar(Cvar::ValueType::Integer, -1024);

		REQUIRE_THROWS_AS(integerCvar.getAs<char>(), Exceptions::ConversionError);
		REQUIRE_THROWS_AS(integerCvar.getAs<std::uint32_t>(), Exceptions::ConversionError);
		REQUIRE_THROWS_AS(integerCvar.set(std::numeric_limits<long double>::infinity()), Exceptions::ConversionError);

		REQUIRE(integerCvar.getAs<int>() == -1024);

		Cvar stringCvar(Cvar::ValueType::String, "99999");

		REQUIRE_THROWS_AS(stringCvar.getAs<std::int8_t>(), Exceptions::ConversionError);
	}
}

