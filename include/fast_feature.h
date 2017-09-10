/*************************************************************************
	> File Name: fast_feature.h
	> Author: lipei
	> Mail: b111180082@163.com 
	> Created Time: 2017年09月10日 星期日 19时33分44秒
 ************************************************************************/

#ifndef FAST_FEATURE_H
#define FAST_FEATURE_H
#include "common.h"
namespace VISG{

class FastFeature{
public:
	FastFeature() = default;
	void extract(cv::Mat &img);
private:
	std::vector<cv::KeyPoint> key_points_;

};

}


#endif // fast_feature.h
