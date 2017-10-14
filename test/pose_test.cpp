/*************************************************************************
	> File Name: pose_test.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月10日 星期日 20时03分06秒
 ************************************************************************/

#include "feature.h"
#include "matcher.h"
#include "pose.h"
#include <iostream>
using namespace std;

int main(int argc,char **argv){
	const string file_name1("/home/lipei/data/left.png");
	const string file_name2("/home/lipei/data/right.png");
	cv::Mat img1 = cv::imread(file_name1);
	cv::Mat img2 = cv::imread(file_name2);
//	cv::Mat gray1;
//	cv::Mat gray2;
//	cv::cvtColor(img1,gray1,CV_BGR2GRAY);
//	cv::cvtColor(img2,gray2,CV_BGR2GRAY);
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
	cv::Mat img_match, img_good_match;
	cv::drawMatches(img1,ff1.key_points(),img2,ff2.key_points(),mt.matches(),img_match);
	cv::drawMatches(img1,ff1.key_points(),img2,ff2.key_points(),mt.inlier_matches(),img_good_match);
	cv::imshow("feature match" ,img_match);
	cv::imshow("inlier feature match" ,img_good_match);
	cv::waitKey(0);
	return 0;
}
