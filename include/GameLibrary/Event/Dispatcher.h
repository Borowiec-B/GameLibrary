#pragma once

#include <map>
#include <set>
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
		using Id = int;
		using Key = int;

		/*
		 *  addCallback(): Add supplied function to list of callbacks called when dispatching event of type E.
		 *  			   If predicate is supplied, callback will be called only if it passes with dispatched event.
		 *				   Callback's signature requirements are: no parameters, or E / const E / const E& parameter.
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
		addCallback(F&& func, std::optional<typename Callback<E>::Predicate> pred = std::nullopt) {
			Key key;
			try {
				key = _idMgr.get();
			} catch (const Exceptions::OverflowError&) {
				throw Exceptions::OverflowError("Event::Dispatcher::addCallback() failed: Key would overflow.");
			}
			
			AnyCallback callback = AnyCallback::create<E>(std::forward<F>(func), pred);

			const auto insertSuccess = _callbacks[typeid(E)].try_emplace(key, std::move(callback)).second;
			if (!insertSuccess)
			{
				_idMgr.free(key);
				throw Exceptions::CreationError("Event::Dispatcher::addCallback() failed: Insertion didn't take place.");
			}

			return key;
		}

		template<typename E, typename... Args>
		Key addOwnedCallback(Id owner, Args&&... addCallbackArgs) {
			const auto key = addCallback<E>(std::forward<Args>(addCallbackArgs)...);

			_ownershipMap[owner].emplace(key);

			return key;
		}

		/*
		 *  removeCallback(): Remove callback referred to by key, and allow key to be returned by future addCallback().
		 *					  Currently has no effect if key is not in use.
		 *
		 *					  NOTE: Don't use it to remove owned callbacks!!! It'll leave a dangling key. Use removeOwnedCallback().
		 */
		void removeCallback(const Key key);
		void removeOwnedCallback(const Id owner, const Key key);
		void removeCallbacks(const Id owner);

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
		std::map<Id, std::set<Key>> _ownershipMap;

		std::map<std::type_index, KeyToCallbackMap> _callbacks;
		Utilities::SequentialIdManager<Key>			_idMgr;
	};
};

