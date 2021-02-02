#include "GameLibrary/Utilities/String.h"

#include <algorithm>
#include <string>

#include "catch2/catch.hpp"

using namespace GameLibrary::Utilities;
using Catch::Matchers::StartsWith;


TEST_CASE("Compose() creates a string from arguments.", "[string][utilities]")
{
	REQUIRE_THAT(compose<std::string>(100, " hundred ", 100.123), StartsWith("100 hundred 100.123"));
	REQUIRE(compose<std::wstring>(L"beg ", 0, std::wstring(L" end")) == L"beg 0 end");
}

TEST_CASE("Surround() works with std::string, std::wstring, and their literals.", "[string][utilities]")
{
	SECTION("std::string")
	{
		const std::string s1 = "";
		const std::string s2 = "String";

		REQUIRE(surround(s1, "abc") == "abcabc");
		REQUIRE(surround(s2, "abc") == "abcStringabc");
		REQUIRE(surround(s2, '"') == "\"String\"");
		REQUIRE(surround("a", s2) == "StringaString");
		REQUIRE(surround("a", "b") == "bab");
	}

	SECTION("std::wstring")
	{
		const std::wstring w1 = L"";
		const std::wstring w2 = L"String";

		REQUIRE(surround(w1, L"abc") == L"abcabc");
		REQUIRE(surround(w2, L"abc") == L"abcStringabc");
		REQUIRE(surround(w2, L'"') == L"\"String\"");
		REQUIRE(surround(L"a", w2) == L"StringaString");
		REQUIRE(surround(L"a", L"b") == L"bab");
	}
}

TEST_CASE("Quote() matches behavior of surround() with quotation mark argument.", "[string][utilities]")
{
	REQUIRE(quote("str") == surround("str", '"'));
	REQUIRE(quote(L"str") == surround(L"str", L'"'));
	REQUIRE(quote("") == surround("", '"'));
	REQUIRE(quote(L"") == surround(L"", L'"'));
}

TEST_CASE("isWhitespace() works for char and wchar_t.", "[string][utilities]")
{
	const std::string noWhitespace = "123_abcd_\0";
	const std::string onlyWhitespace = "\n\r\t\v   ";
	const std::wstring wideNoWhitespace = L"123_abcd_\0";
	const std::wstring wideOnlyWhitespace = L"\n\r\t\v   ";

	REQUIRE(std::none_of(std::begin(noWhitespace), std::end(noWhitespace), isWhitespace<char>));
	REQUIRE(std::all_of(std::begin(onlyWhitespace), std::end(onlyWhitespace), isWhitespace<char>));

	REQUIRE(std::none_of(std::begin(wideNoWhitespace), std::end(wideNoWhitespace), isWhitespace<wchar_t>));
	REQUIRE(std::all_of(std::begin(wideOnlyWhitespace), std::end(wideOnlyWhitespace), isWhitespace<wchar_t>));
}

