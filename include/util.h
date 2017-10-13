/*************************************************************************
	> File Name: util.h
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月10日 星期日 09时00分41秒
 ************************************************************************/
#ifndef UTIL_H
#define UTIL_H
#include "common.h"
#include "pose.h"
#include <iostream>
#include <string>

namespace VISG{
void video2images(const std::string& video_name,const std::string &image_dir);

// camera

cv::Point2f pixel2cam(const cv::Point2f &p, const cv::Mat & K);

cv::Mat cvMatrix3_4(const Pose &pose);
Eigen::Vector3d cv2eigen_Vector3d(const cv::Point3d &p);

cv::Point3f cvPointd2f(const cv::Point3d &p);

double parallax(const FeaturePairs & features_pairs);

bool matches_dis_cmp(const cv::DMatch &lhs,const cv::DMatch &rhs){
	return lhs.distance < rhs.distance;
}
}
#endif // util.h
