/*************************************************************************
	> File Name: ../src/pose.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月11日 星期一 19时31分46秒
 ************************************************************************/

#include "pose.h"
#include <cassert>

namespace VISG{

Pose::Pose(){
	R_ = cv::Mat::eye(3,3,CV_64FC1);
	t_ = cv::Mat::zeros(3,1,CV_64FC1);
}
/*
	@brief:estimate pose from two frame features using recoverPose(base)
*/
bool Pose::Estimate(const std::vector<cv::Point2f> &points1,const std::vector<cv::Point2f> &points2,const Camera &cam,cv::Mat &inliers){
	int focal_length = static_cast<int>(cam.K().at<double>(0,0) +
			cam.K().at<double>(1,1)) / 2;
	cv::Point2d principal_point(cam.K().at<double>(0,2),cam.K().at<double>(1,2));
	cv::Mat E = cv::findEssentialMat(points1,points2,focal_length,principal_point,cv::RANSAC,0.999,1.0,inliers);
	if(E.empty())
		return false;
	int valid_count = cv::countNonZero(inliers);
	if(valid_count < 10 || static_cast<double>(valid_count) / points1.size() < 0.6)
		return false;
	cv::recoverPose(E,points1,points2,R_,t_,focal_length,principal_point,inliers);
	return true;
}

/*
	@brief:estimate pose from two frame features using recoverPose
*/
bool Pose::Estimate(const KeyPoints &key_points1,
	const KeyPoints &key_points2,
	const Camera &cam,
	DMatches & matches,
	std::vector<cv::Point2f> &points1,
	std::vector<cv::Point2f> &points2){
	std::vector<cv::Point2f> points11(matches.size());
	std::vector<cv::Point2f> points22(matches.size());
	for(size_t i = 0; i < matches.size(); ++i){
		points11[i] = key_points1[matches[i].queryIdx].pt;
		points22[i] = key_points2[matches[i].trainIdx].pt;
	}
	cv::Mat inliers;
	bool ret = Estimate(points11,points22,cam,inliers);
	if(!ret)
		return false;
	auto matches_tmp = matches;
	matches.clear();
	for(size_t i = 0; i < inliers.rows; ++i){
		if(inliers.at<int>(i,0)){
			points1.push_back(points11[i]);
			points2.push_back(points22[i]);
			matches.push_back(matches_tmp[i]);
		}
	}
#ifdef TEST
	cv::Mat Rvec;
	cv::Rodrigues(R_,Rvec);
	// std::cout << "[Pose::Estimate] Rvec: " << std::endl << Rvec << std::endl;
	std::cout << "[Pose::Estimate] R[3*3]: " << std::endl << R_ << std::endl;
	std::cout << "[Pose::Estimate] t: " << std::endl << t_ << std::endl;
#endif
	return true;
}
/*
	@brief:estimate pose from two frame features using recoverPose
*/
bool Pose::Estimate(FeaturePairs &features_pairs,const Camera &cam){
	std::vector<cv::Point2f> points1(features_pairs.size());
	std::vector<cv::Point2f> points2(features_pairs.size());
	for(size_t i = 0; i < features_pairs.size(); ++i){
		points1[i] = features_pairs[i].first.pt;
		points2[i] = features_pairs[i].second.pt;
	}
	cv::Mat inliers;
	bool ret = Estimate(points1,points2,cam,inliers);
	if(!ret)
		return false;
	auto features_pairs_tmp = features_pairs;
	features_pairs.clear();
	for(size_t i = 0; i < inliers.rows; ++i){
		if(inliers.at<int>(i,0)){
			features_pairs.push_back(features_pairs_tmp[i]);
		}
	}
#ifdef TEST
	cv::Mat Rvec;
	cv::Rodrigues(R_,Rvec);
	// std::cout << "[Pose::Estimate] Rvec: " << std::endl << Rvec << std::endl;
	std::cout << "[Pose::Estimate] R[3*3]: " << std::endl << R_ << std::endl;
	std::cout << "[Pose::Estimate] t: " << std::endl << t_ << std::endl;
#endif
return true;
}

/*
	@brief:estimate pose from correspondences(point2 <==> point3) using solvePnP
*/

bool Pose::Estimate(const std::vector<cv::Point2f> &points2,const std::vector<cv::Point3f> &points3,const Camera &cam){
	if(points2.size() != points3.size()){
		std::cout << "[Pose::Estimate] correspondences error " << std::endl;
		return false;
	}
	cv::Mat Rvec = cv::Mat::zeros(3,1,CV_32FC1);;
	// bool ret = cv::solvePnP(points3,points2,cam.K(),cv::Mat(),Rvec,t_,false,cv::SOLVEPNP_ITERATIVE);
	bool ret = cv::solvePnPRansac(points3,points2,cam.K(),cv::noArray(),Rvec,t_);
	if(!ret){
		std::cout << "[Pose::Estimate] pnp error" << std::endl;
		return false;
	}
	cv::Rodrigues(Rvec,R_);
#ifdef TEST
	// std::cout << "[Pose::Estimate] Rvec: " << std::endl << Rvec << std::endl;
	std::cout << "[Pose::Estimate] R[3*3]: " << std::endl << R_ << std::endl;
	std::cout << "[Pose::Estimate] t: " << std::endl << t_ << std::endl;
#endif
	return true;
}

bool Pose::Estimate(const PnP &pnp,const Camera &cam){
	std::vector<cv::Point2f> points2(pnp.size());
	std::vector<cv::Point3f> points3(pnp.size());
	for(size_t i = 0; i < pnp.size(); ++i){
		points2[i] = pnp[i].first;
		points3[i] = pnp[i].second;
	}
	return Estimate(points2,points3,cam);
}

/*
	@brief T = [R|t]
*/
cv::Mat Pose::cvPoseMatrix3_4() const{
	return (cv::Mat_<double>(3,4) <<
			R_.at<double>(0,0),R_.at<double>(0,1),R_.at<double>(0,2),t_.at<double>(0,0),
			R_.at<double>(1,0),R_.at<double>(1,1),R_.at<double>(1,2),t_.at<double>(1,0),
			R_.at<double>(2,0),R_.at<double>(2,1),R_.at<double>(2,2),t_.at<double>(2,0));
	// return (cv::Mat_<float>(3,4) <<
	// 		R_.at<float>(0,0),R_.at<float>(0,1),R_.at<float>(0,2),t_.at<float>(0,0),
	// 		R_.at<float>(1,0),R_.at<float>(1,1),R_.at<float>(1,2),t_.at<float>(1,0),
	// 		R_.at<float>(2,0),R_.at<float>(2,1),R_.at<float>(2,2),t_.at<float>(2,0));
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
