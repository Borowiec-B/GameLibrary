#include "GameLibrary/Utilities/IdManager.h"

#include <set>

#include "catch2/catch.hpp"

using namespace GameLibrary::Utilities;


TEST_CASE("SequentialIdManager returns sequentially generated ids, immediately reuses freed ids, and throws on overflows.")
{
	SECTION("Each get() SequentialIdManager returns previous id incremented by step.")
	{
		constexpr int incrementingStep = 5;
		constexpr int decrementingStep = -5;

		SequentialIdManager<std::int8_t> incrementingMgr(0, incrementingStep);
		SequentialIdManager<unsigned int> decrementingMgr(1000, decrementingStep);

		for (int i = 0; i < 10; ++i)
		{
			const auto id1 = incrementingMgr.get();
			const auto incrementedId1 = incrementingMgr.get();
			REQUIRE(incrementedId1 == id1 + incrementingStep);

			const auto id2 = decrementingMgr.get();
			const auto decrementedId2 = decrementingMgr.get();
			REQUIRE(decrementedId2 == id2 + decrementingStep);
		}
	}

