#pragma once

#include <any>

#include "GameLibrary/Event/Callback.h"


namespace GameLibrary::Event
{
	class AnyCallback
	{
	public:
		template<typename E, typename... Args>
		static AnyCallback create(Args&&... args) {
			Callback<E> callbackToWrap(std::forward<Args>(args)...);

			return AnyCallback(std::move(callbackToWrap));
		}

		template<typename E>
		auto operator() (const E& event) {
			auto& extractedCallback = std::any_cast<Callback<E>&>(_callback);

			return extractedCallback(event);
		}

	private:
		template<typename CB>
		AnyCallback(CB callback) : _callback(std::move(callback)) {}

		std::any _callback;
	};
}

