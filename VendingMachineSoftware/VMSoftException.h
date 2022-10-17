#pragma once
#include <stdexcept>
#include <exception>

namespace exceptions {
	using std::exception, std::string, std::string_view;

	class VMSoftException : public exception {
	protected:
		string error_mess;

	public:
		VMSoftException() :exception("Исключение VMSoftware."){}

		VMSoftException(string_view error) {
			this->error_mess = error;
		}
		
		const char* what() const noexcept override {
			return error_mess.c_str();
		}
	};
}