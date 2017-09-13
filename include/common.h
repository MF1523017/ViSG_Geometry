/*************************************************************************
	> File Name: common.h
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月10日 星期日 18时47分40秒
 ************************************************************************/
#ifndef COMMON_H
#define COMMON_H
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>
#include <vector>
#include <utility>



#define USE_ORB
// #define USE_SIFT
#ifdef USE_SIFT
#include <opencv2/xfeatures2d.hpp>
#endif

using FeaturePairs = std::vector<std::pair<cv::Point2d,cv::Point2d>>;

#endif // common.h
