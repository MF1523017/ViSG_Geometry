/*************************************************************************
	> File Name: sophusTest.cpp
	> Author: lipei
	> Mail: b111180082@163.com 
	> Created Time: 2017年09月08日 星期五 09时10分39秒
 ************************************************************************/

#include <iostream>
#include <cmath>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <sophus/so3.h>
#include <sophus/se3.h>

using namespace std;

int main(int argc,char **argv){
	Eigen::Matrix3d R =  Eigen::AngleAxisd(M_PI / 2,Eigen::Vector3d(0,0,1)).toRotationMatrix();
	Sophus::SO3 SO3_R(R);
	Sophus::SO3 SO3_v(0, 0, M_PI / 2);
	Eigen::Quaterniond q(R);
	Sophus::SO3 SO3_q(q);
	
	cout << "SO(3) from matrix:" << SO3_R << endl;
	cout << "SO(3) from vector:" << SO3_v << endl;
	cout << "SO(3) from quaterniond:" << SO3_q << endl;
	return 0;
}

