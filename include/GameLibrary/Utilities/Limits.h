#pragma once

#include <limits>


namespace GameLibrary::Utilities
{
	template<typename T1, typename T2>
	constexpr bool additionWillOverflow(const T1 x, const T2 y) noexcept {
		if (y < 0)
		{
			if ( (std::numeric_limits<T1>::lowest() - y) > x )
			{
				return true;
			}
		}
		else
		{
			if ( (std::numeric_limits<T1>::max() - y) < x )
				return true;
		}

		return false;
	}
}

