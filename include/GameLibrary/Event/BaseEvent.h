#pragma once


namespace GameLibrary::Event
{
	/*
	 *  BaseEvent: Type must inherit this to be considered Event.
	 */
	struct BaseEvent
	{
	protected:
		BaseEvent() = default;
	};
}
