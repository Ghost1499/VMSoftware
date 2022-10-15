#include "Classifier.h"

float classification::Classifier::get_thresh()
{
	return this->thresh;
}

void classification::Classifier::set_thresh(float value)
{
	this->thresh = value;
}

classification::Classifier::Classifier(IFeatureExtractor* feature_extractor, float thresh)
{
	if (feature_extractor == nullptr)
		throw VMSoftException("Параметр feature_extractor имеет некорректное значение.");
	this->feature_extractor = feature_extractor;
	set_thresh(thresh);
}

classification::BottleType classification::Classifier::classify(const Mat mask,const vector<Point>& object_contour)
{
	assert(!mask.empty() && "Входная маска пустая.");
	assert(!object_contour.empty() && "Входной контур пустой.");
	float feature = this->feature_extractor->extract(mask, object_contour);
#ifdef VALIDATE
	std::cout << "Значение признака - " << feature << std::endl;
#endif // VALIDATE
	BottleType bottleType = feature > this->thresh ? BottleType::Bottle : BottleType::Can;
	return bottleType;
}

classification::Classifier::~Classifier() {
	delete this->feature_extractor;
}
