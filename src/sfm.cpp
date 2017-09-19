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
p_camera_(new Camera(cam)),p_feature_(new Feature),p_matcher_(new Matcher){

}

SFM::~SFM(){
	std::cout << "[goodbye sfm] " << std::endl;
}

bool SFM::_init(){
	FeaturePairs features_pairs;
	match(features_pairs);
	double parallax_ = parallax(features_pairs);
	std::cout << "[SFM init] parallax: " << parallax_ << std::endl;
	if(parallax_ < 20 || parallax_ > 50)
		return false;
	p_frame_cur_->p_pose()->estimate(features_pairs,p_camera_->K());
	return true;
}

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
			;
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
void SFM::match(FeaturePairs &features_pairs){
	p_matcher_->match(descriptors_ref_,descriptors_cur_);
	auto inlier_matches = p_matcher_->inlier_matches();
	features_pairs.resize(inlier_matches.size());
	for(size_t i = 0; i < inlier_matches.size(); ++i){
		features_pairs[i] = std::make_pair(key_points_ref_[inlier_matches[i].queryIdx],key_points_cur_[inlier_matches[i].trainIdx]);
	}
	cv::Mat img_match, img_good_match;
	cv::drawMatches(p_frame_ref_->img(),key_points_ref_,p_frame_cur_->img(),key_points_cur_,p_matcher_->matches(),img_match);
	cv::drawMatches(p_frame_ref_->img(),key_points_ref_,p_frame_cur_->img(),key_points_cur_,p_matcher_->inlier_matches(),img_good_match);
	cv::imshow("feature match" ,img_match);
	cv::imshow("inlier feature match" ,img_good_match);
	cv::waitKey(30);
}

}
