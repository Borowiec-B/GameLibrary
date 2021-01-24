#include "GameLibrary/Utilities/Traits.h"

#include "catch2/catch.hpp"

using namespace GameLibrary::Utilities;


TEMPLATE_TEST_CASE("IsString holds true for types implemented to be considered string.", "[traits][utilities]",
char*, const char*, std::string, const std::string, std::string&, const std::string&, std::string&&,
wchar_t*, const wchar_t*, std::wstring, std::wstring&, const std::wstring&, std::wstring&&)
{
	REQUIRE(IsString<TestType>::value);
	REQUIRE(IsStringV<TestType>);
}

TEMPLATE_TEST_CASE("IsString holds false for not-string types", "[traits][utilities]", int, struct Dummy*, char, wchar_t)
{
	REQUIRE_FALSE(IsString<TestType>::value);
	REQUIRE_FALSE(IsStringV<TestType>);
}

TEMPLATE_TEST_CASE("IsArithmeticOrString holds true for arithmetic or string types.", "[traits][utilities]",
char*, std::string, std::string&&, const wchar_t*, std::wstring, int&&, long double, char)
{
	REQUIRE(IsArithmeticOrString<TestType>::value);
	REQUIRE(IsArithmeticOrStringV<TestType>);
}

TEMPLATE_TEST_CASE("IsArithmeticOrString holds false for not-arithmetic-or-string types.", "[traits][utilities]",
std::string*, struct Dummy, int*)
{
	REQUIRE_FALSE(IsArithmeticOrString<TestType>::value);
	REQUIRE_FALSE(IsArithmeticOrStringV<TestType>);
}

