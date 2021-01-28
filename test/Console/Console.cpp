#include "GameLibrary/Console/Console.h"

#include "catch2/catch.hpp"

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

