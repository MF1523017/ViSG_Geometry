/*************************************************************************
	> File Name: slam.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月16日 星期六 13时38分37秒
 ************************************************************************/

#include "slam.h"
#include "util.h"

namespace VISG{

SLAM::SLAM(const Camera &cam):states_(INITIALIZATION),image_count_(0),
p_camera_(new Camera(cam)),p_feature_(new Feature),p_matcher_(new Matcher),p_map_(new Map){

}

SLAM::~SLAM(){
	std::cout << "[goodbye SLAM] " << std::endl;
}
/*
* @brief: init from two frame
*/
bool SLAM::Init(){
	FeatureExtract(p_frame_cur_);
	FeaturePairs features_pairs;
	IndexesPairs match_pairs;
	Match(features_pairs,match_pairs);
	double parallax_ = parallax(features_pairs);
	std::cout << "[SLAM init] parallax: " << parallax_ << std::endl;
	if(parallax_ < 20 || parallax_ > 50)
		return false;
	if(!RecoverPose(features_pairs))
		return false;
	RecoverMapPoints(features_pairs,match_pairs);
	return true;
}

/*
* @brief: recover pose from pairs 2d <==> 2d;
*/
bool SLAM::RecoverPose(const FeaturePairs &features_pairs){
	if(features_pairs.size() < 8){
		std::cout << "[recover_pose_2d_2d] features_pairs too little " << std::endl;
		return false;
	}
	auto p_pose_cur_ = p_frame_cur_->p_pose();
	auto p_pose_ref_ = p_frame_ref_->p_pose();
	p_pose_cur_->Estimate(features_pairs,*p_camera_);
	return true;
}

/*
* @brief: recover pose from pairs 2d <==> 3d;
*/

bool SLAM::RecoverPose(const FeaturePairs &features_pairs,const IndexesPairs & match_pairs, FeaturePairs & new_features_pairs){
	if(features_pairs.size() < 8){
		std::cout << "[recover_pose_2d_3d] features_pairs too little " << std::endl;
		return false;
	}
	std::vector<cv::Point2f> points2;
	std::vector<cv::Point3f> points3;
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
	p_pose_cur_->Estimate(points2,points3,*p_camera_);
	return true;
}

/*
* @brief : reconver map points from triangulation
*/
void SLAM::RecoverMapPoints(const FeaturePairs &features_pairs, const IndexesPairs &match_pairs){
	auto p_pose_cur_ = p_frame_cur_->p_pose();
	auto p_pose_ref_ = p_frame_ref_->p_pose();
	p_map_->Triangulation(features_pairs,*p_pose_ref_,*p_pose_cur_,*p_camera_);
	auto points3d = p_map_->map_points();
	for(size_t i = 0; i < features_pairs.size(); ++i){
		pairs_idx_point3d_.insert(std::make_pair(match_pairs[i].first,points3d[i]));
		map_points_.push_back(cv2eigen_Vector3d(points3d[i]));
	}
}

/*
* @brief: update ref frame
*/
void SLAM::UpdateRefFrame(){
	FeatureExtract(p_frame_pre_);
	FeaturePairs features_pairs;
	IndexesPairs match_pairs;
	Match(features_pairs,match_pairs);
	pairs_idx_point3d_.clear();
	auto points3d = p_map_->map_points();
	for(size_t i = 0 ; i < features_pairs.size(); ++i){
		pairs_idx_point3d_.insert(std::make_pair(match_pairs[i].second,points3d[i]));
	}
	std::cout << "[UpdateRefFrame] pre frame id: " << p_frame_pre_->id() << " map points size: " << points3d.size() << std::endl;
	p_frame_ref_ = p_frame_pre_;
	key_points_ref_ = key_points_pre_;
	descriptors_ref_ = descriptors_pre_;
}
/*
* @brief: tracking
*/

bool SLAM::Tracking(){
	FeatureExtract(p_frame_cur_);
	FeaturePairs features_pairs;
	IndexesPairs match_pairs;
	Match(features_pairs,match_pairs);
	double parallax_ = parallax(features_pairs);
	std::cout << "[SLAM tracking] parallax: " << parallax_ << " feature size :" << features_pairs.size() << std::endl;
	if(parallax_ < 20 || features_pairs.size() < 20){
		return false;
	}else if(parallax_ > 50){
		UpdateRefFrame();
	}
	if(features_pairs.size() > 50){
		FeaturePairs new_features_pairs;
		RecoverPose(features_pairs,match_pairs,new_features_pairs);
		RecoverMapPoints(new_features_pairs,match_pairs);
	}else{
		UpdateRefFrame();
	}
	return true;
}

/*
* @brief : run SLAM
*/
void SLAM::Run(Frame::Ptr p_frame){
	std::cout << "[SLAM::Run] cur frame id: " << p_frame->id() << std::endl;
	if(p_frame->img().empty()){
		std::cout << "[SLAM::run] image error! " << std::endl;
		return;
	}
	p_frame_cur_ = p_frame;
	if(states_ == INITIALIZATION){
		if(0 == p_frame->id())
			p_frame_ref_ = p_frame_cur_;
		std::cout << "[SLAM run] initializaton ... " << std::endl;
 		if(Init()){
			states_ = TRACKING;
			std::cout << "[SLAM run] initialized in " << p_frame->id() << " frame! " << std::endl;
			return;
		}
	}else if(states_ == TRACKING){
		if(Tracking())
			p_frame_pre_ = p_frame_cur_;
	}else{
		std::cout << "[SLAM::run] lost error! " << std::endl;
	}
}

/*
* @ brief: extract features from the image
*/
void SLAM::FeatureExtract(Frame::Ptr p_frame){
	p_feature_->Extract(p_frame->img());
	if(p_frame == p_frame_ref_){
		key_points_ref_ = p_feature_->key_points();
		descriptors_ref_ = p_feature_->descriptors();
	}else if(p_frame == p_frame_cur_){
		key_points_cur_ = p_feature_->key_points();
		descriptors_cur_ = p_feature_->descriptors();
	}else if(p_frame == p_frame_pre_){
		key_points_pre_ = p_feature_->key_points();
		descriptors_pre_ = p_feature_->descriptors();
	}
}


/*
* @ brief: match the two frame
*/
void SLAM::Match(FeaturePairs &features_pairs,IndexesPairs & match_pairs){
	p_matcher_->Match(descriptors_ref_,descriptors_cur_);
	auto inlier_matches = p_matcher_->inlier_matches();
	features_pairs.resize(inlier_matches.size());
	match_pairs.resize(inlier_matches.size());
	for(size_t i = 0; i < inlier_matches.size(); ++i){
		features_pairs[i] = std::make_pair(key_points_ref_[inlier_matches[i].queryIdx],key_points_cur_[inlier_matches[i].trainIdx]);
		match_pairs[i] = std::make_pair(inlier_matches[i].queryIdx,inlier_matches[i].trainIdx);
	}
	cv::Mat img_match, img_good_match;
	cv::namedWindow("feature match",0);
	cv::namedWindow("inlier feature match",0);
	cv::drawMatches(p_frame_ref_->img(),key_points_ref_,p_frame_cur_->img(),key_points_cur_,p_matcher_->matches(),img_match);
	cv::drawMatches(p_frame_ref_->img(),key_points_ref_,p_frame_cur_->img(),key_points_cur_,p_matcher_->inlier_matches(),img_good_match);
	cv::imshow("feature match" ,img_match);
	cv::imshow("inlier feature match" ,img_good_match);
	cv::waitKey(30);
}

}
