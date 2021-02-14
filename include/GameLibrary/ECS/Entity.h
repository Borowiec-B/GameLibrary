#pragma once

#include <tuple>


namespace GameLibrary::ECS
{
	template<typename... Components>
	struct BaseEntity {
		using ComponentsTuple = std::tuple<Components...>;
	};
}

