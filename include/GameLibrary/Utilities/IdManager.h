#pragma once

#include <limits>
#include <set>
#include <stack>
#include <type_traits>

#include "GameLibrary/Exceptions/Standard.h"
#include "GameLibrary/Utilities/Limits.h"
#include "GameLibrary/Utilities/String.h"


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

	template<typename Id = int, typename Step = int>
	class SequentialIdManager : public IdManager<Id>
	{
	public:
		SequentialIdManager(const Id startingId, const Step step) : _nextId(startingId), _step(step) {}
		SequentialIdManager(const Id startingId) : SequentialIdManager(startingId, 1) {}
		SequentialIdManager() : SequentialIdManager(0) {}

		virtual Id get() override {
			if (freeIdAvailable())
				return reuseFreeId();
			else
				return generateNewId();
		}

		virtual void free(const Id id) override {
			// Do nothing if id is not in use.
			if (_usedIds.find(id) != std::end(_usedIds))
			{
				_usedIds.erase(id);
				_freedIds.push(id);
			}
		}

	private:
		bool freeIdAvailable() const noexcept(noexcept(_freedIds.empty())) {
			return !_freedIds.empty();
		}

		Id reuseFreeId() {
			if (_freedIds.empty())
				throw Exceptions::NotFoundError("SequentialIdManager::reuseFreeId() failed: No freed Ids available.");

			const auto ret = _freedIds.top();
			_freedIds.pop();
			return ret;
		}

		Id generateNewId() {
			if (additionWillOverflow(_nextId, _step))
			{
				auto message = compose<std::string>("SequentialIdManager::get() failed: Next id would over- or underflow. \
													(Next id would be: \"", _nextId, " + ", _step, "\")"); 
				throw Exceptions::OverflowError(std::move(message));
			}

			const auto ret = _nextId;

			_nextId += _step;
			_usedIds.emplace_hint(_usedIds.end(), ret);

			return ret;
		}

		Id			   _nextId;
		Step		   _step;

		std::stack<Id> _freedIds;
		std::set<Id>   _usedIds;
	};
}

