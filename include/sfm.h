/*************************************************************************
	> File Name: sfm.h
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月16日 星期六 13时37分28秒
 ************************************************************************/

#ifndef SFM_H
#define SFM_H
#include "common.h"
#include "camera.h"
#include "pose.h"
#include "feature.h"
#include "matcher.h"
#include "geometry.h"
namespace VISG{

class SFM:public Geometry{
public:
	enum States{
		INITIALIZATION = 0,
		TRACKING,
		LOST,
	};
	SFM(const Camera &cam);
	~SFM();
	virtual void run(cv::Mat &img);
	void feature_extract(cv::Mat &img);
	void match(const size_t pre_idx,const size_t next_idx,FeaturePairs &features_pairs);
private:
	bool _init(const size_t pre_idx,const size_t next_idx);
	States states_;
	size_t image_count_;
	std::shared_ptr<Camera> p_camera_;
	std::shared_ptr<Pose> p_pose_;
	std::shared_ptr<Feature> p_feature_;
	std::shared_ptr<Matcher> p_matcher_;
	std::vector<KeyPoints> all_key_points_;
	std::vector<cv::Mat> all_descriptors_;
};

}


#endif // sfm.h
