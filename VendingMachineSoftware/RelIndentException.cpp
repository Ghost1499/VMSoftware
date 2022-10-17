#include "RelIndentException.h"

void exceptions::RelIndentException::init_err_mess() {
	this->error_mess += " rel_indent = " + std::to_string(rel_indent);
	this->error_mess += " ось - " + ax_names[axis];
}

exceptions::RelIndentException::RelIndentException(float rel_indent, features::Axis axis) :rel_indent(rel_indent), axis(axis), VMSoftException("Ошибка в значении отступа.") {
	this->init_err_mess();
}

exceptions::RelIndentException::RelIndentException(string_view error, float rel_indent, Axis axis) :rel_indent(rel_indent), axis(axis), VMSoftException(error) {
	this->init_err_mess();
}