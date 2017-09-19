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
#include "frame.h"
#include "map.h"
#include "geometry.h"
#include "util.h"
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
	virtual void run(Frame::Ptr p_frame);
	void feature_extract(Frame::Ptr p_frame);
	IndexesPairs match(FeaturePairs &features_pairs);
	bool tracking();
	std::vector<Eigen::Vector3d> map_points()const{
		return map_points_;
	}
private:
	bool _init();
	States states_;
	size_t image_count_;
	Camera::Ptr p_camera_;
	Feature::Ptr p_feature_;
	Matcher::Ptr p_matcher_;
	Frame::Ptr p_frame_ref_;
	Frame::Ptr p_frame_cur_;
	Map::Ptr p_map_;
	KeyPoints key_points_ref_;
	KeyPoints key_points_cur_;
	cv::Mat descriptors_ref_;
	cv::Mat descriptors_cur_;
	// Pairs2d_3d pairs2_3_;
	PairsIdxPoint3d pairs_idx_point3d_;
	std::vector<Eigen::Vector3d> map_points_;
	// std::vector<KeyPoints> all_key_points_;
	// std::vector<cv::Mat> all_descriptors_;
};

}


#endif // sfm.h
