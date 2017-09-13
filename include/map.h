/*************************************************************************
	> File Name: map.h
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月12日 星期二 09时16分28秒
 ************************************************************************/
#ifndef MAP_H
#define MAP_H
#include "common.h"
#include "feature.h"
#include "matcher.h"
#include "pose.h"
#include "camera.h"

namespace VISG{

class Map{


public:
	void triangulation(const FeaturePairs &features_pairs,const Camera &cam,const Pose &pose);

	std::vector<cv::Point3d> map_points() const {
		return map_points_;
	}

private:
	std::vector<cv::Point3d> map_points_;

};

}

#endif //map.h
