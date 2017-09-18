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
p_camera_(new Camera(cam)),p_pose_(new Pose),p_feature_(new Feature),p_matcher_(new Matcher){

}

SFM::~SFM(){
	std::cout << "[goodbye sfm] " << std::endl;
}

bool SFM::_init(const size_t pre_idx,const size_t next_idx){
	FeaturePairs features_pairs;
	match(pre_idx,next_idx,features_pairs);
	double parallax_ = parallax(features_pairs);
	if(parallax_ < 0.5 || parallax_ > 30)
		return false;
	p_pose_->estimate(features_pairs,p_camera_->K());
	return true;
}

void SFM::run(cv::Mat &img){
	if(img.empty()){
		std::cout << "[SFM::run] image error! " << std::endl;
		return;
	}
	feature_extract(img);
	if(states_ == INITIALIZATION){
		std::cout << "[SFM run] initializaton ... " << std::endl;
 		if(all_key_points_.size() > 1 && _init(0,all_key_points_.size() - 1)){
			states_ = TRACKING;
			std::cout << "[SFM run] initialized in " << all_key_points_.size() << " frame! " << std::endl;
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
void SFM::feature_extract(cv::Mat &img){
	p_feature_->extract(img);
	all_key_points_.push_back(p_feature_->key_points());
	all_descriptors_.push_back(p_feature_->descriptors());
}

/*
* @ brief: match the two frame
*/
void SFM::match(const size_t pre_idx,const size_t next_idx,FeaturePairs &features_pairs){
	if(pre_idx == next_idx){
		std::cout << "[SFM::match] index equal " << std::endl;
		return;
	}
	if(pre_idx > all_descriptors_.size() || next_idx > all_descriptors_.size()){
		std::cout << "[SFM::match] index > all_descriptors_.size() " << std::endl;
		return;
	}
	p_matcher_->match(all_descriptors_[pre_idx],all_descriptors_[next_idx]);
	auto inlier_matches = p_matcher_->inlier_matches();
	auto key_points1 = all_key_points_[pre_idx];
	auto key_points2 = all_key_points_[next_idx];
	features_pairs.resize(inlier_matches.size());
	for(size_t i = 0; i < inlier_matches.size(); ++i){
		features_pairs[i] = std::make_pair(key_points1[inlier_matches[i].queryIdx],key_points2[inlier_matches[i].trainIdx]);
	}
}

}
