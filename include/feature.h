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
			Feature() = default;
			void extract(cv::Mat &img);
			std::vector<cv::KeyPoint> key_points()const {
				return key_points_;
			}
			cv::Mat descriptors() const {
				return descriptors_;
			}

		private:
			std::vector<cv::KeyPoint> key_points_;
			cv::Mat descriptors_;


	};

}

#endif // Feature.h
