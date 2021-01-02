#include "GameLibrary/Console/Cvar.h"

#include "catch2/catch.hpp"

using namespace GameLibrary::Console;

/*
 *  To do:
 *    - Reorder sections to alphabetical order.
 *    - Write tests for invalid float arguments.
 */

TEST_CASE("Cvar sets and returns the correct value, free of unhandled errors in cases of non-ideal supplied types.")
{
	SECTION("Float Cvar")
	{
		SECTION("String arguments")
		{
			Cvar c(Cvar::ValueType::Float, "1.2340000000");
			REQUIRE(c.getAsString() == "1.234");

			c.setValue("-3.14");
			REQUIRE(c.getAsString() == "-3.14");

			c.setValue("-0");
			REQUIRE(c.getAsString() == "-0");
		}

		SECTION("Integer arguments")
		{
			Cvar c(Cvar::ValueType::Float, 1);
			REQUIRE(c.getAsString() == "1");

			c.setValue(-100);
			REQUIRE(c.getAsString() == "-100");

			// There's no negative zero integer, store it as plain 0.
			c.setValue(-0);
			REQUIRE(c.getAsString() == "0");
		}

		SECTION("Float arguments")
		{
			Cvar c(Cvar::ValueType::Float, -1.0);
			REQUIRE(c.getAsString() == "-1");

			c.setValue(-0.0);
			REQUIRE(c.getAsString() == "-0");
		}
	}

	SECTION("Integer Cvar - values should always be rounded.")
	{
		SECTION("String arguments")
		{
			Cvar c(Cvar::ValueType::Integer, "-1000");
			REQUIRE(c.getAsString() == "-1000");

			c.setValue("1.50000");
			REQUIRE(c.getAsString() == "2");

			c.setValue("-1.50000");
			REQUIRE(c.getAsString() == "-2");

			c.setValue("1.49999");
			REQUIRE(c.getAsString() == "1");

			c.setValue("invalid");
			REQUIRE(c.getAsString() == "1");
		}

		SECTION("Floating-point arguments")
		{
			Cvar c(Cvar::ValueType::Integer, -0.49999999);
			REQUIRE(c.getAsString() == "0");

			c.setValue(0.500);
			REQUIRE(c.getAsString() == "1");
		}

		SECTION("Integer arguments")
		{
			Cvar c(Cvar::ValueType::Integer, 1);
			REQUIRE(c.getAsString() == "1");

			c.setValue(-100000);
			REQUIRE(c.getAsString() == "-100000");
		}
	}

	SECTION("String Cvar")
	{
		SECTION("String arguments")
		{
			Cvar c(Cvar::ValueType::String, "initial_value");
			REQUIRE(c.getAsString() == "initial_value");

			c.setValue("new_correct_value");
			REQUIRE(c.getAsString() == "new_correct_value");

			c.setValue("");
			REQUIRE(c.getAsString() == "");
		}

		SECTION("Integer arguments")
		{
			Cvar c(Cvar::ValueType::String, 50);
			REQUIRE(c.getAsString() == "50");

			c.setValue(100);
			REQUIRE(c.getAsString() == "100");

			c.setValue(-200);
			REQUIRE(c.getAsString() == "-200");

			// There's no negative zero integer, store it as plain 0.
			c.setValue(-0);
			REQUIRE(c.getAsString() == "0");
		}

		SECTION("Floating-point arguments (trailing zeros are to be removed in String representation)")
		{
			Cvar c(Cvar::ValueType::String, 50.5000);
			REQUIRE(c.getAsString() == "50.5");

			c.setValue(3.141);
			REQUIRE(c.getAsString() == "3.141");

			c.setValue(-2.71830000001);
			REQUIRE(c.getAsString() == "-2.71830000001");

			c.setValue(-0.0);
			REQUIRE(c.getAsString() == "-0");
		}
	}
}

