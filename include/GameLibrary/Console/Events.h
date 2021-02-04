#pragma once

#include "GameLibrary/Console/Command.h"
#include "GameLibrary/Console/Cvar.h"
#include "GameLibrary/Event/BaseEvent.h"


namespace GameLibrary::Console
{
	struct CvarValueChangedEvent : Event::BaseEvent {
		const Cvar& cvar;
	};

	struct CommandSentEvent : Event::BaseEvent {
		const Command& command;
	};
}

