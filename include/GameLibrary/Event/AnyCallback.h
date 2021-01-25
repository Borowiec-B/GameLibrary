#pragma once

#include <any>

#include "GameLibrary/Event/Callback.h"
#include "GameLibrary/Exceptions/Standard.h"


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
			try
			{
				auto& extractedCallback = std::any_cast<Callback<E>&>(_callback);
				return extractedCallback(event);
			}
			catch (const std::bad_any_cast&)
			{
				throw Exceptions::InvalidArgument("Event::AnyCallback::() failed: Supplied event is incompatible with stored callback.");
			}

		}

	private:
		template<typename CB>
		AnyCallback(CB callback) : _callback(std::move(callback)) {}

		std::any _callback;
	};
}

