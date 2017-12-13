/*************************************************************************
	> File Name: ../src/camera.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月11日 星期一 17时35分02秒
 ************************************************************************/
#include "camera.h"
#include <iostream>

namespace VISG{

Camera::Camera(float fx,float fy,float cx,float cy):
	fx_(fx),fy_(fy),cx_(cx),cy_(cy){
		K = (cv::Mat_<float>(3,3) <<
				fx_,0,cx_,
				0,fy_,cy_,
				0,0,1);
	}
Camera &Camera::operator = (const Camera &rhs){
	if(this == &rhs)return *this;
	K = rhs.K.clone();
	fx_ = rhs.fx_;
	fy_ = rhs.fy_;
	cx_ = rhs.cx_;
	cy_ = rhs.cy_;
	return *this;
}


}
