#pragma once

#include <type_traits>

#include "GameLibrary/Event/BaseEvent.h"


namespace GameLibrary::Event
{
	template<typename T>
	struct IsEvent : std::is_base_of<BaseEvent, T> {};
	template<typename T>
	inline constexpr bool IsEventV = IsEvent<T>::value;
}

