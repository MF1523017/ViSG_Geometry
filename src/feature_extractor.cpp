/*************************************************************************
	> File Name: feature_extractor.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月13日 星期三 13时27分46秒
 ************************************************************************/

#include "feature_extractor.h"

namespace VISG{

FeatureResult  FeatureExtractor::extract_match(cv::Mat &img1,cv::Mat &img2){
	KeyPoints key_points1,key_points2;
	cv::Mat descriptors1,descriptors2;
	DMatches matches;
	DMatches good_matches;
#ifdef USE_ORB
	cv::Ptr<cv::ORB> orb = cv::ORB::create();
	// cv::Ptr<cv::ORB> orb = cv::ORB::create(500,1.2f,8,31,0,2,cv::ORB::HARRIS_SCORE,31,20);
	orb->detect(img1,key_points1);
	orb->detect(img2,key_points2);
	orb->compute(img1, key_points1,descriptors1);
	orb->compute(img2, key_points2,descriptors2);
	cv::BFMatcher matcher (cv::NORM_HAMMING);
#endif

#ifdef USE_SIFT
		// cv::Ptr<cv::Feature2D> sift = cv::xfeatures2d::SIFT::create(0,3,0.04,10);
	cv::Ptr<cv::Feature2D> sift = cv::xfeatures2d::SIFT::create();
	sift->detectAndCompute(img1,cv::noArray(),key_points1,descriptors1);
	sift->detectAndCompute(img2,cv::noArray(),key_points2,descriptors2);
	cv::BFMatcher matcher (cv::NORM_L2);
#endif

	matcher.match(descriptors1,descriptors2,matches);
	auto min_dis = (*(min_element(matches.begin(),matches.end(),matches_dis_cmp))).distance;
	// cout << "min_dis " << min_dis << endl;
	for(size_t i = 0; i < matches.size(); ++i){
		if(matches[i].distance <= std::max(static_cast<double>(2 * min_dis) ,30.0)){
			good_matches.push_back(matches[i]);
			features_pairs_.push_back(std::make_pair(key_points1[matches[i].queryIdx],key_points2[matches[i].trainIdx]));
		}
	}
	cv::Mat img_match;
	cv::drawMatches(img1,key_points1,img2,key_points2,good_matches,img_match);
	cv::namedWindow("feature match",0);
	cv::imshow("feature match" ,img_match);
	cv::waitKey(5);

	FeatureResult result(key_points1,key_points2,matches);
	return result;
}
}
