#pragma once

#include <cctype>
#include <cwctype>
#include <utility>

#include "GameLibrary/Utilities/Conversions/String.h"
#include "GameLibrary/Utilities/Conversions/StringToSstream.h"


namespace GameLibrary::Utilities
{
	template<typename S, typename... Args>
	S compose(Args&&... args) {
		auto stream = Conversions::stringToOstringstream<S>();
		
		(stream << ... << Conversions::toString<S>(std::forward<Args>(args)));

		return stream.str();
	}

	template<typename S1, typename S2>
	S1 surround(const S1& str, const S2& surrounder) {
		auto stream = Conversions::stringToOstringstream<S1>();
		
		stream << surrounder << str << surrounder;

		return stream.str();
	}

	template<typename S2>
	std::string surround(const char* const str, const S2& surrounder) {
		return surround<std::string, S2>(str, surrounder);
	}

	template<typename S2>
	std::wstring surround(const wchar_t* const str, const S2& surrounder) {
		return surround<std::wstring, S2>(str, surrounder);
	}

	template<typename S>
	S quote(const S& str) {
		return surround(str, '"');
	}

	template<typename C>
	bool isWhitespace(const C c) {
		if constexpr (std::is_same_v<std::decay_t<C>, wchar_t>)
			return std::iswspace(c);
		else
			return std::isspace(c);
	}

	/*
	 *  getNextWord(): Return iterator pair delimiting next word in relation to begin.
	 *
	 *				   First iterator points to first character of word.
	 *  			   Second iterator points to one position past word's last character.
	 */
	template<typename It>
	std::pair<It, It> getNextWord(const It begin, const It end,
								  std::function<bool(typename It::value_type)> delimiterPredicate = isWhitespace<typename It::value_type>) {
		It wordBegin = begin;

		// If begin already points at a word (not delimiter), skip to the end of that word.
		if (!delimiterPredicate(*wordBegin))
			wordBegin = std::find_if(wordBegin, end, delimiterPredicate);

		// wordBegin must be pointing at delimiter or end at this point.
		// Move it to next non-delimiter character, and wordEnd to next delimiter.
		wordBegin = std::find_if_not(wordBegin, end, delimiterPredicate);
		const It wordEnd = std::find_if(wordBegin, end, delimiterPredicate);

		return { wordBegin, wordEnd };
	}

	template<typename It>
	std::pair<It, It> getCurrentOrNextWord(const It begin, const It end,
										   std::function<bool(typename It::value_type)> delimiterPredicate = isWhitespace<typename It::value_type>)
	{
		// If begin is already pointing at a word, getNextWord() will skip over it and find the next one.
		// So only use it if begin is not pointing at one.
		if (delimiterPredicate(*begin))
			return getNextWord(begin, end, delimiterPredicate);
		
		// If execution is here, begin is pointing at a word.
		// So just find the end of this word.
		return { begin, std::find_if(begin, end, delimiterPredicate) };
	}

	std::string quote(const char* const str);
	std::wstring quote(const wchar_t* const str);
}

