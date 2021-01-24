#pragma once

#include <optional>
#include <stdexcept>

#include <boost/type_index.hpp>


namespace GameLibrary::Exceptions
{
	class ConversionError : public std::runtime_error {
	public:
		ConversionError(std::string message) : std::runtime_error(std::move(message)) {}

		template<typename From, typename To>
		static ConversionError fromTypes() {
			return ConversionError(makeMessage<From, To>());
		}

		template<typename From, typename To>
		static ConversionError fromTypes(std::string message) {
			return ConversionError(makeMessage<From, To>(std::move(message)));
		}

	private:
		template<typename From, typename To>
		static std::string types() {
			const auto prettyFrom = boost::typeindex::type_id<From>().pretty_name();
			const auto prettyTo = boost::typeindex::type_id<To>().pretty_name();

			return "From: \"" + prettyFrom + " To: \"" + "\"";
		}

		template<typename From, typename To>
		static std::string makeMessage(std::optional<std::string> message = std::nullopt) {
			std::string ret = "Failed to convert an object.\n";
			ret += types<From, To>();

			if (message.has_value())
				ret += "\nMessage: \"" + std::move(message.value()) + "\".";

			return ret;
		}
	};
}
