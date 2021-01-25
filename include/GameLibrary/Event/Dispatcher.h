#pragma once

#include <map>
#include <typeindex>

#include "GameLibrary/Event/AnyCallback.h"
#include "GameLibrary/Event/Traits.h"
#include "GameLibrary/Exceptions/Standard.h"
#include "GameLibrary/Utilities/IdManager.h"


namespace GameLibrary::Event
{
	class Dispatcher
	{
	public:
		using Key = int;

		template<typename E, typename F>
		std::enable_if_t<IsEventV<E>, Key>
		addCallback(F&& func) {
			Key key;
			try {
				key = _idMgr.get();
			} catch (const Exceptions::OverflowError&) {
				throw Exceptions::OverflowError("Event::Dispatcher::addCallback() failed: Key would overflow.");
			}
			
			AnyCallback callback = AnyCallback::create<E>(std::forward<F>(func));

			const auto insertSuccess = _callbacks[typeid(E)].try_emplace(key, std::move(callback)).second;
			if (!insertSuccess)
				throw Exceptions::CreationError("Event::Dispatcher::addCallback() failed: Insertion didn't take place.");

			return key;
		}

		void removeCallback(const Key key) {
			for (auto& [_, keysToCallbacks] : _callbacks)
				keysToCallbacks.erase(key);

			_idMgr.free(key);
		}

		template<typename E>
		std::enable_if_t<IsEventV<E>, void>
		dispatchEvent(const E& event) {
			const auto callbacksForEvent = _callbacks.find(typeid(E));

			// Just do nothing if there are no callbacks for E.
			if (callbacksForEvent != std::end(_callbacks))
			{
				for (auto& [key, callback] : callbacksForEvent->second)
					callback(event);
			}
		}
			
	private:
		using KeyToCallbackMap = std::map<Key, AnyCallback>;

		std::map<std::type_index, KeyToCallbackMap> _callbacks;
		Utilities::SequentialIdManager<Key>			_idMgr;
	};
};

