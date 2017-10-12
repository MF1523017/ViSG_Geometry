/*************************************************************************
	> File Name: sfm.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月26日 星期二 16时09分11秒
 ************************************************************************/

#include "sfm.h"
namespace VISG{

SFM::SFM(const Camera & cam,const size_t images_size):image_count_(0),images_size_(images_size),p_camera_(new Camera(cam)),
p_feature_(new Feature),p_matcher_(new Matcher),p_pose_(new Pose),p_map_(new Map){
	std::vector<AllMatches> all_matches(images_size_,AllMatches(images_size_));
	all_matches_ = all_matches;
}
SFM::~SFM(){
	std::cout << "[SFM ~SFM] GOODBYE SFM " << std::endl;
}

void SFM::ExtractMatch(Frame::Ptr p_frame){
	p_feature_->Extract(p_frame->img());
	auto key_points = p_feature_->key_points();
	auto descriptors = p_feature_->descriptors();
	auto id = p_frame->id();
	for(size_t i = 0; i < all_key_points_.size(); ++i){
		p_matcher_->Match(all_descriptors_[i],descriptors);
		auto matches = p_matcher_->inlier_matches();
		std::cout << "[ExtractMatch] frame: " << i << " and frame " << p_frame->id()
							<< " matches size: " << matches.size() << std::endl;
		// all_matches_.push_back(matches);
		all_matches_[i][id] = matches;
	}
	all_key_points_.push_back(key_points);
	all_descriptors_.push_back(descriptors);
}

void SFM::InitStructure(){
	// resize correspond_idx_;
	correspond_idx_.clear();
	correspond_idx_.resize(all_key_points_.size());
	for(size_t i = 0; i < all_key_points_.size(); ++i){
		correspond_idx_[i].resize(all_key_points_[i].size(),-1);
	}

	std::vector<cv::Point2f> points1,points2;
	p_pose_->Estimate(all_key_points_[0],all_key_points_[1],all_matches_[0][1],*p_camera_,points1,points2);
	rotations_.push_back(p_pose_->R());
	translations_.push_back(p_pose_->t());
	p_map_->Triangulation(points1,points2,Pose(),*p_pose_,*p_camera_);
	auto map_points = p_map_->map_points();
	auto matches = all_matches_[0][1];
	std::cout << "map_points.size: " << map_points.size() << " matches size: " << matches.size() << std::endl;
	for(size_t i = 0; i < map_points.size(); ++i){
		all_map_points_.push_back(map_points[i]);
		correspond_idx_[0][matches.queryIdx] = i
	}
}



void SFM::Run(Frame::Ptr p_frame){
	ExtractMatch(p_frame);
	InitStructure();
}



}
