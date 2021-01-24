#include "GameLibrary/Utilities/IdManager.h"

#include <set>

#include "catch2/catch.hpp"

#include "GameLibrary/Exceptions/Standard.h"

using namespace GameLibrary;
using namespace GameLibrary::Utilities;


TEST_CASE("SequentialIdManager returns sequentially generated ids, and immediately reuses freed ids.", "[utilities]")
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

	SECTION("SequentialIdManager immediately reuses freed ids.")
	{
		SequentialIdManager<int> mgr;
		constexpr int numIds = 100;

		// Get some ids.
		std::set<int> usedIds;
		for (int i = 0; i < numIds; ++i)
			usedIds.emplace(mgr.get());

		// Free all those ids.
		for (const auto id : usedIds)
			mgr.free(id);

		// Get some ids again. Make sure they're the same ones.
		std::set<int> reusedIds;
		for (int i = 0; i < numIds; ++i)
			reusedIds.emplace(mgr.get());

		REQUIRE(usedIds == reusedIds);
	}
}

TEMPLATE_TEST_CASE("SequentialIdManager::get() throws only if it would cause its Id type to overflow.", "[utilities]", char, unsigned char, short, unsigned short)
{
	SequentialIdManager<TestType> overflowingMgr(0, 10);
	SequentialIdManager<TestType> underflowingMgr(0, -1);

	// Keep getting() until there are enough ids generated for (current_id + step) to over/undeflow.
	for (TestType i = 0; i < std::numeric_limits<TestType>::max()/10; ++i)
		overflowingMgr.get();
	for (TestType i = 0; i > std::numeric_limits<TestType>::lowest(); --i)
		underflowingMgr.get();

	// Managers' ids are currently close enough to their types' limits for get() to over/underflow.
	REQUIRE_THROWS_AS(overflowingMgr.get(), Exceptions::OverflowError);
	REQUIRE_THROWS_AS(underflowingMgr.get(), Exceptions::OverflowError);
}

