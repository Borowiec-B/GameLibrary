#include "GameLibrary/Event/Dispatcher.h"

#include "catch2/catch.hpp"

#include "GameLibrary/Event/BaseEvent.h"

using namespace GameLibrary::Event;


TEST_CASE("Dispatcher accepts any Event callbacks, and dispatches event to all registered callbacks.", "[event]")
{
	struct DummyEvent : BaseEvent {};
	struct TestEvent : BaseEvent {
		const int incrementValue = 1;
	};

	int callCount = 0;
	int eventCallCount = 0;

	// DummyEvent callback doesn't have any parameters. TestEvent callback takes a TestEvent.
	// Just to ensure valid callback signature has 0 parameters, or 1 (event).
	auto increaseCallCount = [ &callCount ] { callCount++; };
	auto increaseEventCallCount = [ &eventCallCount ] ( const TestEvent& e ) {
		eventCallCount += e.incrementValue;
	};

	Dispatcher d;
	constexpr int callbacksCount = 5;

	for (int i = 0; i < callbacksCount; ++i)
		d.addCallback<DummyEvent>(increaseCallCount);
	for (int i = 0; i < callbacksCount; ++i)
		d.addCallback<TestEvent>(increaseEventCallCount);

	// Dispatcher should have callbacksCount callbacks for DummyEvent and TestEvent registered.
	// This should call them all.
	d.dispatchEvent(DummyEvent{});
	d.dispatchEvent(TestEvent{});

	REQUIRE(callCount == callbacksCount);
	REQUIRE(eventCallCount == callbacksCount);
}
