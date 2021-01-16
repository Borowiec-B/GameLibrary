#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <string>
#include <type_traits>

#include "GameLibrary/Utilities/Conversions/Arithmetic.h"
#include "GameLibrary/Utilities/Conversions/String.h"


namespace GameLibrary::Console
{
	/*
	 * Cvar: GameLibrary's interactive variable.
	 *
	 * Currently planned support for:
	 *   - Float, Integer, String types of Cvars.
	 *   - Callbacks.
	 *	 - Limits.
	 *
	 * To do:
	 *   - Allow registering callbacks called on various events.
	 *   - Add flags (cheat-protection, etc.).
	 *   - Add limits.
	 */
	class Cvar
	{
	public:
		enum class ValueType {
			Float,
			Integer,
			String
		};

		using Float = long double;
		using Integer = long long;
		using String = std::string;

	private:

		/*
		 *  Value: Cvar's class for managing a raw value.
		 * 		   Currently primary function is get/set functionality, with setter converting a wide range of types into ValueType.
		 */

		template<typename VT>
		class Value {
			using ValueType = VT;
		public:
			template<typename T>
			Value(T&& initialValue) {
				set(std::forward<T>(initialValue));
			}

			Value() {
				set(getDefault());
			}

			/*
			 *  set(): Convert newValue to ValueType and store.
			 *  	   Currently supports String and arithmetic arguments.
			 *
			 *  Throws:
			 *    - ConversionError if converting functions failed for whatever reason
			 *      (example causes: {NaN,infinity,alphabetical_string} for arithmetic Cvar; under/overflowing numbers for integral Cvar).
			 */
			template<typename T>
			void set(T&& newValue) {
				if constexpr (Utilities::IsStringV<T>)
					setFromString(std::forward<T>(newValue));
				else
					setFromArithmetic(newValue);
			}

			ValueType get() const {
				return _value;
			}

			String getAsString() const {
				return Utilities::Conversions::toString(_value);
			}

		private:
			ValueType getDefault() const {
				if constexpr (std::is_same_v<ValueType, String>)
					return "";
				else
					return 0;
			}

			template<typename S>
			void setFromString(S&& str) {
				_value = Utilities::Conversions::fromString<ValueType>(std::forward<S>(str));
			}

			template<typename A>
			void setFromArithmetic(A a) {
				if constexpr (Utilities::IsStringV<ValueType>)
					_value = Utilities::Conversions::toString<ValueType>(a);
				else
					_value = Utilities::Conversions::safeArithmeticCast<ValueType>(a);
			}

			ValueType _value;
		};

		using FloatValue = Value<Float>;
		using IntegerValue = Value<Integer>;
		using StringValue = Value<String>;

		/*
		 *  AnyValue: Holder for any Value<T>, supposed to offer Value<T>'s public interface by visiting its held variant.
		 */
		class AnyValue {
		public:
			template<typename T>
			AnyValue(const ValueType type, T&& initialValue) {
				switch (type) {
					case ValueType::Float:
						_anyValue = FloatValue(std::forward<T>(initialValue));
						break;
					case ValueType::Integer:
						_anyValue = IntegerValue(std::forward<T>(initialValue));
						break;
					case ValueType::String:
						_anyValue = StringValue(std::forward<T>(initialValue));
						break;
					default:
						throw Exceptions::InvalidArgument("AnyValue::AnyValue() failed, unknown type supplied.");
						break;
				}
			}

			template<typename T>
			void set(T&& newValue) {
				std::visit( [ &newValue ] ( auto& valueInVariant ) {
					valueInVariant.set(std::forward<T>(newValue));
				}, _anyValue);
			}

			auto getAsString() const {
				return std::visit( [ ] ( const auto& valueInVariant ) {
					return valueInVariant.getAsString();
				}, _anyValue);
			}

			template<typename T>
			auto getAs() const {
				return std::visit( [ ] ( const auto& valueInVariant ) {
						return valueInVariant.template getAs<T>();
				}, _anyValue);
			}

		private:
			std::variant<FloatValue, IntegerValue, StringValue> _anyValue;
		};

	public:
		template<typename T>
		Cvar(const ValueType type, T&& initialValue) : _value(type, std::forward<T>(initialValue)) {
		}

		String getAsString() const;

		template<typename T>
		void set(T&& newValue) {
			_value.set(std::forward<T>(newValue));
		}

	private:
		AnyValue _value;
	};
}

