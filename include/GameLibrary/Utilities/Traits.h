#pragma once

#include <functional>
#include <string>
#include <type_traits>


namespace GameLibrary::Utilities
{
	template<typename T>
	struct IsString : public std::disjunction<
					   std::is_same<std::decay_t<T>, char*>,
					   std::is_same<std::decay_t<T>, const char*>,
					   std::is_same<std::decay_t<T>, wchar_t*>,
					   std::is_same<std::decay_t<T>, const wchar_t*>,
					   std::is_same<std::decay_t<T>, std::string>,
					   std::is_same<std::decay_t<T>, std::wstring>
	> {};
	template<typename T>
	inline constexpr bool IsStringV = IsString<T>::value;

	template<typename T>
	struct IsArithmeticOrString : public std::disjunction<std::is_arithmetic<std::decay_t<T>>, IsString<std::decay_t<T>>> {};
	template<typename T>
	constexpr inline bool IsArithmeticOrStringV = IsArithmeticOrString<T>::value;


	template<typename S>
	struct SignatureInfo;

	template<typename R, typename... Args>
	struct SignatureInfo<R(Args...)>
	{
		using ReturnType = R;
		static constexpr auto argsCount = sizeof...(Args);
	};

	template<typename T, typename R, typename... Args>
	struct SignatureInfo<R(T::*)(Args...)> : SignatureInfo<R(Args...)>
	{ };

	template<typename R, typename... Args>
	auto makeSignatureInfo(std::function<R(Args...)>) {
		return SignatureInfo<R(Args...)>();
	}

	template<typename F>
	auto makeSignatureInfo(F f) {
		return makeSignatureInfo(std::function(std::move(f)));
	}

	template<typename S>
	using ReturnTypeT = typename SignatureInfo<S>::ReturnType;
	template<typename S>
	constexpr inline auto argsCountV = SignatureInfo<S>::argsCount;
}

