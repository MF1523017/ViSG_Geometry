/*************************************************************************
	> File Name: map.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月12日 星期二 09时26分18秒
 ************************************************************************/
#include "map.h"
#include "util.h"

namespace VISG{
	/*
	* @brief triangulation: all inputs should be float type(base)
	*/
void Map::Triangulation(const std::vector<cv::Point2f> &points1,const std::vector<cv::Point2f> &points2,const cv::Mat &T1,const cv::Mat &T2){
	cv::Mat points_4d;
	// cv::Mat points_4d(1,points1.size(),CV_64FC4);
	cv::triangulatePoints(T1,T2,points1,points2,points_4d);
	map_points.clear();// clear old map points;
	// cv::Mat points_4d;
	// points_4d1.convertTo(points_4d,CV_64FC1);
	map_points.reserve(points_4d.cols);
	for(size_t i = 0; i < points_4d.cols; ++i){
		cv::Mat x = points_4d.col(i);
		if(fabs(x.at<float>(3.0)) < 1e-5)
			x.at<float>(3.0) = 1e-5;
		x /= x.at<float>(3,0);
		// std::cout << "[Map::Triangulation] x: " << x << std::endl;
		map_points.push_back(cv::Point3f(x.at<float>(0,0),x.at<float>(1,0),x.at<float>(2,0)));
	}
}
	/*
	* @brief triangulation: all inputs should be float type
	*/
void Map::Triangulation(const FeaturePairs &features_pairs,const Pose &pose1,const Pose &pose2,const Camera &cam){
	if(features_pairs.empty()){
		std::cout << "features_pairs empty " << std::endl;
		return;
	}
	// cv::Mat T1 = (cv::Mat_<double>(3,4) <<
	// 		1,0,0,0,
	// 		0,1,0,0,
	// 		0,0,1,0);
	cv::Mat T1;
	pose1.cvPoseMatrix3_4().convertTo(T1,CV_32FC1);
	cv::Mat T2;
	pose2.cvPoseMatrix3_4().convertTo(T2,CV_32FC1);
	// std::cout << "[triangulation] T2:" << T2 << std::endl;
	cv::Mat K;
	cam.K.convertTo(K,CV_32FC1);
	size_t matches_size = features_pairs.size();
	std::vector<cv::Point2f> points1(matches_size);
	std::vector<cv::Point2f> points2(matches_size);
	for(size_t i = 0; i < matches_size; ++i){
		points1[i] = pixel2cam(features_pairs[i].first.pt,K);
		points2[i] = pixel2cam(features_pairs[i].second.pt,K);
	}
	Triangulation(points1,points2,T1,T2);
}

/*
* @brief triangulation
*/
void Map::Triangulation(const std::vector<cv::Point2f> &points1,const std::vector<cv::Point2f> &points2,
	const Pose &pose1,const Pose &pose2,const Camera &cam){
	cv::Mat T1;
	pose1.cvPoseMatrix3_4().convertTo(T1,CV_32FC1);
	cv::Mat T2;
	pose2.cvPoseMatrix3_4().convertTo(T2,CV_32FC1);
	cv::Mat K;
	cam.K.convertTo(K,CV_32FC1);
	// std::cout << "[Map::Triangulation]: T1 " << std::endl << T1 << std::endl << "T2: " << std::endl << T2 << std::endl << "K: " << std::endl << K << std::endl;
	size_t matches_size = points1.size();
	std::vector<cv::Point2f> points1_cam(matches_size);
	std::vector<cv::Point2f> points2_cam(matches_size);
	for(size_t i = 0; i < matches_size; ++i){
		points1_cam[i] = pixel2cam(points1[i],K);
		points2_cam[i] = pixel2cam(points2[i],K);
	}
	Triangulation(points1_cam,points2_cam,T1,T2);
}

}
