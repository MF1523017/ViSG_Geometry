/*************************************************************************
	> File Name: ../src/pose.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月11日 星期一 19时31分46秒
 ************************************************************************/

#include "pose.h"
namespace VISG{

/*
	@brief:estimate pose from two frame features using recoverPose
*/
void Pose::estimate(const std::vector<cv::KeyPoint> &key_points1,
	const std::vector<cv::KeyPoint> &key_points2,
	const std::vector<cv::DMatch> & matches,
	const Camera &cam,
	cv::Mat &inlers){
	std::vector<cv::Point2f> points1(matches.size());
	std::vector<cv::Point2f> points2(matches.size());
	for(size_t i = 0; i < matches.size(); ++i){
		points1[i] = key_points1[matches[i].queryIdx].pt;
		points2[i] = key_points2[matches[i].trainIdx].pt;
	}

	int focal_length = static_cast<int>(cam.K().at<double>(0,0) +
			cam.K().at<double>(1,1)) / 2;
	cv::Point2d principal_point(cam.K().at<double>(0,2),cam.K().at<double>(1,2));

	cv::Mat E = cv::findEssentialMat(points1,points2,focal_length,principal_point,cv::RANSAC,0.999,1.0,inlers);
	std::cout << "essential matrix:" << E << std::endl;

	cv::recoverPose(E,points1,points2,R_,t_,focal_length,principal_point,inlers);
	cv::Mat Rvec;
	cv::Rodrigues(R_,Rvec);
	std::cout << "Rvec: " << std::endl << Rvec << std::endl;
	std::cout << "t: " << std::endl << t_ << std::endl;
}
/*
	@brief:estimate pose from two frame features using recoverPose
*/
void Pose::estimate(const FeaturePairs &features_pairs,const Camera &cam,cv::Mat &inlers){
	std::vector<cv::Point2f> points1(features_pairs.size());
	std::vector<cv::Point2f> points2(features_pairs.size());
	for(size_t i = 0; i < features_pairs.size(); ++i){
		points1[i] = features_pairs[i].first;
		points2[i] = features_pairs[i].second;
	}
	int focal_length = static_cast<int>(cam.K().at<double>(0,0) +
			cam.K().at<double>(1,1)) / 2;
	cv::Point2d principal_point(cam.K().at<double>(0,2),cam.K().at<double>(1,2));

	cv::Mat E = cv::findEssentialMat(points1,points2,focal_length,principal_point,cv::RANSAC,0.999,1.0,inlers);
	std::cout << "essential matrix:" << E << std::endl;

	cv::recoverPose(E,points1,points2,R_,t_,focal_length,principal_point,inlers);
	cv::Mat Rvec;
	cv::Rodrigues(R_,Rvec);
	std::cout << "Rvec: " << std::endl << Rvec << std::endl;
	std::cout << "t: " << std::endl << t_ << std::endl;
}
/*
	@brief:estimate pose from correspondences(point2 <==> point3) using solvePnP
*/

void Pose::estimate(const std::vector<cv::Point2f> &points2,const std::vector<cv::Point3f> &points3,const Camera &cam){
	if(points2.size() != points3.size()){
		std::cout << "correspondences error " << std::endl;
		return;
	}
	cv::solvePnP(points3,points2,cam.K(),cv::Mat(),R_,t_,false,cv::SOLVEPNP_EPNP);
	cv::Mat Rvec;
	cv::Rodrigues(R_,Rvec);
	std::cout << "Rvec: " << std::endl << Rvec << std::endl;
	std::cout << "t: " << std::endl << t_ << std::endl;
}

/*
	@brief T = [R|t]
*/
cv::Mat Pose::cvPoseMatrix3_4() const{
	return (cv::Mat_<double>(3,4) <<
			R_.at<double>(0,0),R_.at<double>(0,1),R_.at<double>(0,2),t_.at<double>(0,0),
			R_.at<double>(1,0),R_.at<double>(1,1),R_.at<double>(1,2),t_.at<double>(1,0),
			R_.at<double>(2,0),R_.at<double>(2,1),R_.at<double>(2,2),t_.at<double>(2,0));
}

}
