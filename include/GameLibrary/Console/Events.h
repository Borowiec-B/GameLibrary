#pragma once

#include "GameLibrary/Console/Cvar.h"
#include "GameLibrary/Event/BaseEvent.h"


namespace GameLibrary::Console
{
	struct CvarValueChangedEvent : Event::BaseEvent {
		const Cvar& cvar;
	};
}

