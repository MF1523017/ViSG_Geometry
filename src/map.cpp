/*************************************************************************
	> File Name: map.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月12日 星期二 09时26分18秒
 ************************************************************************/
#include "map.h"
#include "util.h"

namespace VISG{

void Map::triangulation(const FeaturePairs &features_pairs,const Camera &cam,const Pose &pose1,const Pose &pose2){
	if(features_pairs.empty()){
		std::cout << "features_pairs empty " << std::endl;
		return;
	}
	// cv::Mat T1 = (cv::Mat_<double>(3,4) <<
	// 		1,0,0,0,
	// 		0,1,0,0,
	// 		0,0,1,0);
	cv::Mat T1 = pose1.cvPoseMatrix3_4();
	cv::Mat T2 = pose2.cvPoseMatrix3_4();
	// std::cout << "[triangulation] T2:" << T2 << std::endl;
	cv::Mat K = cam.K();
	size_t matches_size = features_pairs.size();
	std::vector<cv::Point2d> points1(matches_size);
	std::vector<cv::Point2d> points2(matches_size);
	for(size_t i = 0; i < matches_size; ++i){
		points1[i] = pixel2cam(features_pairs[i].first.pt,K);
		points2[i] = pixel2cam(features_pairs[i].second.pt,K);
	}

	cv::Mat points_4d;
	cv::triangulatePoints(T1,T2,points1,points2,points_4d);
	map_points_.clear();// clear old map points;
	for(size_t i = 0; i < points_4d.cols; ++i){
		cv::Mat x = points_4d.col(i);
	//	std::cout << "[triangulation] x:" << x << std::endl;
		x /= x.at<double>(3,0);
		map_points_.push_back(cv::Point3d(x.at<double>(0,0),x.at<double>(1,0),x.at<double>(2,0)));
	}
}

}
