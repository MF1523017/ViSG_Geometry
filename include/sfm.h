/*************************************************************************
	> File Name: sfm.h
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月26日 星期二 16时09分38秒
 ************************************************************************/

#ifndef SFM_H
#define SFM_H
#include "common.h"
#include "geometry.h"
#include "frame.h"
#include "feature.h"
#include "pose.h"

namespace VISG{

class SFM:public Geometry{
public:
	using Ptr = std::shared_ptr<SFM>;
	SFM(const Camera & cam,const size_t images_size);
	~SFM();
	virtual void Run(Frame::Ptr p_frame);
	void ExtractMatch(Frame::Ptr p_frame);
	void InitStructure();
	void MultiView();
	void GetPnP(const size_t idx,std::vector<cv::Point2f> &points2,std::vector<cv::Point3f> &points3);
	void FusionStructure(const size_t idx);
	MapPoints all_map_points()const{
		return all_map_points_;
	}
	// void
private:
	size_t image_count_;
	size_t images_size_;
	Camera::Ptr p_camera_;
	Feature::Ptr p_feature_;
	Matcher::Ptr p_matcher_;
	Pose::Ptr p_pose_;
	Map::Ptr p_map_;
	AllKeyPoints all_key_points_;
	std::vector<AllMatches> all_matches_;// all_matches_[i][j]: the matches between the ith frame and the jth frame
	AllDescriptors all_descriptors_;
	MapPoints all_map_points_; // all map points 3d
	std::vector<std::vector<int>> correspond_idx_;// correspond_idx_[i][j]: the index of map_points_ correspondence the ith frame jth feature
	std::vector<cv::Mat> rotations_;
	std::vector<cv::Mat> translations_;
};
}


#endif // sfm.h
