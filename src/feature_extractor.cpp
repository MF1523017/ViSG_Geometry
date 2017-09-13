/*************************************************************************
	> File Name: feature_extractor.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月13日 星期三 13时27分46秒
 ************************************************************************/

#include "feature_extractor.h"

namespace VISG{

void FeatureExtractor::extract_match(cv::Mat &img1,cv::Mat &img2){
	std::vector<cv::KeyPoint> key_points1,key_points2;
	cv::Mat descriptors1,descriptors2;
	std::vector<cv::DMatch> matches;
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

	cv::Mat img_match, img_good_match;
	cv::drawMatches(img1,key_points1,img2,key_points2,matches,img_match);
	cv::namedWindow("feature match",0);
	cv::imshow("feature match" ,img_match);
	cv::waitKey(0);

	features_pairs_.resize(matches.size());
	for(size_t i = 0; i < matches.size(); ++i){
		features_pairs_[i] = std::make_pair(key_points1[matches[i].queryIdx].pt,key_points2[matches[i].trainIdx].pt);
	}

}
}
