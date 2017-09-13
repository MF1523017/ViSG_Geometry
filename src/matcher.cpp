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
	/*
		// get inliers with homography
    	if(matches_.size() < 8){// 8 points method
			std::cout << "matches is less 8 " << std::endl;
    		return;
		}

    	std::vector<cv::Point2f> src_points(matches_.size());
    	std::vector<cv::Point2f> dst_points(matches_.size());
    	for(size_t i = 0; i < matches_.size(); ++i){
			src_points[i] = key_points1[matches_[i].trainIdx].pt;
			dst_points[i] = key_points2[matches_[i].queryIdx].pt;
		}
		std::cout << "findHomography " << std::endl;
		std::vector<unsigned char> inliers_mask;
		auto H = cv::findHomography(src_points,dst_points,cv::RANSAC,5,inliers_mask);

		for(size_t i = 0; i < inliers_mask.size(); ++i){
			if(inliers_mask[i])
				inlier_matches_.push_back(matches_[i]);
		}
*/
	}

}
