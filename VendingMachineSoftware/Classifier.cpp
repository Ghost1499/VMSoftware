#include "Classifier.h"

float Classifier::get_thresh()
{
	return this->thresh;
}

void Classifier::set_thresh(float value)
{
	this->thresh = value;
}

Classifier::Classifier(IFeatureExtractor* feature_extractor, float thresh)
{
	this->feature_extractor = feature_extractor;
	set_thresh(thresh);
}

BottleType Classifier::classify(Mat mask, vector<Point> object_contour)
{
	float feature = this->feature_extractor->extract(mask, object_contour);
#ifdef VALIDATE
	cout << "Значение признака - " << feature << endl;
#endif // VALIDATE
	BottleType bottleType = feature > this->thresh ? BottleType::Bottle : BottleType::Can;
	return bottleType;
}

Classifier::~Classifier() {
	delete this->feature_extractor;
}
