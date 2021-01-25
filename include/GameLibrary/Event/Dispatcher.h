#pragma once

#include <map>
#include <typeindex>

#include "GameLibrary/Event/AnyCallback.h"
#include "GameLibrary/Event/Traits.h"
#include "GameLibrary/Exceptions/Standard.h"
#include "GameLibrary/Utilities/IdManager.h"


namespace GameLibrary::Event
{
	/*
	 *  Dispatcher: Event callback manager.
	 *
	 *  Registers callbacks for given Event types, and on dispatch calls all callbacks with matching type.
	 *  Key returned by addCallback() is used to refer to callbacks, currently only used for unregistering.
	 */
	class Dispatcher
	{
	public:
		using Key = int;

		/*
		 *  addCallback(): Add supplied function to list of callbacks called when dispatching any event of type E.
		 *				   Function's signature requirements are: no parameters, or E / const E / const E& parameter.
		 *
		 *  Returns:
		 *    - Key, used to refer to added callback in functions taking a key.
		 *
		 *  Throws:
		 *    - CreationError if callback couldn't be added to list for whatever reason (shouldn't really happen).
		 *    - OverflowError if amount of registered callbacks would exceed upper limit of type Key.
		 */
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

		/*
		 *  removeCallback(): Remove callback referred to by key, and allow key to be returned by future addCallback().
		 *					  Currently has no effect if key is not in use.
		 */
		void removeCallback(const Key key) {
			for (auto& [_, keysToCallbacks] : _callbacks)
				keysToCallbacks.erase(key);

			_idMgr.free(key);
		}

		/*
		 *  dispatchEvent(): Call all callbacks registered for event type E, passing event as argument if signature allows it.
		 */
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

