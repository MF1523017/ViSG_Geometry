/*************************************************************************
	> File Name: ../src/matcher.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月11日 星期一 10时50分08秒
 ************************************************************************/
#include "matcher.h"

namespace VISG{

	void Matcher::match(const cv::Mat &descriptors1,const cv::Mat &descriptors2){
		if(descriptors1.type() == descriptors2.type() &&
				descriptors1.cols == descriptors2.cols){
#ifdef USE_SIFT
			cv::BFMatcher matcher (cv::NORM_L2);
#endif
#ifdef USE_ORB
			cv::BFMatcher matcher (cv::NORM_HAMMING);
#endif
			matcher.match(descriptors1,descriptors2,matches_);
		}
	}

}
