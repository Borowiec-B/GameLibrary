#include "GameLibrary/Event/Traits.h"

#include "catch2/catch.hpp"

#include "GameLibrary/Event/BaseEvent.h"

using namespace GameLibrary::Event;


TEST_CASE("IsEvent returns correct results.")
{
	struct NotDerived {};
	struct Derived : public BaseEvent {};
	struct DerivedAgain : public Derived {};

	REQUIRE(IsEvent<Derived>::value);
	REQUIRE(IsEventV<DerivedAgain>);

	REQUIRE_FALSE(IsEvent<NotDerived>::value);
	REQUIRE_FALSE(IsEventV<int>);
}

