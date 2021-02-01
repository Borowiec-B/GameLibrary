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

namespace
{
	int noParams();
	void twoParams(int, float) {};
}
TEST_CASE("SingnatureInfo holds valid information; runtime helper function makeSignatureInfo() returns correct SignatureInfo.")
{
	struct S {
		char method(int);
	};

	int dummyCapture = 0;
	auto lambda = [ &dummyCapture ] ( std::string ) { return true; };

	// Free functions
	REQUIRE(argsCountV<decltype(noParams)> == 0);
	REQUIRE(std::is_same_v<ReturnTypeT<decltype(noParams)>, int>);

	REQUIRE(makeSignatureInfo(&twoParams).argsCount == 2);
	REQUIRE(std::is_same_v<decltype(makeSignatureInfo(&twoParams))::ReturnType, void>);

	// Member function
	REQUIRE(argsCountV<decltype(&S::method)> == 1);
	REQUIRE(std::is_same_v<ReturnTypeT<decltype(&S::method)>, char>);

	// Lambda
	REQUIRE(makeSignatureInfo(lambda).argsCount == 1);
	REQUIRE(std::is_same_v<decltype(makeSignatureInfo(lambda))::ReturnType, bool>);
}

