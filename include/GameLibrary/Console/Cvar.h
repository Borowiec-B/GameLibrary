#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <type_traits>

#include "GameLibrary/Console/Types.h"
#include "GameLibrary/Utilities/Conversions/ArithmeticString.h"


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

	private:

		/*
		 *  Value: Cvar's class for managing a raw value.
		 * 		   Currently primary function is get/set functionality, with setter converting a wide range of types into ValueType.
		 */
		template<typename VT>
		class Value {
			using ValueType = VT;

			constexpr static auto _floatToStringPrecision = Utilities::Conversions::FloatPrecision(4);
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
				try {
					_value = Utilities::Conversions::arithmeticOrStringCast<ValueType>(std::forward<T>(newValue), _floatToStringPrecision);
				} catch (Exceptions::ConversionError) {
					throw Exceptions::ConversionError::fromTypes<T, ValueType>("Cvar::Value::set() failed.");
				}
			}

			/*
			 *  getAs(): Use arithmeticOrStringCast() to get given type, or - if that's not possible - try to construct T(_value).
			 *
			 *  Throws:
			 *    - ConversionError if any used function throws.
			 */
			template<typename T>
			T getAs() const {
				try
				{
					if constexpr (Utilities::IsArithmeticOrStringV<T>)
						return Utilities::Conversions::arithmeticOrStringCast<T>(_value, _floatToStringPrecision);
					else
						return T(_value);
				}
				catch (...)
				{
					throw Exceptions::ConversionError::fromTypes<ValueType, T>("Cvar::Value::getAs() failed.");
				}
			}

			String getAsString() const {
				return getAs<String>();
			}

		private:
			constexpr ValueType getDefault() const {
				if constexpr (std::is_same_v<ValueType, String>)
					return "";
				else
					return 0;
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
			template<typename... Args>
			AnyValue(const ValueType type, Args&&... initialValue) {
				initValue(type, std::forward<Args>(initialValue)...);
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
			template<typename... Args>
			void initValue(const ValueType type, Args&&... ctorArgs) {
				switch (type) {
					case ValueType::Float:
						_anyValue = FloatValue(std::forward<Args>(ctorArgs)...);
						break;
					case ValueType::Integer:
						_anyValue = IntegerValue(std::forward<Args>(ctorArgs)...);
						break;
					case ValueType::String:
						_anyValue = StringValue(std::forward<Args>(ctorArgs)...);
						break;
					default:
						throw Exceptions::InvalidArgument("AnyValue::initValue() failed, unknown type supplied.");
						break;
				}
			}

			std::variant<FloatValue, IntegerValue, StringValue> _anyValue;
		};

	public:
		template<typename T>
		Cvar(String name, const ValueType type, T&& initialValue) : _name(name), _value(type, std::forward<T>(initialValue)) {}
		Cvar(String name, const ValueType type) : _name(name), _value(type) {}

		String getAsString() const;

		template<typename T>
		void set(T&& newValue) {
			_value.set(std::forward<T>(newValue));
		}

		void setDescription(String newDescription);
		const String& getDescription() const;

		const String& getName() const;

		template<typename T>
		auto getAs() const {
			return _value.getAs<T>();
		}

	private:
		String _name;
		String _description;
		AnyValue _value;
	};
}

