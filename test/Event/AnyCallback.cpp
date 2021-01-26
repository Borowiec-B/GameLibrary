#include "GameLibrary/Event/AnyCallback.h"

#include "catch2/catch.hpp"

#include "GameLibrary/Event/BaseEvent.h"
#include "GameLibrary/Exceptions/Standard.h"

using namespace GameLibrary;
using namespace GameLibrary::Event;


TEST_CASE("AnyCallback wraps Callback<E> and mimics its interface.")
{
	struct TestEvent : BaseEvent {
		bool* indicator = nullptr;
	};

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

TEST_CASE("AnyCallback throws on invalid call arguments.")
{
	AnyCallback callback = AnyCallback::create<struct DummyEvent>([] {});
	
	struct InvalidEvent {};
	REQUIRE_THROWS_AS(callback(InvalidEvent{}), Exceptions::InvalidArgument);
}

