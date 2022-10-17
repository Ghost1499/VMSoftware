#pragma once
#include "VMSoftException.h"
namespace exceptions {
	using std::stringstream;
	class SlicesCountException : public VMSoftException {
	private:
		void init_err_mess() {
			this->error_mess += " slices_count = " + std::to_string(slices_count);
		}
	protected:
		int slices_count;
	public:
		SlicesCountException(int slices_count) :slices_count(slices_count), VMSoftException("Ошибка в количестве слайсов.") {
			this->init_err_mess();
		}
		
		SlicesCountException(string_view error, int slices_count) :slices_count(slices_count), VMSoftException(error) {
			this->init_err_mess();
		}

		//не должно работать, потому что c_str() передает
		//указатель на временный объект ss.str()
		//const char* what() const noexcept override {
		//	stringstream ss;
		//	ss<<SlicesCountException::what()<< " slices_count = "<< slices_count;
		//	return ss.str().c_str();
		//}
	};
}