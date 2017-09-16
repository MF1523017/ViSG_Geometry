/*************************************************************************
	> File Name: ../src/sfm.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月16日 星期六 13时38分37秒
 ************************************************************************/

#include "sfm.h"

namespace VISG{

SFM::SFM():image_count_(0),p_feature_(new Feature),p_matcher_(new Matcher){
	states_ = INITIALIZATION;
}

SFM::~SFM(){
	std::cout << "[goodbye sfm] " << std::endl;
}

void SFM::run(cv::Mat &img){
	if(img.empty()){
		std::cout << "[SFM::run] image error! " << std::endl;
		return;
	}
	if(states_ == INITIALIZATION){
		if(all_key_points_.empty()){
			feature_extract(img);
		}else{
			feature_extract(img);
			match(0,1);
		}
		states_ = TRACKING;
		return;
	}else if(states_ == TRACKING){
		feature_extract(img);
	}else{
		std::cout << "[SFM::run] lost error! " << std::endl;
		return;
	}
}

void SFM::feature_extract(cv::Mat &img){
	p_feature_->extract(img);
	all_key_points_.push_back(p_feature_->key_points());
	all_descriptors_.push_back(p_feature_->descriptors());
}

void SFM::match(size_t pre_idx,size_t next_idx){
	if(pre_idx == next_idx){
		std::cout << "[SFM::match] index equal " << std::endl;
		return;
	}
	if(pre_idx > all_descriptors_.size() || next_idx > all_descriptors_.size()){
		std::cout << "[SFM::match] index > all_descriptors_.size() " << std::endl;
		return;
	}
	p_matcher_->match(all_descriptors_[pre_idx],all_descriptors_[next_idx]);
}

}
