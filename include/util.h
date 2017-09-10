/*************************************************************************
	> File Name: util.h
	> Author: lipei
	> Mail: b111180082@163.com 
	> Created Time: 2017年09月10日 星期日 09时00分41秒
 ************************************************************************/
#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

namespace VISG{
void video2images(const std::string& video_name,const std::string &image_dir);
}
#endif // util.h

