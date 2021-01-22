#include "GameLibrary/Event/AnyCallback.h"

#include "catch2/catch.hpp"

#include "GameLibrary/Event/BaseEvent.h"

using namespace GameLibrary::Event;


struct TestEvent : BaseEvent {
	bool* indicator = nullptr;
};

TEST_CASE("AnyCallback wraps Callback<E> and mimics its interface.")
{
	bool parameterCallStatus = false;
	bool noParameterCallStatus = false;
	
	auto parameterCallback = AnyCallback::create<TestEvent>([] ( const TestEvent& e )
	{
		*(e.indicator) = true;
	});

	auto noParameterCallback = AnyCallback::create<TestEvent>([ &noParameterCallStatus ] {
		noParameterCallStatus = true;
	});

	TestEvent e;
	e.indicator = &parameterCallStatus;

	parameterCallback(e);
	noParameterCallback(e);

	REQUIRE(parameterCallStatus);
	REQUIRE(noParameterCallStatus);
}

