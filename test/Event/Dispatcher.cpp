#include "GameLibrary/Event/Dispatcher.h"

#include "catch2/catch.hpp"

#include <vector>

#include "GameLibrary/Event/BaseEvent.h"

using namespace GameLibrary::Event;


TEST_CASE("Dispatcher accepts any Event callbacks, and dispatches event to all registered callbacks. Supports owned callbacks.", "[event]")
{
	struct DummyEvent : BaseEvent {};
	struct TestEvent : BaseEvent {
		const int incrementValue = 1;
	};

	int callCount = 0;
	int ownedCallCount = 0;
	int eventCallCount = 0;

	// DummyEvent callback doesn't have any parameters. TestEvent callback takes a TestEvent.
	// Just to ensure valid callback signature has 0 parameters, or 1 (event).
	auto increaseCallCount = [ &callCount ] { callCount++; };
	auto increaseOwnedCallCount = [ &ownedCallCount ] { ownedCallCount++; };
	auto increaseEventCallCount = [ &eventCallCount ] ( const TestEvent& e ) {
		eventCallCount += e.incrementValue;
	};

	Dispatcher d;
	constexpr int callbacksCount = 5;

	// Add 5 callbacks calling the no-parameters incrementer (increasing callCount).
	for (int i = 0; i < callbacksCount; ++i)
		d.addCallback<DummyEvent>(increaseCallCount);
	// Add 5 callbacks calling the event-parameter incrementer (increasing eventCallCount).
	for (int i = 0; i < callbacksCount; ++i)
		d.addCallback<TestEvent>(increaseEventCallCount);
	// Add 5 callbacks calling the no-parameters incremented owned by id 1 (increasing ownedCallCount).
	for (int i = 0; i < callbacksCount; ++i)
		d.addOwnedCallback<DummyEvent>(1, increaseOwnedCallCount);

	// Dispatcher should have callbacksCount callbacks for DummyEvent and TestEvent registered.
	// This should call them all.
	d.dispatchEvent(DummyEvent{});
	d.dispatchEvent(TestEvent{});

	REQUIRE(callCount == callbacksCount);
	REQUIRE(eventCallCount == callbacksCount);
	REQUIRE(ownedCallCount == callbacksCount);
}

TEST_CASE("Dispatcher removes free-standing and owned callbacks.")
{
	struct DummyEvent : BaseEvent {};
	Dispatcher d;
	int callCount = 0;

	constexpr Dispatcher::Id twoCallbacksOwnerId = 1;
	constexpr Dispatcher::Id fourCallbacksOwnerId = 2;

	// Add 10 free-standing callCount's incrementers.
	std::vector<Dispatcher::Key> keys;
	for (int i = 0; i < 10; ++i)
		keys.emplace_back(d.addCallback<DummyEvent>([ &callCount ] { ++callCount; }));

	// Add two incrementers owned by id 1, and four incrementers owned by id 2. (total 16 incrementers after this)
	for (int i = 0; i < 2; ++i)
		d.addOwnedCallback<DummyEvent>(twoCallbacksOwnerId, [ &callCount ] { ++callCount; });
	for (int i = 0; i < 4; ++i)
		d.addOwnedCallback<DummyEvent>(fourCallbacksOwnerId, [ &callCount ] { ++callCount; });

	// Remove five free-standing incrementers, and two owned. (total 9 incrementers after this)
	for (int i = 0; i < 5; ++i)
		d.removeCallback(i);
	d.removeCallbacks(twoCallbacksOwnerId);
	
	// Removing callbacks owned by id already having 0 does nothing right now.
	d.removeCallbacks(twoCallbacksOwnerId);
	d.removeCallbacks(twoCallbacksOwnerId);

	d.dispatchEvent(DummyEvent{});

	REQUIRE(callCount == 9);
}

