#include "GameLibrary/ECS/EntityManager.h"

#include <set>

#include "catch2/catch.hpp"

#include "GameLibrary/ECS/Component.h"
#include "GameLibrary/ECS/Entity.h"

using namespace GameLibrary::ECS;


TEST_CASE("EntityManager adds / removes entities, and reports their existence.", "[ECS]")
{
	struct DummyComponent : BaseComponent {};
	struct DummyEntity : BaseEntity<DummyComponent> {};

	EntityManager mgr;
	std::set<EntityManager::Id> usedIds;

	for (int i = 0; i < 4; ++i)
		usedIds.emplace(mgr.addEntity<DummyEntity>());

	REQUIRE(mgr.getCount() == 4);
	for (const auto id : usedIds)
		REQUIRE(mgr.entityExists(id));

	const auto removedId = *(std::cbegin(usedIds));
	mgr.removeEntity(removedId);

	REQUIRE(mgr.getCount() == 3);
	REQUIRE_FALSE(mgr.entityExists(removedId));
}

