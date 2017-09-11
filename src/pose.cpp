/*************************************************************************
	> File Name: ../src/pose.cpp
	> Author: lipei
	> Mail: b111180082@163.com 
	> Created Time: 2017年09月11日 星期一 19时31分46秒
 ************************************************************************/

#include "pose.h"
namespace VISG{

void Pose::estimate(const std::vector<cv::KeyPoint> &key_points1, const std::vector<cv::KeyPoint> &key_points2,const std::vector<cv::DMatch> & matches,const Camera &cam){
	std::vector<cv::Point2f> points1(matches.size());
	std::vector<cv::Point2f> points2(matches.size());
	for(size_t i = 0; i < matches.size(); ++i){
		points1[i] = key_points1[matches[i].queryIdx].pt;
		points2[i] = key_points2[matches[i].trainIdx].pt;
	}

	int focal_length = static_cast<int>(cam.K().at<double>(0,0) +
			cam.K().at<double>(1,1));
	cv::Point2d principal_point(cam.K().at<double>(0,2),cam.K().at<double>(1,2));

	cv::Mat mask;
	cv::Mat E = cv::findEssentialMat(points1,points2,focal_length,principal_point,cv::RANSAC,0.999,1.0,mask);
	std::cout << "essential matrix:" << E << std::endl;
	
	cv::recoverPose(E,points1,points2,R_,t_,focal_length,principal_point,mask);
	std::cout << "R: " << std::endl << R_ << std::endl;
	std::cout << "t: " << std::endl << t_ << std::endl;
}

}
