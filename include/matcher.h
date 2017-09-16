/*************************************************************************
	> File Name: matcher.h
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月11日 星期一 10时33分24秒
 ************************************************************************/
#ifndef MATCHER_H
#define MATCHER_H

#include "common.h"
#include <iostream>
#include <vector>

namespace VISG{

	class Matcher{
	public:
		Matcher();
		void match(const cv::Mat &descriptors1,const cv::Mat &descriptors2);
		DMatches matches() const {
			return matches_;
		}
		DMatches inlier_matches() const {
			return inlier_matches_;
		}
	private:
		cv::Ptr<cv::DescriptorMatcher> matcher_;
		DMatches matches_;
		DMatches inlier_matches_;
	};


}

#endif // matcher.h
