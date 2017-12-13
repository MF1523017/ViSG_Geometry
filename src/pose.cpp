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
	cRw = cv::Mat::eye(3,3,CV_32F);
	ctw = cv::Mat::zeros(3,1,CV_32F);
}
/*
	@brief:estimate pose from two frame features using recoverPose(base)
*/
bool Pose::Estimate(const std::vector<cv::Point2f> &points1,const std::vector<cv::Point2f> &points2,const Camera &cam,cv::Mat &inliers){

	cv::Mat E = cv::findEssentialMat(points1,points2,cam.K,cv::RANSAC,0.999,1.0,inliers);
	if(E.empty()){
		std::cout << "[Pose::Estimate]E empty error!" << std::endl;
		return false;
	}
	std::cout << "[Pose::Estimate]3" << std::endl;
	int valid_count = cv::countNonZero(inliers);
	if(valid_count < 10 || static_cast<double>(valid_count) / points1.size() < 0.6){
		std::cout << "[Pose::Estimate]valid_count error!" << valid_count<< std::endl;
		return false;
	}
	std::cout << "[Pose::Estimate]4" << std::endl;
	cv::Mat mask = inliers.clone();
	std::cout << "[Pose::Estimate]5" << std::endl;
	cv::recoverPose(E,points1,points2,cam.K,cRw,ctw,mask);
	std::cout << "[Pose::Estimate]6" << std::endl;
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
	std::cout << "[Pose::Estimate]0!" << std::endl;
	bool ret = Estimate(points11,points22,cam,inliers);
		std::cout << "[Pose::Estimate]1!" << std::endl;
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
	cv::Rodrigues(cRw,Rvec);
	// std::cout << "[Pose::Estimate] Rvec: " << std::endl << Rvec << std::endl;
	std::cout << "[Pose::Estimate] R[3*3]: " << std::endl << cRw << std::endl;
	std::cout << "[Pose::Estimate] t: " << std::endl << ctw << std::endl;
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
	cv::Rodrigues(cRw,Rvec);
	// std::cout << "[Pose::Estimate] Rvec: " << std::endl << Rvec << std::endl;
	std::cout << "[Pose::Estimate] R[3*3]: " << std::endl << cRw << std::endl;
	std::cout << "[Pose::Estimate] t: " << std::endl << ctw << std::endl;
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
	cv::Mat Rvec(3,1,CV_32F);
	// bool ret = cv::solvePnP(points3,points2,cam.K(),cv::Mat(),Rvec,t_,false,cv::SOLVEPNP_ITERATIVE);
	bool ret = cv::solvePnPRansac(points3,points2,cam.K,cv::noArray(),Rvec,ctw);
	if(!ret){
		std::cout << "[Pose::Estimate] pnp error" << std::endl;
		return false;
	}
	cv::Rodrigues(Rvec,cRw);
#ifdef TEST
	// std::cout << "[Pose::Estimate] Rvec: " << std::endl << Rvec << std::endl;
	std::cout << "[Pose::Estimate] R[3*3]: " << std::endl << cRw << std::endl;
	std::cout << "[Pose::Estimate] t: " << std::endl << ctw << std::endl;
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
	cv::Mat R_,t_;
	cRw.convertTo(R_,CV_32F);
	ctw.convertTo(t_,CV_32F);
	return (cv::Mat_<float>(3,4) <<
			R_.at<float>(0,0),R_.at<float>(0,1),R_.at<float>(0,2),t_.at<float>(0,0),
			R_.at<float>(1,0),R_.at<float>(1,1),R_.at<float>(1,2),t_.at<float>(1,0),
			R_.at<float>(2,0),R_.at<float>(2,1),R_.at<float>(2,2),t_.at<float>(2,0));
	// return (cv::Mat_<float>(3,4) <<
	// 		R_.at<float>(0,0),R_.at<float>(0,1),R_.at<float>(0,2),t_.at<float>(0,0),
	// 		R_.at<float>(1,0),R_.at<float>(1,1),R_.at<float>(1,2),t_.at<float>(1,0),
	// 		R_.at<float>(2,0),R_.at<float>(2,1),R_.at<float>(2,2),t_.at<float>(2,0));
}
/*
	@brief convert to eigen
	R
*/

Eigen::Matrix3f Pose::EigenR() const{
	Eigen::Matrix3f R;
	cv::Mat R_;
	cRw.convertTo(R_,CV_32F);
	R << R_.at<float>(0,0),R_.at<float>(0,1),R_.at<float>(0,2),
	R_.at<float>(1,0),R_.at<float>(1,1),R_.at<float>(1,2),
	R_.at<float>(2,0),R_.at<float>(2,1),R_.at<float>(2,2);
	return R;
}
/*
	@brief convert to eigen
	t
*/
Eigen::Vector3f Pose::Eigent() const{
	cv::Mat t_;
	ctw.convertTo(t_,CV_32F);
	return Eigen::Vector3f(t_.at<float>(0,0),t_.at<float>(1,0),t_.at<float>(2,0));
}
/*
	@brief convert to eigen
	Matrix3_4d
*/
Eigen::Matrix<float,3,4> Pose::EigenPoseMatrix3_4() const{
	cv::Mat R_,t_;
	cRw.convertTo(R_,CV_32F);
	ctw.convertTo(t_,CV_32F);
	Eigen::Matrix<float,3,4> matrix3_4d;
	matrix3_4d <<
	R_.at<float>(0,0),R_.at<float>(0,1),R_.at<float>(0,2),t_.at<float>(0,0),
	R_.at<float>(1,0),R_.at<float>(1,1),R_.at<float>(1,2),t_.at<float>(1,0),
	R_.at<float>(2,0),R_.at<float>(2,1),R_.at<float>(2,2),t_.at<float>(2,0);
	return matrix3_4d;
}
}
