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
#include <cassert>

using namespace std;

int main(int argc,char **argv){
	// SO3
	Eigen::Matrix3d R =  Eigen::AngleAxisd(M_PI / 2,Eigen::Vector3d(0,0,1)).toRotationMatrix();
	Sophus::SO3 SO3_R(R);
	Sophus::SO3 SO3_v(0, 0, M_PI / 2);
	Eigen::Quaterniond q(R);
	Sophus::SO3 SO3_q(q);

	cout << "SO(3) from matrix:" << endl << SO3_R << endl;
	cout << "SO(3) from vector:" << endl << SO3_v << endl;
	cout << "SO(3) from quaterniond:" << endl << SO3_q << endl;

	// log
	Eigen::Vector3d so3 = SO3_R.log();
	cout << "so3: " << endl << so3.transpose() << endl;
	// hat
	cout << "so3 hat: " << endl << Sophus::SO3::hat(so3) << endl;
	// vee (从反对称矩阵到向量)
	assert(so3 == Sophus::SO3::vee(Sophus::SO3::hat(so3)));

	// little nums
	Eigen::Vector3d update_so3(1e-4,0,0);
	Sophus::SO3 SO3_updated = Sophus::SO3::exp(update_so3) * SO3_R;// left mult
	cout << "SO3_updated: " << endl << SO3_updated << endl;

	//SE3

	Eigen::Vector3d t(1.0,0,0);
	Sophus::SE3 SE3_Rt(R,t);
	Sophus::SE3 SE3_qt(q,t);
	cout << "SE3_Rt: " << endl << SE3_Rt << endl;
	cout << "SE3_qt: " << endl << SE3_qt << endl;

	using Vector6d = Eigen::Matrix<double,6,1>;
	Vector6d se3 = SE3_Rt.log();
	cout << "se3: " << endl << se3.transpose() << endl;// [translation rotation]

	cout << "se3 hat: " << endl << Sophus::SE3::hat(se3) << endl;
	assert(se3 == Sophus::SE3::vee(Sophus::SE3::hat(se3)));

	Vector6d update_se3;
	update_se3.setZero();
	update_se3(0) = 1e-4;
	Sophus::SE3 SE3_updated = Sophus::SE3::exp(update_se3) * SE3_Rt;
	cout << "SE3_updated: " << endl << SE3_updated << endl;

	return 0;
}
