/*************************************************************************
	> File Name: map_test.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月12日 星期二 10时02分37秒
 ************************************************************************/
#include "map.h"
#include <fstream>
using namespace std;

void savePoints(const string &name,std::vector<cv::Point3d> points){
	ofstream out(name);
	for(auto p: points){
		out << p.x << "," << p.y << "," << p.z << endl;
	}
}

int main(int argc,char **argv){
	// if(argc != 3){
	// 	std::cout << "usage: map_test img_1 img2" << std::endl;
	// 	return 0;
	// }
	const string file_name1("/home/lipei/data/left.png");
	const string file_name2("/home/lipei/data/right.png");
	cv::Mat img1 = cv::imread(file_name1);
	cv::Mat img2 = cv::imread(file_name2);
	cout << "feature extraction " << endl;
	VISG::Feature ff1;
	ff1.Extract(img1);
	auto descriptors1 = ff1.descriptors();
	auto key_points1 = ff1.key_points();
	ff1.Extract(img2);
	auto descriptors2 = ff1.descriptors();
	auto key_points2 = ff1.key_points();

	cout << "feature match " << endl;
	VISG::Matcher mt;
	mt.Match(descriptors1,descriptors2);

	cout << "draw result " << endl;

	VISG::Camera cam(458.654,457.296,367.215,248.375);
	VISG::Pose pose;
	pose.Estimate(key_points1,key_points2,mt.inlier_matches(),cam);
	cout << "[pose] EigenR " << pose.EigenR() << endl;
	cout << "[pose] Eigent " << pose.Eigent().transpose() << endl;
	cout << "[pose] EigenPoseMatrix3_4 " << pose.EigenPoseMatrix3_4() << endl;

	cout << "[inliers]: row: " << inliers.rows << ", col: " << inliers.cols << endl;

	FeaturePairs feature_pairs;
	std::vector<cv::DMatch> good_matches;
	for(size_t i = 0; i < inliers.rows; ++i){
		if(inliers.at<int>(i,0)){
			feature_pairs.push_back(std::make_pair(points1[matches[i].queryIdx].pt,points2[matches[i].trainIdx].pt));
			good_matches.push_back(matches[i]);
		}
	}
	VISG::Map map;
	map.Triangulation(feature_pairs,Pose(),pose,cam);
	const string points_file("/home/lipei/data/points.txt");
	savePoints(points_file,map.map_points());

	cv::Mat img_match, img_good_match;
	cv::drawMatches(img1,ff1.key_points(),img2,ff2.key_points(),matches,img_match);
	cv::drawMatches(img1,ff1.key_points(),img2,ff2.key_points(),good_matches,img_good_match);
	cv::namedWindow("feature match",0);
	cv::namedWindow("inlier feature match",0);
	cv::imshow("feature match" ,img_match);
	cv::imshow("inlier feature match" ,img_good_match);
	cv::waitKey(0);
	return 0;



}
