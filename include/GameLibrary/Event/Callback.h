#pragma once

#include <functional>
#include <optional>
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
		using Predicate = std::function<bool(const E&)>;

		Callback(CBVariant func, std::optional<Predicate> pred = std::nullopt) : _cbVariant(std::move(func)), _predicate(pred) {}

		auto operator() (const E& event) {
			if (!eventPassesPredicate(event))
				return;

			if (std::holds_alternative<NoParamsCB>(_cbVariant))
				std::get<NoParamsCB>(_cbVariant)();
			else if (std::holds_alternative<EventParamCB>(_cbVariant))
				std::get<EventParamCB>(_cbVariant)(event);
		}

	private:
		bool eventPassesPredicate(const E& event) const {
			if (_predicate.has_value())
				return (*_predicate)(event);
			else
				return true;
		}

		CBVariant _cbVariant;
		std::optional<Predicate> _predicate;
	};
}

