/*************************************************************************
	> File Name: geometry.h
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月16日 星期六 13时42分58秒
 ************************************************************************/

#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "common.h"

namespace VISG{

class Geometry{
public:
	virtual void run(cv::Mat &img) = 0;
};


}

#endif //gemotry.h
