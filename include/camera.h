/*************************************************************************
	> File Name: camera.h
	> Author: lipei
	> Mail: b111180082@163.com 
	> Created Time: 2017年09月11日 星期一 17时24分44秒
 ************************************************************************/

#ifndef CAMEAR_H
#define CAMEAR_H
#include "common.h"
namespace VISG{

class Camera{
public:
	Camera(double fx,double fy,double cx,double cy);
	cv::Mat K() const {
		return K_;
	}
private:
	cv::Mat K_;
	double fx_;
	double fy_;
	double cx_;
	double cy_;
};
}


#endif// camera.h
