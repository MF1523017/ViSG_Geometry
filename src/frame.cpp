/*************************************************************************
	> File Name: frame.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月19日 星期二 09时22分15秒
 ************************************************************************/

#include "frame.h"

namespace VISG{
Frame::Frame(const cv::Mat &img,size_t id,double time_stamp):p_pose_(new Pose),
img_(img),id_(id),time_stamp_(time_stamp){
}

}
