/*************************************************************************
	> File Name: fast_feature_test.cpp
	> Author: lipei
	> Mail: b111180082@163.com 
	> Created Time: 2017年09月10日 星期日 20时03分06秒
 ************************************************************************/

#include "feature.h"
#include <iostream>
using namespace std;

int main(int argc,char **argv){
	const string file_name("/home/lipei/data/arya.png");
	cv::Mat img = cv::imread(file_name);
	VISG::Feature ff;
	ff.Extract(img);
	return 0;
}
