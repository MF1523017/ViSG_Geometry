/*************************************************************************
	> File Name: matcher_test.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月10日 星期日 20时03分06秒
 ************************************************************************/

#include "feature.h"
#include "matcher.h"
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
	ff1.extract(img1);
	VISG::Feature ff2;
	ff2.extract(img2);

	cout << "feature match " << endl;
	VISG::Matcher mt;
	mt.match(ff1.descriptors(),ff2.descriptors());

	cout << "draw result " << endl;

	cv::Mat img_match, img_good_match;
	cv::drawMatches(img1,ff1.key_points(),img2,ff2.key_points(),mt.matches(),img_match);
	// cv::drawMatches(img1,ff1.key_points(),img2,ff2.key_points(),mt.inlier_matches(),img_good_match);
	cv::imshow("feature match" ,img_match);
	// cv::imshow("inlier feature match" ,img_good_match);
	cv::waitKey(0);
	return 0;
}
