#pragma once

#include <functional>
#include <variant>

#include "GameLibrary/Event/Traits.h"


namespace GameLibrary::Event
{
	/*
	 *  Callback: Wrapper for function considered a callback for event type E.
	 *			  Can be created from a variety of signatures - refer to type aliases.
	 */
	template<typename E, typename = IsEvent<E>>
	class Callback
	{
		using NoParamsCB = std::function<void()>;
		using EventParamCB = std::function<void(const E&)>;
	public:
		using CBVariant = std::variant<NoParamsCB, EventParamCB>;

		Callback(CBVariant func) : _cbVariant(std::move(func)) {
		}

		auto operator() (const E& event) {
			if (std::holds_alternative<NoParamsCB>(_cbVariant))
				std::get<NoParamsCB>(_cbVariant)();
			else if (std::holds_alternative<EventParamCB>(_cbVariant))
				std::get<EventParamCB>(_cbVariant)(event);
		}

	private:
		CBVariant _cbVariant;
	};
}

