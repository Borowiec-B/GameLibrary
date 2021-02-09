#include "GameLibrary/Console/Command.h"

#include <limits>

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

TEST_CASE("CommandInfo::countMatchesParamsCount() returns correct results.")
{
	const CommandInfo fiveParamsCmd("five_params", 5);
	const CommandInfo anyParamsCmd("any_params", CommandInfo::ParamsCount::Any);
	const CommandInfo oneParamCmd("one_param", CommandInfo::ParamsCount::ConcatenateArgs);

	REQUIRE(fiveParamsCmd.countMatchesParamsCount(5));
	REQUIRE_FALSE(fiveParamsCmd.countMatchesParamsCount(4));
	REQUIRE_FALSE(fiveParamsCmd.countMatchesParamsCount(6));

	REQUIRE(anyParamsCmd.countMatchesParamsCount(std::numeric_limits<std::size_t>::max()));
	REQUIRE(anyParamsCmd.countMatchesParamsCount(0));

	REQUIRE(oneParamCmd.countMatchesParamsCount(1));
	REQUIRE_FALSE(oneParamCmd.countMatchesParamsCount(0));
	REQUIRE_FALSE(oneParamCmd.countMatchesParamsCount(2));
}

