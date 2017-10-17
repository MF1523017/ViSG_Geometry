/*************************************************************************
  > File Name: feature.cpp
  > Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月10日 星期日 19时43分07秒
 ************************************************************************/
 #include "feature.h"
 #include <cassert>

 namespace VISG{
 Feature::Feature(){
 #ifdef USE_ORB
 	detector_ = cv::ORB::create();
 #endif

 #ifdef USE_SIFT
 	detector_ = cv::xfeatures2d::SIFT::create();
 #endif

 }
 void Feature::Extract(const cv::Mat &img){
 	if(img.empty())
 		assert(false);
 	key_points_.clear();//clear old key_points
 	descriptors_.release();// clear old descriptors
 	detector_->detect(img,key_points_);
 	detector_->compute(img, key_points_,descriptors_);

 #ifdef TEST
 	cv::Mat img_key_points;
 	cv::drawKeypoints(img,key_points_,img_key_points,cv::Scalar::all(-1),cv::DrawMatchesFlags::DEFAULT);
 	cv::namedWindow("keyPoints",0);
 	cv::imshow("keyPoints",img_key_points);
 	cv::waitKey(2000);
 	cv::destroyAllWindows();
 #endif
 }
 }
