#include "GameLibrary/Event/Callback.h"

#include <vector>

#include "catch2/catch.hpp"

#include "GameLibrary/Event/BaseEvent.h"

using namespace GameLibrary::Event;


struct TestEvent : public BaseEvent {
	bool* callIndicator = NULL;
};

static const char*	valid1(TestEvent e) { *(e.callIndicator) = true; return "success"; }
static float		valid2(const TestEvent& e) { *(e.callIndicator) = true; return 0; }
static auto			valid3 = [ ] ( const TestEvent& e ) { *(e.callIndicator) = true; return true; };

TEST_CASE("Event Callback stores functions taking an event parameter and calls them.")
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
