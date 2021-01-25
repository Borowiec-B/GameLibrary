#include "GameLibrary/Event/Callback.h"

#include <vector>

#include "catch2/catch.hpp"

#include "GameLibrary/Event/BaseEvent.h"

using namespace GameLibrary::Event;


struct TestEvent : public BaseEvent {
	bool* callIndicator = nullptr;
};
static const char*	valid1(TestEvent e) { *(e.callIndicator) = true; return "success"; }
static float		valid2(const TestEvent& e) { *(e.callIndicator) = true; return 0; }
static auto			valid3 = [ ] ( const TestEvent& e ) { *(e.callIndicator) = true; return true; };

static bool			callIndicator = false;
static void			valid4() { callIndicator = true; }
static auto			valid5 = [ ] { callIndicator = true; };


TEST_CASE("Event Callback stores a function taking no parameters or an event, and calls it.")
{
	SECTION("Callback stores and calls functions taking an event.")
	{
		std::vector<Callback<TestEvent>> callbacks = { Callback<TestEvent>(valid1), Callback<TestEvent>(valid2), Callback<TestEvent>(valid3) };
		
		for (auto& callback : callbacks)
		{
			bool callbackWasCalled = false;

			TestEvent e;
			e.callIndicator = &callbackWasCalled;

			callback(e);

			REQUIRE(callbackWasCalled);
		}
	}

	SECTION("Callback stores and calls functions taking no parameters.")
	{
		std::vector<Callback<TestEvent>> callbacks = { Callback<TestEvent>(valid4), Callback<TestEvent>(valid5) };

		for (auto& callback : callbacks)
		{
			callIndicator = false;

			// Even if callback stores a no-parameters function, it takes an event.
			callback(TestEvent{});

			REQUIRE(callIndicator);
		}
	}
}

TEST_CASE("If Callback contains a predicate, call will happen only if it passes for supplied event.")
{
	struct BoolEvent : public BaseEvent {
		bool value;
	};
	auto pred = [ ] ( const BoolEvent& e ) { return e.value; };

	BoolEvent passing;
	passing.value = true;
	BoolEvent notPassing;
	notPassing.value = false;

	// valid4() sets callIndicator to true.
	Callback<BoolEvent> callIndicatorSetter(valid4, pred);

	callIndicator = false;
	callIndicatorSetter(passing);
	REQUIRE(callIndicator);

	callIndicator = false;
	callIndicatorSetter(notPassing);
	REQUIRE_FALSE(callIndicator);
}

