#include "GameLibrary/Console/Command.h"

#include "catch2/catch.hpp"

#include "GameLibrary/Exceptions/Standard.h"

using namespace GameLibrary;
using namespace GameLibrary::Console;


TEST_CASE("Command initializes its name and arguments from string, throws if it's impossible.")
{
	Command c("exec arg1 \targ2      arg3 ");
	REQUIRE(c.getName() == "exec");
	REQUIRE((c.getArgs().size() == 3 && c.getArgs()[0] == "arg1" && c.getArgs()[1] == "arg2" && c.getArgs()[2] == "arg3"));

	Command noArgs("  write      \t\t\n");
	REQUIRE(noArgs.getName() == "write");
	REQUIRE(noArgs.getArgs().size() == 0);

	REQUIRE_THROWS_AS(Command("   \t\t\n "), Exceptions::InvalidArgument);
}

