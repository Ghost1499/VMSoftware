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
		throw VMSoftException("�������� feature_extractor ����� ������������ ��������.");
	this->feature_extractor = feature_extractor;
	set_thresh(thresh);
}

classification::BottleType classification::Classifier::classify(const Mat mask,const vector<Point>& object_contour)
{
	assert(!mask.empty() && "������� ����� ������.");
	assert(!object_contour.empty() && "������� ������ ������.");
	float feature = this->feature_extractor->extract(mask, object_contour);
#ifdef VALIDATE
	std::cout << "�������� �������� - " << feature << std::endl;
#endif // VALIDATE
	BottleType bottleType = feature > this->thresh ? BottleType::Bottle : BottleType::Can;
	return bottleType;
}

classification::Classifier::~Classifier() {
	delete this->feature_extractor;
}
