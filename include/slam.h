/*************************************************************************
	> File Name: SLAM.h
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月16日 星期六 13时37分28秒
 ************************************************************************/

#ifndef SLAM_H
#define SLAM_H
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

class SLAM:public Geometry{
public:
	enum States{
		INITIALIZATION = 0,
		TRACKING,
		LOST,
	};
	SLAM(const Camera &cam);
	~SLAM();
	virtual void Run(Frame::Ptr p_frame);
	void FeatureExtract(Frame::Ptr p_frame);
	void Match(FeaturePairs &features_pairs,IndexesPairs & match_pairs);
	bool RecoverPose(FeaturePairs &features_pairs);
	bool RecoverPose(const FeaturePairs &features_pairs,const IndexesPairs & match_pairs,FeaturePairs &new_features_pairs);
	void RecoverMapPoints(const FeaturePairs &features_pairs,const IndexesPairs & match_pairs);
	bool Tracking();
	bool Init();
	void UpdateRefFrame();
	std::vector<Eigen::Vector3d> map_points()const{
		return map_points_;
	}
private:

	States states_;
	size_t image_count_;
	Camera::Ptr p_camera_;
	Feature::Ptr p_feature_;
	Matcher::Ptr p_matcher_;
	Frame::Ptr p_frame_ref_;
	Frame::Ptr p_frame_cur_;
	Frame::Ptr p_frame_pre_;
	Map::Ptr p_map_;
	KeyPoints key_points_ref_;
	KeyPoints key_points_cur_;
	KeyPoints key_points_pre_;
	cv::Mat descriptors_ref_;
	cv::Mat descriptors_cur_;
	cv::Mat descriptors_pre_;
	// Pairs2d_3d pairs2_3_;
	PairsIdxPoint3d pairs_idx_point3d_;
	std::vector<Eigen::Vector3d> map_points_;
	// std::vector<KeyPoints> all_key_points_;
	// std::vector<cv::Mat> all_descriptors_;
};

}


#endif // SLAM.h
