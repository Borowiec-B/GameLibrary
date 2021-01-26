#include "GameLibrary/Event/Dispatcher.h"

using namespace GameLibrary::Event;


void Dispatcher::removeCallback(const Key key) {
	for (auto& [_, keysToCallbacks] : _callbacks)
		keysToCallbacks.erase(key);

	_idMgr.free(key);
}
