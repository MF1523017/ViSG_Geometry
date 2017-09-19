/*************************************************************************
	> File Name: frame.h
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月19日 星期二 09时21分34秒
 ************************************************************************/

#ifndef FRAME_H
#define FRAME_H

#include "common.h"
#include "feature.h"
#include "pose.h"
#include "map.h"

namespace VISG{

class Frame{
public:
	using Ptr = std::shared_ptr<Frame>;
	Frame(const cv::Mat &img,size_t id,double time_stamp = 0);
	void set_id(const size_t id){
		id_ = id;
	}
	cv::Mat img()const{
		return img_;
	}
	const Pose::Ptr p_pose()const{
		return p_pose_;
	}
	const size_t id()const{
		return id_;
	}
private:
	Pose::Ptr p_pose_;
	cv::Mat img_;
	size_t id_;
	double time_stamp_;
};
}


#endif // frame.h
