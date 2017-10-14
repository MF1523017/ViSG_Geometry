/*************************************************************************
	> File Name: feature_extractor_test.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月13日 星期三 13时43分14秒
 ************************************************************************/

#include "feature_extractor.h"
#include "map.h"
#include "util.h"
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

	const string file_name1("/home/lipei/data/left.jpg");
	const string file_name2("/home/lipei/data/right.jpg");
	cv::Mat img1 = cv::imread(file_name1);
	cv::Mat img2 = cv::imread(file_name2);
//	cv::Mat gray1;
//	cv::Mat gray2;
//	cv::cvtColor(img1,gray1,CV_BGR2GRAY);
//	cv::cvtColor(img2,gray2,CV_BGR2GRAY);
	cout << "feature extraction " << endl;

	VISG::FeatureExtractor extractor;
	extractor.ExtractMatch(img1,img2);
	cout << "draw result " << endl;

	VISG::Camera cam(458.654,457.296,367.215,248.375);
	// VISG::Camera cam(2064.259,2077.247,1009.855,1321.547);

	VISG::Pose pose;
	auto features_pairs =  extractor.features_pairs();
	pose.Estimate(features_pairs,cam);
	VISG::Map map;
	map.Triangulation(features_pairs,cam,pose);
	const string points_file("/home/lipei/data/points.txt");
	savePoints(points_file,map.map_points());
	cout << "[parallax]: " << VISG::parallax(features_pairs) << endl;
	cv::waitKey(0);
	return 0;



}
