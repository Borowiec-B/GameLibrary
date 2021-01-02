#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <string>
#include <type_traits>

#include "GameLibrary/Utilities/Conversions/String.h"


namespace GameLibrary
{
	namespace Console
	{
		/*
		 * Cvar: GameLibrary's safe, interactive variable.
		 *
		 * Currently planned support for:
		 *   - Float, Integer, String types of Cvars.
		 *   - Callbacks.
		 *	 - Limits.
		 *
		 * To do:
		 *   - Consider changing my mind on throwing exceptions on unsuccessful value assign attempts (meaning, start throwing).
		 *   - Allow registering callbacks called on various events.
		 *   - Add flags (cheat-protection, etc.).
		 *   - Add limits.
		 *	 - Clean this file up (refactor functions and format multi-line sentences in comments).
		 */
		class Cvar
		{
		private:
			/*
			 *  Value: Generic, uninstantiable Cvar value. To be used as abstract base class.
			 * 		   Derived classes must implement all of its public interface.
			 *
			 *  Supposed to provide a generic interface, and forward arguments to appropriate overridden methods.
			 *
			 *  As a workaround for C++17 not featuring virtual function templates, template functions will
			 *  dynamically check which derived class 'this' is, and call the appropriate derived class's method.
			 *
			 *  To do: Consider moving constructors from derived classes to Value.
			 */
			class Value
			{
			public:
				virtual ~Value() = default;

				template<typename T>
				void set(T&& newValue) {
					if (auto p = dynamic_cast<FloatValue*>(this))
						p->set(std::forward<T>(newValue));
					else if (auto p = dynamic_cast<IntegerValue*>(this))
						p->set(std::forward<T>(newValue));
					else if (auto p = dynamic_cast<StringValue*>(this))
						p->set(std::forward<T>(newValue));
				}

			    /*
			     *  getAsString: Return value's string representation regardless of its type.
			     */
				virtual std::string getAsString() const = 0;
			};

			/*
			 *  FloatValue: Set of functions for interacting with a floating-point Cvar value.
			 */
			class FloatValue : public Value
			{
				using ValueType = long double;
			public:
				template<typename T>
				FloatValue(T&& initialValue) {
					set(std::forward<T>(initialValue));
				}

				template<typename T>
				void set(T&& newValue) {
					if constexpr (std::is_convertible_v<T, ValueType>)
					{
						_value = std::forward<T>(newValue);
					}
					// std::sto* functions can accept const std::string& and const std::wstring&.
					// Use std::stold (at least as long as ValueType is compatible) if supplied argument can be used with it.
					else if constexpr (std::is_convertible_v<decltype(std::forward<T>(newValue)), const std::string&> ||
									   std::is_convertible_v<decltype(std::forward<T>(newValue)), const std::wstring&>)
					{
						try
						{
							_value = std::stold(std::forward<T>(newValue));
						}
						catch (std::exception&)
						{
							// Unsuccessful setter calls aren't supposed to do anything special yet.
							// To do: Consider throwing exceptions after all.
						}
					}
				}

				std::string getAsString() const override;

			private:
				ValueType _value;
			};

			/*
			 *  IntegerValue: Set of functions for interacting with an integral Cvar value.
			 */
			class IntegerValue : public Value
			{
				using ValueType = long long;
			public:
				template<typename T>
				IntegerValue(T&& initialValue) {
					set(std::forward<T>(initialValue));
				}

				// set: Round the value and assign as integer.
				template<typename T>
				void set(T&& newValue) {
					if constexpr (std::is_integral_v<T>)
					{
						_value = std::forward<T>(newValue);
					}
					else if constexpr (std::is_floating_point_v<T>)
					{
						_value = std::llroundl(newValue);
					}
					// std::sto* functions can accept const std::string& and const std::wstring&.
					// Use std::stold (at least as long as ValueType is compatible) to convert string to an arithmetic type,
					// then round it up and assign.
					else if constexpr (std::is_convertible_v<decltype(std::forward<T>(newValue)), const std::string&> ||
									   std::is_convertible_v<decltype(std::forward<T>(newValue)), const std::wstring&>)
					{
						try
						{
							const long double unroundedNewValue = std::stold(std::forward<T>(newValue));
							_value = std::llroundl(unroundedNewValue);
						}
						catch (std::exception&)
						{
							// Unsuccessful setter calls aren't supposed to do anything special yet.
							// To do: Consider throwing exceptions after all.
						}
					}
				}

				std::string getAsString() const override;

			private:
				ValueType _value;
			};
			/*
			 *  StringValue: Set of functions for interacting with a String Cvar value.
			 *
			 *  To do:
			 *    - Move _value's type to ValueType.
			 */
			class StringValue : public Value
			{
			public:
				template<typename T>
				StringValue(T&& initialValue) {
					set(std::forward<T>(initialValue));
				}

				template<typename T>
				void set(T&& newValue) {
					_value = Utilities::Conversions::toString(std::forward<T>(newValue));
				}

				std::string getAsString() const override;

			private:
				std::string _value;
			};

		public:
			enum class ValueType {
				Float,
				Integer,
				String
			};

			template<typename T>
			Cvar(const ValueType valueType, T&& initialValue) {
				switch (valueType)
				{
					// To do: implement actions for the rest of the types.
					case ValueType::Float:
						_value = std::make_unique<FloatValue>(std::forward<T>(initialValue));
						break;
					case ValueType::Integer:
						_value = std::make_unique<IntegerValue>(std::forward<T>(initialValue));
						break;
					case ValueType::String:
						_value = std::make_unique<StringValue>(std::forward<T>(initialValue));
						break;
					default:
						break;
				}
			}

			/*
			 *  setValue: Pass newValue to *_value's set function.
			 *
			 *  It should then try to convert newValue to *_value's desired type and assign it as its new value.
			 *  This function should be callable with all types - *_value should set its fallback in case of incompatibility.
			 */
			template<typename T>
			void setValue(T&& newValue) {
				_value->set(std::forward<T>(newValue));
			}

			/*
			 *  getAsString: Return *_value's string representation regardless of its type.
			 */
			std::string	getAsString() const;

		private:
			std::unique_ptr<Value> _value;
		};
	}
}

