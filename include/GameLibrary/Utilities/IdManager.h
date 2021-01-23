#pragma once


namespace GameLibrary::Utilities
{
	template<typename Id = int>
	class IdManager
	{
	public:
		virtual ~IdManager() = default;

		virtual Id get() = 0;
		virtual void free(const Id) = 0;
	};
}
