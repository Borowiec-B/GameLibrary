#include "GameLibrary/Event/Dispatcher.h"

using namespace GameLibrary::Event;


void Dispatcher::removeCallback(const Key key) {
	for (auto& [_, keysToCallbacks] : _callbacks)
		keysToCallbacks.erase(key);

	_idMgr.free(key);
}

void Dispatcher::removeCallbacks(const Id owner) {
	// Ignore non-existent owners.
	if (_ownershipMap.find(owner) == std::cend(_ownershipMap))
		return;

	for (const auto key : _ownershipMap.at(owner))
		removeCallback(key);

	_ownershipMap.erase(owner);
}

