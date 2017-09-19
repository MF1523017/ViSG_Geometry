/*************************************************************************
	> File Name: ../src/matcher.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月11日 星期一 10时50分08秒
 ************************************************************************/
#include "matcher.h"
#include "util.h"

namespace VISG{

	Matcher::Matcher(){
#ifdef USE_ORB
		matcher_ = cv::DescriptorMatcher::create("BruteForce-Hamming");
#endif
#ifdef USE_SIFT
		matcher_ = cv::DescriptorMatcher::create("BruteForce");
#endif
	}
	//match
	void Matcher::match(const cv::Mat &descriptors1,const cv::Mat &descriptors2){
		if(descriptors1.type() == descriptors2.type() &&
				descriptors1.cols == descriptors2.cols){
			matches_.clear();//clear old match;
			inlier_matches_.clear();
			matcher_->match(descriptors1,descriptors2,matches_);
			auto min_dis = (*(min_element(matches_.begin(),matches_.end(),matches_dis_cmp))).distance;
			// std::cout << "[Matcher ]match::min_dis: " << min_dis << std::endl;
			for(size_t i = 0; i < matches_.size(); ++i){
				if(matches_[i].distance <= std::max(static_cast<double>(2 * min_dis) ,30.0)){
					inlier_matches_.push_back(matches_[i]);
				}
			}
		}
	}
}
