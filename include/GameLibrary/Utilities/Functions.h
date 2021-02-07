#pragma once

#include <functional>


namespace GameLibrary::Utilities
{
	template<typename T, typename R, typename... Params>
	std::function<R(Params...)> bindMemberFunction(T* owner, R(T::*method)(Params...)) {
		return [ owner, method ] ( auto&&... args ) {
			return (owner->*method)(std::forward<decltype(args)>(args)...);
		};
	}
}
