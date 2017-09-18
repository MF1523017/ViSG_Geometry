/*************************************************************************
	> File Name: ../src/camera.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月11日 星期一 17时35分02秒
 ************************************************************************/
#include "camera.h"
#include <iostream>

namespace VISG{

Camera::Camera(double fx,double fy,double cx,double cy):
	fx_(fx),fy_(fy),cx_(cx),cy_(cy){
		K_ = (cv::Mat_<double>(3,3) <<
				fx_,0,cx_,
				0,fy_,cy_,
				0,0,1);
	}
Camera &Camera::operator = (const Camera &rhs){
	if(this == &rhs)return *this;
	K_ = rhs.K_.clone();
	fx_ = rhs.fx_;
	fy_ = rhs.fy_;
	cx_ = rhs.cx_;
	cy_ = rhs.cy_;
	return *this;
}


}
