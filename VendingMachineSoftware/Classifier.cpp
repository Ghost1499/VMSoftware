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
	this->feature_extractor = feature_extractor;
	set_thresh(thresh);
}

classification::BottleType classification::Classifier::classify(Mat mask, vector<Point> object_contour)
{
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
