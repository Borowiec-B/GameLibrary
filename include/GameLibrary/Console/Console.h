#pragma once

#include <map>
#include <memory>

#include "GameLibrary/Console/Cvar.h"
#include "GameLibrary/Console/Events.h"
#include "GameLibrary/Console/Types.h"
#include "GameLibrary/Event/Dispatcher.h"
#include "GameLibrary/Utilities/IdManager.h"
#include "GameLibrary/Utilities/String.h"


namespace GameLibrary::Console
{
	using CvarCollection = std::map<String, Cvar>;
	using Id = int;

	/*
	 *  ConsoleObject: Base class for classes with tight Console integration (provides support for e.g. event handling, member cvar/cmd callbacks).
	 *				   Currently, interaction with Console is planned through inherited methods, rather than by direct use of _console.
	 *
	 * * * * * * *
	 *
	 *  Example of defining a derived class and adding it to a Console:
	 *
	 *    struct MyClass : public ConsoleObject {
	 *        D(Console& c, Id id, int i, int j) : ConsoleObject(c, id), _i(i), _j(j) {}
	 *        int _i; int _j;        
	 *    }
	 *
	 *    // When creating the object via Console::addObject(), first two (Console& and Id) are to be ommited in ctorArgs...
	 *    // Reason is, they'll be supplied by console.
	 *    console.addObject<MyClass>(10, 20);  
	 *
	 * * * * * * *
	 */
	class ConsoleObject
	{
		friend class Console;
	public:
		virtual ~ConsoleObject() = default;

	protected:
		ConsoleObject(class Console& console, Id id);

	private:
		Console& _console;
		const Id _id;
	};

	/*
	 *  Console: Manages Cvars and commands, manages and provides interaction between ConsoleObjects, and much more.
	 *			 Class which brings things together.
	 */
	class Console
	{
		using ObjectPtr = std::unique_ptr<ConsoleObject>;
	public:
		/*
		 *  addObject(): Create and store a ConsoleObject-deriving class.
		 *
		 *  Throws:
		 *    At this moment, all exceptions thrown in this method are to be handled by Console. 
		 */
		template<typename T, typename... Args>
		Id addObject(Args&&... ctorArgs) {
			const auto id = _idMgr.get();

			ObjectPtr obj = std::make_unique<T>(*this, id, std::forward<Args>(ctorArgs)...);

			_objects.try_emplace(id, std::move(obj));

			return id;
		}

		/*
		 *  initCvars(): Add all Cvars returned by T::getCvars() to list of Cvars.
		 *  			 T::getCvars() should return CvarCollection.
		 */
		template<typename T>
		void initCvars() {
			_cvars.merge(T::getCvars());
		}

		template<typename T1, typename T2, typename... Ts>
		void initCvars() {
			initCvars<T1>();
			initCvars<T2, Ts...>();
		}

		template<typename T>
		void setCvar(const String& name, T&& newValue) {
			// To do: improve handling of invalid name.
			if (_cvars.find(name) == std::end(_cvars))
				return;

			Cvar& target = _cvars.at(name);

			target.set(std::forward<T>(newValue));
			_eventDispatcher.dispatchEvent(CvarValueChangedEvent{{}, target});
		}

		const Cvar& getCvar(const String& name);

		template<typename F>
		Event::Dispatcher::Key addCvarListener(String name, F&& callback) {
			auto cvarNameMatchesArgument = [ name(std::move(name)) ] ( const CvarValueChangedEvent& e ) { return e.cvar.getName() == name; };

			return _eventDispatcher.addCallback<CvarValueChangedEvent>(std::forward<F>(callback), std::move(cvarNameMatchesArgument));
		}

		/*
		 *  removeObject(): If ConsoleObject referenced by id exists, destroy it and free its resources.
		 */
		void removeObject(const Id id);

	private:
		std::map<String, Cvar>				_cvars;
		GameLibrary::Event::Dispatcher		_eventDispatcher;
		Utilities::SequentialIdManager<Id>	_idMgr{0, 1};
		std::map<Id, ObjectPtr>				_objects;
	};
}

