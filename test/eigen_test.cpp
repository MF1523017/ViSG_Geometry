/*************************************************************************
	> File Name: eigenText.cpp
	> Author: lipei
	> Mail: b111180082@163.com 
	> Created Time: 2017年09月07日 星期四 19时11分26秒
 ************************************************************************/

#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

int main(int argc,char **argv){

	Matrix3d m3 = Matrix3d::Zero();
	cout << m3 << endl;
	Matrix4d m4 = Matrix4d::Identity();
	cout << m4 << endl;
	cout << "matrix trace: " << m4.trace() << endl;
	cout << "matrix transpose: " << m4.transpose() << endl;
	return 0;
}

