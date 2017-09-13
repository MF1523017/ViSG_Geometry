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

cv::Point2d pixel2cam(const cv::Point2d &p, const cv::Mat & K);

cv::Mat cvMatrix3_4(const Pose &pose);

}
#endif // util.h

