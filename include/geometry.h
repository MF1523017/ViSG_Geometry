/*************************************************************************
	> File Name: geometry.h
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月16日 星期六 13时42分58秒
 ************************************************************************/

#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "common.h"
#include "frame.h"

namespace VISG{

class Geometry{
public:
	virtual void Run(Frame::Ptr p_frame) = 0;
	virtual ~Geometry(){
			std::cout << "[Geometry ~Geometry] GOODBYE Geometry " << std::endl;
	}
};


}

#endif //gemotry.h
