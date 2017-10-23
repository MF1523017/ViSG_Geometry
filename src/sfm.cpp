/*************************************************************************
	> File Name: sfm.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月26日 星期二 16时09分11秒
 ************************************************************************/

#include "sfm.h"
#include <cassert>
namespace VISG{

SFM::SFM(const Camera & cam,const size_t images_size):image_count_(0),images_size_(images_size),p_camera_(new Camera(cam)),
p_feature_(new Feature),p_matcher_(new Matcher),p_pose_(new Pose),p_map_(new Map){
	// std::vector<AllMatches> all_matches(images_size_,AllMatches(images_size_));
	// all_matches_ = all_matches;
	all_matches_.resize(images_size_,AllMatches(images_size_));
	std::cout << "[SFM] all_matches_ size: " << all_matches_.size() << " all_matches_[0] size: " << all_matches_[0].size() << std::endl;
	rotations_.resize(images_size_);
	translations_.resize(images_size_);
	rotations_[0] = cv::Mat::eye(3,3,CV_64FC1);
	translations_[0] = cv::Mat::zeros(3,1,CV_64FC1);
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
/*
* @brief reconstruc from first two frames
*/
void SFM::InitStructure(){
	// resize correspond_idx_;
	correspond_idx_.clear();
	correspond_idx_.resize(all_key_points_.size());
	for(size_t i = 0; i < all_key_points_.size(); ++i){
		correspond_idx_[i].resize(all_key_points_[i].size(),-1);
	}

	std::vector<cv::Point2f> points1,points2;
	bool ret = p_pose_->Estimate(all_key_points_[0],all_key_points_[1],*p_camera_,all_matches_[0][1],points1,points2);
	assert(ret);
	p_pose_->R().copyTo(rotations_[1]);
	p_pose_->t().copyTo(translations_[1]);
	p_map_->Triangulation(points1,points2,Pose(),*p_pose_,*p_camera_);
	auto map_points = p_map_->map_points();
	auto matches = all_matches_[0][1];
	std::cout << "[InitStructure] map_points.size: " << map_points.size() << " matches size: " << matches.size() << std::endl;
	for(size_t i = 0; i < map_points.size(); ++i){
		all_map_points_.push_back(map_points[i]);
		correspond_idx_[0][matches[i].queryIdx] = i;
		correspond_idx_[1][matches[i].trainIdx] = i;
	}
}

/*
* @brief: get the correspondences between points2 in 2d and points3 in 3d
*/
void SFM::GetPnP(const size_t idx,std::vector<cv::Point2f> &points2,std::vector<cv::Point3f> &points3){
	for(size_t i = 0; i < idx; ++i){
		auto matches = all_matches_[i][idx];
		if(matches.size() < 8){
			std::cout << "[GetPnP] too little matches: " << matches.size() << " idx: " << idx << std::endl;
			continue;
		}
		for(size_t j = 0; j < matches.size(); ++j){
			auto query_idx = matches[j].queryIdx;
			auto train_idx = matches[j].trainIdx;
			auto map_points_idx = correspond_idx_[i][query_idx];
			if(map_points_idx == -1)
				continue;
			points2.push_back(all_key_points_[idx][train_idx].pt);
			points3.push_back(all_map_points_[map_points_idx]);
		}
	}
}

/*
* @brief: fusion the new correspondences to structure
*/
void SFM::FusionStructure(const size_t idx){
	std::vector<cv::Point2f> points1,points2;
	for(size_t i = 0; i < idx; ++i){
		auto matches = all_matches_[i][idx];
		if(matches.size() < 8)
			continue;
		points1.clear();
		points2.clear();
		for(size_t j = 0; j < matches.size(); ++j){
			auto query_idx = matches[j].queryIdx;
			auto train_idx = matches[j].trainIdx;
			auto map_points_idx = correspond_idx_[i][query_idx];
			if(map_points_idx == -1){
				points1.push_back(all_key_points_[i][query_idx].pt);
				points2.push_back(all_key_points_[idx][train_idx].pt);
			}else{
				correspond_idx_[idx][train_idx] = map_points_idx;
			}
		}
		// std::cout << "[FusionStructure] correspondences ize: " << points1.size() << std::endl;
		p_map_->Triangulation(points1,points2,Pose(rotations_[i],translations_[i]),Pose(rotations_[idx],translations_[idx]),*p_camera_);
		size_t map_points_size = all_map_points_.size();
		auto new_map_points = p_map_->map_points();
		// std::cout << "[FusionStructure] new map points size: " << new_map_points.size() << " i: " << i << " <==> idx: " << idx << std::endl;
		for(size_t k = 0; k < new_map_points.size(); ++k){
			// std::cout << "[FusionStructure] points1: " << points1[k] << " points2: " << points2[k] << std::endl;
			// std::cout << "[FusionStructure] new map points: " << new_map_points[k] << std::endl;
			all_map_points_.push_back(new_map_points[k]);
			correspond_idx_[i][matches[k].queryIdx] = map_points_size + k;
			correspond_idx_[idx][matches[k].trainIdx] = map_points_size + k;
		}
	}
}
/*
* @brief: MultiView: structure from motion
*/
void SFM::MultiView(){
#ifdef TEST
	std::cout << "[MultiView] all_matches_: " << std::endl;
	for(size_t i = 0; i < all_matches_.size(); ++i){
		for(size_t j = 0; j < all_matches_[0].size(); ++j){
			std::cout << all_matches_[i][j].size() << " ";
		}
		std::cout << std::endl;
	}
#endif

	std::vector<cv::Point2f> points2;
	std::vector<cv::Point3f> points3;
	for(size_t i = 2; i < all_matches_.size(); ++i){
		points2.clear();
		points3.clear();
		GetPnP(i,points2,points3);
		std::cout << "[MultiView] pnp size: " << points3.size() << std::endl;
		bool ret = p_pose_->Estimate(points2,points3,*p_camera_);
		assert(ret);
		p_pose_->R().copyTo(rotations_[i]);
		p_pose_->t().copyTo(translations_[i]);
		std::cout << "[MultiView] ith: " << i << "  R: " << rotations_[i] << std::endl
							<< " t: " << translations_[i] << std::endl;
 		FusionStructure(i);
	}
}

void SFM::Run(Frame::Ptr p_frame){
	ExtractMatch(p_frame);
	InitStructure();
}



}
