/*************************************************************************
	> File Name: pipeline_test.cpp
	> Author: lipei
	> Mail: b111180082@163.com
	> Created Time: 2017年09月14日 星期四 10时48分25秒
 ************************************************************************/

 #include "feature_extractor.h"
 #include "map.h"
 #include "util.h"
 #include <fstream>
 using namespace std;

 #define SKIP_FRAMES 10

 void savePoints(const string &name,std::vector<cv::Point3d> points){
 	ofstream out(name);
 	for(auto p: points){
 		out << p.x << "," << p.y << "," << p.z << endl;
 	}
 }

 int main(int argc,char **argv){
 	// if(argc != 3){
 	// 	std::cout << "usage: map_test img_1 img2" << std::endl;
 	// 	return 0;
 	// }
 	const string file_dir("/home/lipei/data/cam0");
 	const string data_file(file_dir + "/data.csv");
 	ifstream read_images(data_file);
	string line;
	vector<string> images;
	while(read_images && !read_images.eof()){
		getline(read_images,line);
		if('#' == line[0])
			continue;
		size_t pos = line.find(",");
		auto image_name =  line.substr(pos + 1,line.size()-pos-2);
		if(image_name.empty())
			break;
		// cout << "[image_name]: " << image_name << endl;
		images.push_back(image_name);
	}
	read_images.close();


	VISG::Camera cam(458.654,457.296,367.215,248.375);
 	// VISG::Camera cam(2064.259,2077.247,1009.855,1321.547);
	const string image_dir(file_dir + "/data/");
#ifdef SKIP_FRAMES
	for(size_t i = 0; i < images.size() - SKIP_FRAMES ; i = i + SKIP_FRAMES){
		const string file_name1(image_dir + images[i]);
	 	const string file_name2(image_dir + images[i+SKIP_FRAMES]);
#else
	for(size_t i = 0; i < images.size() - 1 ; ++i){
		const string file_name1(image_dir + images[i]);
	 	const string file_name2(image_dir + images[i+1]);
#endif
	 	cv::Mat img1 = cv::imread(file_name1);
	 	cv::Mat img2 = cv::imread(file_name2);
		if(img1.empty() || img2.empty()){
			cout << "error" << endl;
			return -1;
		}

	 	cout << "feature extraction " << endl;

	 	VISG::FeatureExtractor extractor;
	 	auto feature_result = extractor.ExtractMatch(img1,img2);
	 	cout << "draw result " << endl;


	 	VISG::Pose pose;
	 	auto features_pairs =  extractor.features_pairs();
	 	pose.Estimate(features_pairs,cam);
	 	VISG::Map map;
	 	map.Triangulation(features_pairs,Pose(),pose,cam);
		// 	const string points_file("/home/lipei/data/points.txt");
		// 	savePoints(points_file,map.map_points());
	 	cout << "[parallax]: " << VISG::parallax(features_pairs) << endl;
	}
 	return 0;



 }
