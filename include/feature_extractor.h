/*************************************************************************
	> File Name: feature_extractor.h
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月13日 星期三 13时23分04秒
 ************************************************************************/
#ifndef FEATURE_EXTRACTOR_H
#define FEATURE_EXTRACTOR_H

#include "common.h"
#include "common.h"
#include "feature.h"
#include "matcher.h"
#include "pose.h"
#include "camera.h"

namespace VISG{

class FeatureExtractor{
public:
	FeatureExtractor() = default;
	void extract_match(cv::Mat &img1,cv::Mat &img2);
	FeaturePairs features_pairs()const{
		return features_pairs_;
	}
private:
	FeaturePairs features_pairs_;
};

}

#endif // feature_extractor.h
