/*************************************************************************
	> File Name: video2imagesTest.cpp
	> Author: lipei
	> Mail: b111180082@163.com 
	> Created Time: 2017年09月10日 星期日 09时22分04秒
 ************************************************************************/
#include "util.h"

using namespace std;

int main(int argc,char **argv){
	cout << "Test util video2images !" << endl;
	const string video_name("/home/lipei/data/calibration.mp4");
	const string images_dir("/home/lipei/data/calib_images/");
	VISG::video2images(video_name,images_dir);
	return 0;
}

