#pragma once
#pragma once
#include <map>
#include "VMSoftException.h"
#include "FeatureExtractor.h"

namespace exceptions {
	using std::stringstream;
	using features::Axis;

	static std::map<Axis, string> ax_names{ {Axis::Vertical,"вертикальная"},{Axis::Horizontal,"горизонтальная"} };

	class RelIndentException: public VMSoftException {
	private:
		void init_err_mess();
	protected:
		float rel_indent;
		Axis axis;
	public:
		RelIndentException(float rel_indent, Axis axis);

		RelIndentException(string_view error, float rel_indent, Axis axis);
	};
}