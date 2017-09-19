/*************************************************************************
	> File Name: ../src/sfm.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月16日 星期六 13时38分37秒
 ************************************************************************/

#include "sfm.h"
#include "util.h"

namespace VISG{

SFM::SFM(const Camera &cam):states_(INITIALIZATION),image_count_(0),
p_camera_(new Camera(cam)),p_feature_(new Feature),p_matcher_(new Matcher),p_map_(new Map){

}

SFM::~SFM(){
	std::cout << "[goodbye sfm] " << std::endl;
}
/*
* @brief: init from two frame
*/
bool SFM::_init(){
	FeaturePairs features_pairs;
	auto match_pairs = match(features_pairs);
	double parallax_ = parallax(features_pairs);
	std::cout << "[SFM init] parallax: " << parallax_ << std::endl;
	if(parallax_ < 20 || parallax_ > 50)
		return false;
	auto p_pose_cur_ = p_frame_cur_->p_pose();
	auto p_pose_ref_ = p_frame_ref_->p_pose();
	p_pose_cur_->estimate(features_pairs,*p_camera_);
	p_map_->triangulation(features_pairs,*p_camera_,*p_pose_ref_,*p_pose_cur_);
	auto points3d = p_map_->map_points();
	for(size_t i = 0; i < features_pairs.size(); ++i){
		pairs_idx_point3d_.insert(std::make_pair(match_pairs[i].first,points3d[i]));
		// pairs2_3_[features_pairs[i].first] = points3d[i];
		map_points_.push_back(cv2eigen_Vector3d(points3d[i]));
	}
	return true;
}

/*
* @brief: tracking
*/

bool SFM::tracking(){
	FeaturePairs features_pairs;
	auto match_pairs = match(features_pairs);
	double parallax_ = parallax(features_pairs);
	std::cout << "[SFM tracking] parallax: " << parallax_ << " feature size :" << features_pairs.size() << std::endl;
	if(parallax_ < 20){
		return false;
	}else if(parallax_ > 50){
		p_frame_ref_ = p_frame_cur_;
		return false;
	}
	// select 2d--3d
	if(features_pairs.size() > 50){
		std::vector<cv::Point2f> points2;
		std::vector<cv::Point3f> points3;
		FeaturePairs new_features_pairs;
		for(size_t i = 0; i < features_pairs.size(); ++i){
			size_t idx = match_pairs[i].first;
			auto it = pairs_idx_point3d_.find(idx);
			if(it != pairs_idx_point3d_.end()){
				points2.push_back(key_points_ref_[idx].pt);
				points3.push_back(cvPointd2f(pairs_idx_point3d_[idx]));
			}else{
				new_features_pairs.push_back(features_pairs[i]);
			}
		}
		auto p_pose_cur_ = p_frame_cur_->p_pose();
		auto p_pose_ref_ = p_frame_ref_->p_pose();
		p_pose_cur_->estimate(points2,points3,*p_camera_);
		p_map_->triangulation(new_features_pairs,*p_camera_,*p_pose_ref_,*p_pose_cur_);
		auto new_points3d = p_map_->map_points();
		for(size_t i = 0; i < new_features_pairs.size(); ++i){
			pairs_idx_point3d_.insert(std::make_pair(match_pairs[i].first,new_points3d[i]));
			map_points_.push_back(cv2eigen_Vector3d(new_points3d[i]));
		}
	}else{
		p_frame_ref_ = p_frame_cur_;
	}
	return true;
}

/*
* @brief : run sfm
*/
void SFM::run(Frame::Ptr p_frame){
	if(p_frame->img().empty()){
		std::cout << "[SFM::run] image error! " << std::endl;
		return;
	}
	if(states_ == INITIALIZATION){
		if(0 == p_frame->id()){
			p_frame_cur_ = p_frame_ref_ = p_frame;
		}else{
			p_frame_cur_ = p_frame;
		}
		feature_extract(p_frame);
		std::cout << "[SFM run] initializaton ... " << std::endl;
 		if(_init()){
			states_ = TRACKING;
			std::cout << "[SFM run] initialized in " << p_frame->id() << " frame! " << std::endl;
			return;
		}
	}else if(states_ == TRACKING){
		p_frame_cur_ = p_frame;
		feature_extract(p_frame);
		tracking();
		if(p_frame_cur_ == p_frame_ref_){
			feature_extract(p_frame);
		}
	}else{
		std::cout << "[SFM::run] lost error! " << std::endl;
		return;
	}
}

/*
* @ brief: extract features from the image
*/
void SFM::feature_extract(Frame::Ptr p_frame){
	p_feature_->extract(p_frame->img());
	if(p_frame == p_frame_ref_){
		key_points_ref_ = p_feature_->key_points();
		descriptors_ref_ = p_feature_->descriptors();
	}else if(p_frame == p_frame_cur_){
		key_points_cur_ = p_feature_->key_points();
		descriptors_cur_ = p_feature_->descriptors();
}
}

/*
* @ brief: match the two frame
*/
IndexesPairs SFM::match(FeaturePairs &features_pairs){
	IndexesPairs match_pairs;
	p_matcher_->match(descriptors_ref_,descriptors_cur_);
	auto inlier_matches = p_matcher_->inlier_matches();
	features_pairs.resize(inlier_matches.size());
	match_pairs.resize(inlier_matches.size());
	for(size_t i = 0; i < inlier_matches.size(); ++i){
		features_pairs[i] = std::make_pair(key_points_ref_[inlier_matches[i].queryIdx],key_points_cur_[inlier_matches[i].trainIdx]);
		match_pairs[i] = std::make_pair(inlier_matches[i].queryIdx,inlier_matches[i].trainIdx);
	}
	cv::Mat img_match, img_good_match;
	cv::drawMatches(p_frame_ref_->img(),key_points_ref_,p_frame_cur_->img(),key_points_cur_,p_matcher_->matches(),img_match);
	cv::drawMatches(p_frame_ref_->img(),key_points_ref_,p_frame_cur_->img(),key_points_cur_,p_matcher_->inlier_matches(),img_good_match);
	cv::imshow("feature match" ,img_match);
	cv::imshow("inlier feature match" ,img_good_match);
	cv::waitKey(30);
	return match_pairs;
}

}
