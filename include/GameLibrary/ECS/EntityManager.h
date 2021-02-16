#pragma once

#include <map>
#include <memory>
#include <tuple>
#include <typeindex>

#include <boost/mp11.hpp>

#include "GameLibrary/ECS/Component.h"
#include "GameLibrary/Utilities/IdManager.h"


namespace GameLibrary::ECS
{
	class EntityManager
	{
		using ComponentPtr = std::unique_ptr<BaseComponent>;
	public:
		using Id = long long;

		template<typename E>
		Id addEntity() {
			const auto id = _idMgr.get();

			// Call addComponent<C>() for each type of E::ComponentsTuple.
			boost::mp11::tuple_for_each(typename E::ComponentsTuple(), [ this, id ] ( auto component ) { addComponent<decltype(component)>(id); });

			return id;
		}

		template<typename C>
		bool entityHasComponent(const Id id) const {
			const bool componentMapExists = (_components.find(typeid(C)) != std::cend(_components));

			if (!componentMapExists)
				return false;

			const auto& componentMap = _components.at(typeid(C));

			return (componentMap.find(id) != std::cend(componentMap));
		}

		bool entityExists(const Id id) const {
			for (const auto& [_, componentMap] : _components)
			{
				if (componentMap.find(id) != std::cend(componentMap))
					return true;
			}

			return false;
		}

		std::size_t getCount() const {
			std::set<Id> foundIds;

			for (const auto& [_, componentMap] : _components)
			{
				for (const auto& [key, _] : componentMap)
					foundIds.emplace(key);
			}

			return foundIds.size();
		}

		void removeEntity(const Id id) {
			for (auto& [_, idToComponentMap] : _components)
				idToComponentMap.erase(id);

			_idMgr.free(id);
		}

	public:
		template<typename C>
		void addComponent(const Id entity) {
			_components[typeid(C)].try_emplace(entity, std::make_unique<C>());
		}

		std::map<std::type_index, std::map<Id, ComponentPtr>> _components;
		Utilities::SequentialIdManager<Id> _idMgr{0, 1};
	};
}

