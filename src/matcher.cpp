/*************************************************************************
	> File Name: ../src/matcher.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月11日 星期一 10时50分08秒
 ************************************************************************/
#include "matcher.h"

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
	void Matcher::Match(const cv::Mat &descriptors1,const cv::Mat &descriptors2){
		if(descriptors1.type() == descriptors2.type() &&
				descriptors1.cols == descriptors2.cols){
			matches_.clear();//clear old match;
			inlier_matches_.clear();
#ifdef USE_ORB
			matcher_->match(descriptors1,descriptors2,matches_);
			auto min_dis = (*(min_element(matches_.begin(),matches_.end(),matches_dis_cmp))).distance;
			// std::cout << "[Matcher ]match::min_dis: " << min_dis << std::endl;
			for(size_t i = 0; i < matches_.size(); ++i){
				if(matches_[i].distance <= std::max(static_cast<double>(2 * min_dis) ,30.0)){
					inlier_matches_.push_back(matches_[i]);
				}
			}
#endif

#ifdef USE_SIFT
			std::vector<std::vector<cv::DMatch>> knn_matches;
			matcher_->knnMatch(descriptors1,descriptors2,knn_matches,2);
			float min_dis = 1e9;
			for(size_t i = 0; i < knn_matches.size(); ++i){
				if(knn_matches[i][0].distance > 0.6 * knn_matches[i][1].distance)
					continue;
				float dis = knn_matches[i][0].distance;
				if(dis < min_dis)
					min_dis = dis;
			}
			for(size_t i = 0; i < knn_matches.size(); ++i){
				matches_.push_back(knn_matches[i][0]);
				if(knn_matches[i][0].distance > 0.6 * knn_matches[i][1].distance||
					knn_matches[i][0].distance > 5 * fmax(min_dis,10.0f))
					continue;
				inlier_matches_.push_back(knn_matches[i][0]);
			}
#endif
		}
	}

}
