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

	// VISG::Camera cam(458.654,457.296,367.215,248.375);
	VISG::Camera cam(2759.48,2764.16,1520.69,1006.81);
	VISG::Pose pose;
	std::vector<cv::Point2f> points1,points2;
	auto matches = mt.inlier_matches();
	bool ret = pose.Estimate(key_points1,key_points2,cam,matches,points1,points2);
	if(!ret)
		cout << "[Pose] error " << endl;
	// cout << "[pose] EigenR " << pose.EigenR() << endl;
	// cout << "[pose] Eigent " << pose.Eigent().transpose() << endl;
	// cout << "[pose] EigenPoseMatrix3_4 " << pose.EigenPoseMatrix3_4() << endl;

	VISG::Map map;
	map.Triangulation(points1,points2,VISG::Pose(),pose,cam);
	const string points_file("/home/lipei/data/points.txt");
	savePoints(points_file,map.map_points());

	cv::Mat img_match, img_good_match;
	cv::drawMatches(img1,key_points1,img2,key_points2,mt.matches(),img_match);
	cv::drawMatches(img1,key_points1,img2,key_points2,matches,img_good_match);
	cv::namedWindow("feature match",0);
	cv::namedWindow("inlier feature match",0);
	cv::imshow("feature match" ,img_match);
	cv::imshow("inlier feature match" ,img_good_match);
	cv::waitKey(0);
	return 0;



}
