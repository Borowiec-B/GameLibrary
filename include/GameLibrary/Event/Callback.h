#pragma once

#include <functional>

#include "GameLibrary/Event/Traits.h"


namespace GameLibrary::Event
{
	template<typename E, typename = IsEvent<E>>
	class Callback
	{
	public:
		using CB = std::function<void(const E&)>;

		Callback(CB func) : _func(std::move(func)) {
		}

		auto operator() (const E& event) {
			return _func(event);
		}

	private:
		CB _func;
	};
}

