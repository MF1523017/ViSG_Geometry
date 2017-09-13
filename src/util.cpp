/*************************************************************************
	> File Name: ../src/util.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月10日 星期日 09时10分08秒
 ************************************************************************/

#include "util.h"
using namespace std;
using namespace cv;
namespace VISG{

void video2images(const string &video_name,const string &image_dir){
	VideoCapture cap(video_name);
	if(!cap.isOpened()){
		cerr << "Error! video error" << endl;
		return;
	}

	double rate = cap.get(CV_CAP_PROP_FPS);
	namedWindow("video");
	cout << "handle video to images!" << endl;
	Mat frame;
	int i = 0;
	while(true){
		cap.read(frame);
		if(frame.empty()){
			cerr << "Error! frame error!" << endl;
			break;
		}
		imshow("video",frame);
		imwrite(image_dir + to_string(i++) + ".jpg",frame);
		if(waitKey(5) >= 0)
			break;
	}
}


/*
 * @brief: x = K * X ==> K' * x
 * */
cv::Point2d pixel2cam(const cv::Point2d &p, const cv::Mat & K){
	return cv::Point2d(
			(p.x - K.at<double>(0,2)) / K.at<double>(0,0),
			(p.y - K.at<double>(1,2)) / K.at<double>(1,1)
			);
}
/*
 * @brief T = [R|t]
 *
 * */
cv::Mat cvMatrix3_4(const Pose &pose){
	cv::Mat R = pose.R();
	cv::Mat t = pose.t();
	return (cv::Mat_<double>(3,4) <<
			R.at<double>(0,0),R.at<double>(0,1),R.at<double>(0,2),t.at<double>(0,0),
			R.at<double>(1,0),R.at<double>(1,1),R.at<double>(1,2),t.at<double>(1,0),
			R.at<double>(2,0),R.at<double>(2,1),R.at<double>(2,2),t.at<double>(2,0));
}

}
