/*************************************************************************
	> File Name: ../src/pose.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月11日 星期一 19时31分46秒
 ************************************************************************/

#include "pose.h"
namespace VISG{

Pose::Pose(){
	R_ = (cv::Mat_<double>(3,3) <<
				1,0,0,
				0,1,0,
				0,0,1);
	t_ = (cv::Mat_<double>(3,1) << 0,0,0);
}
/*
	@brief:estimate pose from two frame features using recoverPose
*/
void Pose::estimate(const KeyPoints &key_points1,
	const KeyPoints &key_points2,
	const DMatches & matches,
	const Camera &cam){
	std::vector<cv::Point2f> points1(matches.size());
	std::vector<cv::Point2f> points2(matches.size());
	for(size_t i = 0; i < matches.size(); ++i){
		points1[i] = key_points1[matches[i].queryIdx].pt;
		points2[i] = key_points2[matches[i].trainIdx].pt;
	}

	int focal_length = static_cast<int>(cam.K().at<double>(0,0) +
			cam.K().at<double>(1,1)) / 2;
	cv::Point2d principal_point(cam.K().at<double>(0,2),cam.K().at<double>(1,2));

	cv::Mat inlers;
	cv::Mat E = cv::findEssentialMat(points1,points2,focal_length,principal_point,cv::RANSAC,0.999,1.0,inlers);
	cv::recoverPose(E,points1,points2,R_,t_,focal_length,principal_point,inlers);
	cv::Mat Rvec;
	cv::Rodrigues(R_,Rvec);
	std::cout << "Rvec: " << std::endl << Rvec << std::endl;
	std::cout << "t: " << std::endl << t_ << std::endl;
}
/*
	@brief:estimate pose from two frame features using recoverPose
*/
void Pose::estimate(const FeaturePairs &features_pairs,const cv::Mat &K){
	std::vector<cv::Point2f> points1(features_pairs.size());
	std::vector<cv::Point2f> points2(features_pairs.size());
	for(size_t i = 0; i < features_pairs.size(); ++i){
		points1[i] = features_pairs[i].first.pt;
		points2[i] = features_pairs[i].second.pt;
	}
	int focal_length = static_cast<int>(K.at<double>(0,0) +
			K.at<double>(1,1)) / 2;
	cv::Point2d principal_point(K.at<double>(0,2),K.at<double>(1,2));

	cv::Mat inlers;
	cv::Mat E = cv::findEssentialMat(points1,points2,focal_length,principal_point,cv::RANSAC,0.999,1.0,inlers);
	cv::recoverPose(E,points1,points2,R_,t_,focal_length,principal_point,inlers);
#ifdef TEST
	cv::Mat Rvec;
	cv::Rodrigues(R_,Rvec);
	std::cout << "Rvec: " << std::endl << Rvec << std::endl;
	std::cout << "t: " << std::endl << t_ << std::endl;
#endif
}

/*
	@brief:estimate pose from two frame features using recoverPose
*/
void Pose::estimate(const FeaturePairs &features_pairs,const Camera &cam){
	estimate(features_pairs,cam.K());
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
/*
	@brief convert to eigen
	R
*/

Eigen::Matrix3d Pose::EigenR() const{
	Eigen::Matrix3d R;
	R << R_.at<double>(0,0),R_.at<double>(0,1),R_.at<double>(0,2),
	R_.at<double>(1,0),R_.at<double>(1,1),R_.at<double>(1,2),
	R_.at<double>(2,0),R_.at<double>(2,1),R_.at<double>(2,2);
	return R;
}
/*
	@brief convert to eigen
	t
*/
Eigen::Vector3d Pose::Eigent() const{
	return Eigen::Vector3d(t_.at<double>(0,0),t_.at<double>(1,0),t_.at<double>(2,0));
}
/*
	@brief convert to eigen
	Matrix3_4d
*/
Eigen::Matrix<double,3,4> Pose::EigenPoseMatrix3_4() const{
	Eigen::Matrix<double,3,4> matrix3_4d;
	matrix3_4d <<
	R_.at<double>(0,0),R_.at<double>(0,1),R_.at<double>(0,2),t_.at<double>(0,0),
	R_.at<double>(1,0),R_.at<double>(1,1),R_.at<double>(1,2),t_.at<double>(1,0),
	R_.at<double>(2,0),R_.at<double>(2,1),R_.at<double>(2,2),t_.at<double>(2,0);
	return matrix3_4d;
}
}
