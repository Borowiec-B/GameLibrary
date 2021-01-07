#include "GameLibrary/Utilities/String.h"

using namespace GameLibrary;


std::string Utilities::quote(const char* const str) {
	return surround<std::string>(str, '"');
}

std::wstring Utilities::quote(const wchar_t* const str) {
	return surround<std::wstring>(str, L'"');
}

