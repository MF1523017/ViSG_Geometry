/*************************************************************************
	> File Name: pose.h
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月11日 星期一 19时15分53秒
 ************************************************************************/
#ifndef POSE_H
#define POSE_H

#include "common.h"
#include "camera.h"
#include <iostream>
#include <vector>


namespace VISG{

class Pose{
public:
	using Ptr = std::shared_ptr<Pose>;
	Pose();
	Pose(cv::Mat R,cv::Mat t):R_(R),t_(t){}
	bool Estimate(const std::vector<cv::Point2f> &points1,const std::vector<cv::Point2f> &points2,const Camera &cam,cv::Mat &inliers);
	bool Estimate(const KeyPoints &key_points1,
		const KeyPoints &key_points2,
		const Camera &cam,
		DMatches &matches,
		std::vector<cv::Point2f> &points1,
		std::vector<cv::Point2f> &points2);
	bool Estimate(FeaturePairs &features_pairs,const Camera &cam);
	void Estimate(const std::vector<cv::Point2f> &points2,const std::vector<cv::Point3f> &points3,const Camera &cam);
	void Estimate(const PnP &pnp,const Camera &cam);
	cv::Mat R()const {
		return R_;
	}
	cv::Mat Rvec() const{
		cv::Mat Rvec_;
		cv::Rodrigues(R_,Rvec_);
		return Rvec_;
	}
	cv::Mat t()const {
		return t_;
	}
	cv::Mat cvPoseMatrix3_4()const;
	Eigen::Matrix3d EigenR()const;
	Eigen::Vector3d Eigent()const;
	Eigen::Matrix<double,3,4> EigenPoseMatrix3_4()const;

private:

	cv::Mat R_;//3*3 matrix
	cv::Mat t_;
};


}

#endif// pose.h
