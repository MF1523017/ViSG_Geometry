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
		using Ptr = std::shared_ptr<Matcher>;
		Matcher();
		void Match(const cv::Mat &descriptors1,const cv::Mat &descriptors2);
	private:
		cv::Ptr<cv::DescriptorMatcher> matcher_;
	public:
		DMatches matches;
		DMatches inlier_matches;
	};


}

#endif // matcher.h
