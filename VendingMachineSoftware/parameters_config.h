#pragma once
#include "FeatureExtractor.h"


namespace config {
	using features::Axis;

	int SLICES_COUNT = 5;
	Axis SLICE_AXIS = Axis::Horizontal;
	Axis FE_IMG_ORIENT = Axis::Horizontal;
	float REL_INDENT_X = 0.02f;
	float REL_INDENT_Y = 0.0f;
	float BOTTLE_CLASS_THRESH = 0.07f;
}