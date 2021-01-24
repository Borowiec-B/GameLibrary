#include "GameLibrary/Event/Dispatcher.h"

#include "catch2/catch.hpp"

#include <vector>

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

TEST_CASE("Dispatcher removes callbacks.")
{
	struct DummyEvent : BaseEvent {};
	Dispatcher d;
	int callCount = 0;

	// Add 10 callCount's incrementers, then remove 5 - so dispatchEvent() should increment callCount 5 times.
	std::vector<Dispatcher::Key> keys;
	for (int i = 0; i < 10; ++i)
		keys.emplace_back(d.addCallback<DummyEvent>([ &callCount ] { ++callCount; }));
	for (int i = 0; i < 5; ++i)
		d.removeCallback(i);

	d.dispatchEvent(DummyEvent{});
	REQUIRE(callCount == 5);
}

