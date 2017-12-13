/*************************************************************************
	> File Name: feature.h
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月11日 星期一 09时57分23秒
 ************************************************************************/
#ifndef FEATURE_H
#define FEATURE_H

#include "common.h"

namespace VISG{

	class Feature{
		public:
			Feature();
			using Ptr = std::shared_ptr<Feature>;
			void Extract(const cv::Mat &img);
		public:
			KeyPoints key_points;
			cv::Mat descriptors;
		private:
			cv::Ptr<cv::FeatureDetector> detector_;
	};

}

#endif // Feature.h
