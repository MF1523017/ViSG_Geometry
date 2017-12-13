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
	using Ptr = std::shared_ptr<Camera>;
	Camera(float fx,float fy,float cx,float cy);
	Camera(const Camera &rhs){
		*this = rhs;
	}
	Camera &operator = (const Camera &rhs);
	cv::Mat K;
private:

	float fx_;
	float fy_;
	float cx_;
	float cy_;
};
}


#endif// camera.h
