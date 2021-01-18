#include "GameLibrary/Utilities/Traits.h"

#include "catch2/catch.hpp"

using namespace GameLibrary::Utilities;


TEMPLATE_TEST_CASE("IsString returns true for types implemented to be considered string.", "[traits]",
char*, const char*, std::string, const std::string, std::string&, const std::string&, std::string&&,
wchar_t*, const wchar_t*, std::wstring, std::wstring&, const std::wstring&, std::wstring&&)
{
	REQUIRE(IsStringV<TestType>);
}

TEMPLATE_TEST_CASE("IsString returns false for not-string types", "[traits]", int, struct Dummy*, char, wchar_t)
{
	REQUIRE_FALSE(IsStringV<TestType>);
}


