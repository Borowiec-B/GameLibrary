#include "GameLibrary/Console/Console.h"

#include "catch2/catch.hpp"

#include "GameLibrary/Exceptions/Standard.h"

using namespace GameLibrary::Exceptions;
using namespace GameLibrary::Console;


TEST_CASE("Console adds and removes ConsoleObject-deriving classes.")
{
	static int latestMyClassConstructionStatus = 0;
	static int latestMyClassDestructionStatus = 0;

	struct MyClass : public ConsoleObject {
		MyClass(Console& c, Id id, int constructionStatus, int destructionStatus) : ConsoleObject(c, id), _destructionStatus(destructionStatus) {
			latestMyClassConstructionStatus = constructionStatus;
		}
		~MyClass()  {
			latestMyClassDestructionStatus = _destructionStatus;
		}

		const int _destructionStatus;
	};

	Console c;

	const auto firstObjectId = c.addObject<MyClass>(1, 10);
	REQUIRE(latestMyClassConstructionStatus == 1);

	const auto secondObjectId = c.addObject<MyClass>(2, 20);
	REQUIRE(latestMyClassConstructionStatus == 2);

	const auto thirdObjectId = c.addObject<MyClass>(3, 30);
	REQUIRE(latestMyClassConstructionStatus == 3);


	c.removeObject(firstObjectId);
	REQUIRE(latestMyClassDestructionStatus == 10);

	c.removeObject(thirdObjectId);
	REQUIRE(latestMyClassDestructionStatus == 30);

	c.removeObject(secondObjectId);
	REQUIRE(latestMyClassDestructionStatus == 20);
}

TEST_CASE("Console initializes Cvars returned by T::getCvars(), and throws on invalid getCvar().")
{
	struct CvarHolder1 {
		static CvarCollection getCvars() {
			CvarCollection ret;

			ret.try_emplace("sv_cheats", "sv_cheats", Cvar::ValueType::Integer, 0);
			ret.try_emplace("volume", "volume", Cvar::ValueType::Float, 1.5);

			return ret;
		};
	};

	struct CvarHolder2 {
		static CvarCollection getCvars() {
			CvarCollection ret;

			ret.try_emplace("name", "name", Cvar::ValueType::String, "default_name");
			ret.try_emplace("group", "group", Cvar::ValueType::String, "default_group");

			return ret;
		}
	};

	Console c;
	c.initCvars<CvarHolder1, CvarHolder2>();

	REQUIRE(c.getCvar("sv_cheats").getAs<int>() == 0);
	REQUIRE(c.getCvar("volume").getAs<float>() == Approx(1.5));
	REQUIRE(c.getCvar("name").getAsString() == "default_name");
	REQUIRE(c.getCvar("group").getAsString() == "default_group");

	REQUIRE_THROWS_AS(c.getCvar("invalid").getAsString(), NotFoundError);
	REQUIRE_THROWS_AS(c.getCvar("invalid"), NotFoundError);
}

TEST_CASE("Console sets Cvar values.")
{
	struct CvarHolder {
		static CvarCollection getCvars() {
			CvarCollection ret;

			ret.try_emplace("sv_cheats", "sv_cheats", Cvar::ValueType::Integer);
			ret.try_emplace("volume", "volume", Cvar::ValueType::Float);
			ret.try_emplace("name", "name", Cvar::ValueType::String);

			return ret;
		}
	};

	Console c;
	c.initCvars<CvarHolder>();

	c.setCvar("sv_cheats", 10);
	c.setCvar("volume", 0.55);
	c.setCvar("name", "Player");

	REQUIRE(c.getCvar("sv_cheats").getAs<int>() == 10);
	REQUIRE(c.getCvar("volume").getAs<float>() == Approx(0.55));
	REQUIRE(c.getCvar("name").getAsString() == "Player");

	// Console should handle setCvar() exceptions by itself soon, this is just to confirm it's not implemented yet.
	REQUIRE_THROWS(c.setCvar("sv_cheats", "invalid"));

	// Setting non-existent Cvars should do nothing at the moment.
	REQUIRE_NOTHROW(c.setCvar("invalid", 100));
}

TEST_CASE("Console adds Cvar listeners and calls them on Cvar setter calls.")
{
	Console c;

	struct CvarHolder {
		static CvarCollection getCvars() {
			CvarCollection ret;

			ret.try_emplace("volume", "volume", Cvar::ValueType::Float, 0);
			ret.try_emplace("name", "name", Cvar::ValueType::String, "");

			return ret;
		};
	};

	c.initCvars<CvarHolder>();

	Float volumeAfterChange = 0;
	auto onVolumeChange = [ &volumeAfterChange ] ( const CvarValueChangedEvent& e ) {
		volumeAfterChange = e.cvar.getAs<Float>();
	};

	String nameAfterChange = "";
	auto onNameChange = [ &nameAfterChange ] ( const CvarValueChangedEvent& e ) {
		nameAfterChange = e.cvar.getAsString();
	};

	c.addCvarListener("volume", onVolumeChange);
	c.addCvarListener("name", onNameChange);

	c.setCvar("volume", 99.9);
	c.setCvar("name", "Player");

	REQUIRE(volumeAfterChange == Approx(99.9));
	REQUIRE(nameAfterChange == "Player");
}

