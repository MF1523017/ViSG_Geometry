/*************************************************************************
	> File Name: fast_feature.cpp
	> Author: lipei
	> Mail: b111180082@163.com 
	> Created Time: 2017年09月10日 星期日 19时43分07秒
 ************************************************************************/
#include "fast_feature.h"
#include <cassert>
namespace VISG{

void FastFeature::extract(cv::Mat &img){
	if(img.empty())
		assert(false);
	cv::FAST(img,key_points_,20,1,cv::FastFeatureDetector::TYPE_9_16);

	cv::Mat img_key_points;
	cv::drawKeypoints(img,key_points_,img_key_points,cv::Scalar::all(-1),cv::DrawMatchesFlags::DEFAULT);

	cv::imshow("keyPoints",img_key_points);
	cv::waitKey(0);
}
}
