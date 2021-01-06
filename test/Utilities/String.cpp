#include "GameLibrary/Utilities/String.h"

#include <string>

#include "catch2/catch.hpp"

using namespace GameLibrary::Utilities;


TEST_CASE("Surround works with std::string and std::wstring (but no mixed arguments).")
{
	SECTION("std::string")
	{
		const std::string s1 = "";
		const std::string s2 = "String";

		REQUIRE(surround(s1, "abc") == "abcabc");
		REQUIRE(surround(s2, "abc") == "abcStringabc");
		REQUIRE(surround(s2, '"') == "\"String\"");
	}

	SECTION("std::wstring")
	{
		const std::wstring w1 = L"";
		const std::wstring w2 = L"String";

		REQUIRE(surround(w1, L"abc") == L"abcabc");
		REQUIRE(surround(w2, L"abc") == L"abcStringabc");
		REQUIRE(surround(w2, '"') == L"\"String\"");
	}
}
