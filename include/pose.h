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
	void estimate(const std::vector<cv::KeyPoint> &key_points1,const std::vector<cv::KeyPoint> &key_points2, const std::vector<cv::DMatch> &matches, const Camera &cam);

	cv::Mat R()const {
		return R_;
	}
	cv::Mat t()const {
		return t_;
	}

private:
	cv::Mat R_;
	cv::Mat t_;
};


}

#endif// pose.h

