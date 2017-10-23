/*************************************************************************
	> File Name: pose_test.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月10日 星期日 20时03分06秒
 ************************************************************************/

#include "feature.h"
#include "matcher.h"
#include "pose.h"
#include "util.h"
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

	// VISG::Camera cam(458.654,457.296,367.215,248.375);
	VISG::Camera cam(2759.48,2764.16,1520.69,1006.81);
	VISG::Pose pose;
	std::vector<cv::Point2f> points1,points2;
	auto matches = mt.inlier_matches();
	bool ret = pose.Estimate(key_points1,key_points2,cam,matches,points1,points2);
	if(!ret)
		cout << "[Pose] error " << endl;
	cout << "[Pose] cvPoseMatrix3_4: " << pose.cvPoseMatrix3_4() << endl;
	cout << "[pose] EigenR " << pose.EigenR() << endl;
	cout << "[pose] Eigent " << pose.Eigent().transpose() << endl;
	cout << "[pose] EigenPoseMatrix3_4 " << pose.EigenPoseMatrix3_4() << endl;
	cv::Mat img_match, img_good_match;
	cv::drawMatches(img1,key_points1,img2,key_points2,mt.matches(),img_match);
	cv::drawMatches(img1,key_points1,img2,key_points2,matches,img_good_match);
	cv::namedWindow("feature match",0);
	cv::namedWindow("inlier feature match",0);
	cv::imshow("feature match" ,img_match);
	cv::imshow("inlier feature match" ,img_good_match);
	cv::waitKey(0);
	cv::Mat img_1 = cv::imread ("/home/lipei/slambook/ch7/1.png" , CV_LOAD_IMAGE_COLOR );
  cv::Mat img_2 = cv::imread ( "/home/lipei/slambook/ch7/2.png", CV_LOAD_IMAGE_COLOR );
	ff1.Extract(img_1);
	descriptors1 = ff1.descriptors();
	key_points1 = ff1.key_points();
	ff1.Extract(img_2);
	descriptors2 = ff1.descriptors();
	key_points2 = ff1.key_points();
	cout << "feature match " << endl;
	mt.Match(descriptors1,descriptors2);
 	matches = mt.inlier_matches();
  cout<<"一共找到了"<<matches.size() <<"组匹配点"<<endl;

    // 建立3D点
    cv::Mat d1 = cv::imread ( "/home/lipei/slambook/ch7/1_depth.png", CV_LOAD_IMAGE_UNCHANGED );       // 深度图为16位无符号数，单通道图像
    VISG::Camera cam1(520.9, 521.0,325.1, 249.7);
    vector<cv::Point3f> pts_3d;
    vector<cv::Point2f> pts_2d;
    for ( auto m:matches )
    {
        ushort d = d1.ptr<unsigned short> (int ( key_points1[m.queryIdx].pt.y )) [ int ( key_points1[m.queryIdx].pt.x ) ];
        if ( d == 0 )   // bad depth
            continue;
        float dd = d/1000.0;
        cv::Point2f p1 = VISG::pixel2cam( key_points1[m.queryIdx].pt, cam1.K() );
				std::cout << "[p1]: " << p1 << std::endl;

				std::cout << "[dd]: " << dd << std::endl;
        pts_3d.push_back ( cv::Point3f ( p1.x*dd, p1.y*dd, dd ) );
				std::cout << "[pts3d]: " << pts_3d.back() << std::endl;
        pts_2d.push_back ( key_points2[m.trainIdx].pt );
    }

    cout<<"3d-2d pairs: "<<pts_3d.size() <<endl;
		pose.Estimate(pts_2d,pts_3d,cam1);
    // Mat r, t;
    // solvePnP ( pts_3d, pts_2d, K, Mat(), r, t, false ); // 调用OpenCV 的 PnP 求解，可选择EPNP，DLS等方法
    // Mat R;
    // cv::Rodrigues ( r, R ); // r为旋转向量形式，用Rodrigues公式转换为矩阵

    cout<<"R="<<endl<<pose.R()<<endl;
    cout<<"t="<<endl<<pose.t()<<endl;
	return 0;
}
