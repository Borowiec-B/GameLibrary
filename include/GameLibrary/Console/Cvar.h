#pragma once

#include <iomanip>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>


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
					// To do: Implement rest of the cases.
					if (auto p = dynamic_cast<StringValue*>(this))
						p->set(std::forward<T>(newValue));
				}

			    /*
			     *  getAsString: Return value's string representation regardless of its type.
			     */
				virtual std::string getAsString() const = 0;
			};

			/*
			 *  StringValue: Set of functions for interacting with a String Cvar value.
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
					if constexpr (std::is_arithmetic_v<T>)
					{
						std::stringstream stringConversionStream;

						// Don't keep unnecessary zeros / super-precise digits in the string.
						stringConversionStream << std::setprecision(std::numeric_limits<T>::digits10) << std::forward<T>(newValue);

						_value = std::move(stringConversionStream.str());
					}
					else if constexpr (std::is_convertible_v<decltype(newValue), std::string>)
					{
						_value = std::forward<T>(newValue);
					}
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

