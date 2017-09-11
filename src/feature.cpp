/************************************************************************* > File Name: feature.cpp > Author: lipei
	> Mail: b111180082@163.com 
	> Created Time: 2017年09月10日 星期日 19时43分07秒
 ************************************************************************/
#include "feature.h"
#include <cassert>

#ifdef USE_SIFT
#include <opencv2/xfeatures2d.hpp>
#endif


namespace VISG{

void Feature::extract(cv::Mat &img){
	if(img.empty())
		assert(false);
#ifdef USE_ORB
	cv::Ptr<cv::ORB> orb = cv::ORB::create(500,1.2f,8,31,0,2,cv::ORB::HARRIS_SCORE,31,20);
	orb->detect(img,key_points_);
	orb->compute(img, key_points_,descriptors_);
#endif

#ifdef USE_SIFT
	cv::Ptr<cv::Feature2D> sift = cv::xfeatures2d::SIFT::create(0,3,0.04,10);
	sift->detectAndCompute(img,cv::noArray(),key_points_,descriptors_);
#endif
//
//	cv::Mat img_key_points;
//	cv::drawKeypoints(img,key_points_,img_key_points,cv::Scalar::all(-1),cv::DrawMatchesFlags::DEFAULT);
//
//	cv::namedWindow("keyPoints",cv::WINDOW_AUTOSIZE);
//	cv::imshow("keyPoints",img_key_points);
//	cv::waitKey(0);
//	cv::destroyAllWindows();
}
}
