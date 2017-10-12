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
#include <Eigen/Core>
#include <Eigen/Dense>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <utility>
#include <memory>
#include <cmath>



// #define TEST
// #define USE_ORB
#define USE_SIFT
#ifdef USE_SIFT
#include <opencv2/xfeatures2d.hpp>
#endif

using FeaturePairs = std::vector<std::pair<cv::KeyPoint,cv::KeyPoint>>;
using KeyPoints = std::vector<cv::KeyPoint>;
using DMatches = std::vector<cv::DMatch>;
using PairsIdxPoint3d = std::map<size_t,cv::Point3d>;
// using Pairs2d_3d = std::map<cv::KeyPoint,cv::Point3d>;
using IndexesPairs = std::vector<std::pair<size_t,size_t>>;
using PnP = std::vector<std::pair<cv::Point2f,cv::Point3f>>;//2d_3d

using AllKeyPoints = std::vector<std::vector<cv::KeyPoint>>;
using AllDescriptors = std::vector<cv::Mat>;
using AllMatches = std::vector<std::vector<cv::DMatch>>;
using MapPoints = std::vector<cv::Point3d>;

// using IndexPoint3Pairs

#endif // common.h
