#pragma once

#include <functional>
#include <map>
#include <memory>

#include "GameLibrary/Console/Command.h"
#include "GameLibrary/Console/Cvar.h"
#include "GameLibrary/Console/Events.h"
#include "GameLibrary/Console/Types.h"
#include "GameLibrary/Event/Dispatcher.h"
#include "GameLibrary/Utilities/Functions.h"
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
		
		/*
		 *  addMemberCvarListner(): Add member function to Console's list of callbacks called on Cvar change.
		 */
		template<typename T, typename R, typename... Params>
		Event::Dispatcher::Key addMemberCvarListener(String cvarName, R(T::*method)(Params...));

		template<typename T, typename R, typename... Params>
		Event::Dispatcher::Key addMemberCommandListener(String cmdName, R(T::*method)(Params...));

		/*
		 *  onCreation(): Console-centric constructor, will be called almost immediately after constructor if object is created through Console.
		 *
		 *  			  During all of regular construction, Object is not in Console's list of objects - leading to many exceptions.
		 *  			  Object is added to list immediately after constructor, then onCreation() is called.
		 */
		virtual void onCreation() {}

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

			_objects.at(id)->onCreation();

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

		/*
		 *  setCvar(): Set Cvar's value to newValue and call its listeners.
		 *
		 *  To do:
		 *    - Maybe don't call listeners if Cvar's value doesn't change.
		 *    - Handle Cvar::set() exceptions.
		 */
		template<typename T>
		void setCvar(const String& name, T&& newValue) {
			// To do: improve handling of invalid name.
			if (_cvars.find(name) == std::end(_cvars))
				return;

			Cvar& target = _cvars.at(name);

			target.set(std::forward<T>(newValue));
			_eventDispatcher.dispatchEvent(CvarValueChangedEvent{{}, target});
		}

		/*
		 *  getCvar(): Return const reference to Cvar.
		 *
		 *  Throws:
		 *    NotFoundError if Cvar doesn't exist.
		 */
		const Cvar& getCvar(const String& name);

		/*
		 *  addCvarListener(): Add callback to be called each time Cvar's setter is called.
		 */
		template<typename F>
		Event::Dispatcher::Key addCvarListener(String name, F&& callback) {
			auto cvarNameMatchesArgument = [ name(std::move(name)) ] ( const CvarValueChangedEvent& e ) { return e.cvar.getName() == name; };

			return _eventDispatcher.addCallback<CvarValueChangedEvent>(std::forward<F>(callback), std::move(cvarNameMatchesArgument));
		}

		void removeListener(const Event::Dispatcher::Key key);

		/*
		 *  addCommandListener(): Add callback to be called each time Command is executed.
		 *
		 *  					  Multi-word names are accepted, but are unlikely to ever be called.
		 *						  That's because input parsing is going to produce commands with name created from first word.
		 */
		template<typename F>
		Event::Dispatcher::Key addCommandListener(String name, F&& callback) {
			auto commandNameMatchesArgument = [ name(std::move(name)) ] ( const CommandSentEvent& e ) { return e.command.getName() == name; };

			return _eventDispatcher.addCallback<CommandSentEvent>(std::forward<F>(callback), std::move(commandNameMatchesArgument));
		}

		/*
		 *  addOwnedCvarListener(): Add callback to be called each time Cvar's setter is called, until object referenced by objectId is destroyed.
		 *  						objectId must be an existing ConsoleObject.
		 *
		 *  Throws:
		 *    NotFoundError if Console is not holding a ConsoleObject referenced by objectId.
		 */
		template<typename F>
		Event::Dispatcher::Key addOwnedCvarListener(const Id objectId, String cvarName, F&& callback) {
			if (_objects.find(objectId) == std::cend(_objects))
				throw Exceptions::NotFoundError(Utilities::compose("Console::addMemberCvarListener() failed: Non-existent object id: ", objectId, "."));

			auto cvarNameMatchesArgument = [ cvarName(std::move(cvarName)) ] ( const CvarValueChangedEvent& e ) { return e.cvar.getName() == cvarName; };

			return _eventDispatcher.addOwnedCallback<CvarValueChangedEvent>(objectId, std::forward<F>(callback), std::move(cvarNameMatchesArgument));
		}

		template<typename F>
		Event::Dispatcher::Key addOwnedCommandListener(const Id objectId, String cmdName, F&& callback) {
			if (_objects.find(objectId) == std::cend(_objects))
				throw Exceptions::NotFoundError(Utilities::compose("Console::addMemberCommandListener() failed: Non-existent object id: ", objectId, "."));

			auto cmdNameMatchesArgument = [ cmdName(std::move(cmdName)) ] ( const CommandSentEvent& e ) { return e.command.getName() == cmdName; };

			return _eventDispatcher.addOwnedCallback<CommandSentEvent>(objectId, std::forward<F>(callback), std::move(cmdNameMatchesArgument));
		}

		/*
		 *  removeObject(): If ConsoleObject referenced by id exists, destroy it and free its resources.
		 */
		void removeObject(const Id id);
		void dispatchCommand(Command cmd);

	private:
		std::map<String, Cvar>				_cvars;
		GameLibrary::Event::Dispatcher		_eventDispatcher;
		Utilities::SequentialIdManager<Id>	_idMgr{0, 1};
		std::map<Id, ObjectPtr>				_objects;
	};

	template<typename T, typename R, typename... Params>
	Event::Dispatcher::Key ConsoleObject::addMemberCvarListener(String cvarName, R(T::*method)(Params...)) {
		constexpr auto paramsCount = sizeof...(Params);

		static_assert(paramsCount == 0 || paramsCount == 1, "ConsoleObject::addMemberCvarListener() failed: Cvar listener must take 0 or 1 (event) argument.");

		return _console.addOwnedCvarListener(_id, std::move(cvarName), Utilities::bindMemberFunction(static_cast<T*>(this), method));
	}

//	template<typename T, typename R, typename... Params>
//	Event::Dispatcher::Key ConsoleObject::addMemberCommandListener(String cmdName, R(T::*method)(Params...)) {
//		constexpr auto paramsCount = sizeof...(Params);
//
//		static_assert(paramsCount == 0 || paramsCount == 1,
//				"ConsoleObject::addMemberCommandListener() failed: Command listener must take 0 or 1 (event) argument.");
//
//		return _console.addOwnedCommandListener(_id, std::move(cmdName), Utilities::bindMemberFunction(static_cast<T*>(this), method));
//	}
}

