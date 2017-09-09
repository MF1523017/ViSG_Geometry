/*************************************************************************
	> File Name: opencvTest.cpp
	> Author: lipei
	> Mail: b111180082@163.com 
	> Created Time: 2017年09月08日 星期五 16时11分44秒
 ************************************************************************/

#include <iostream>
#include <chrono>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc,char **argv){
	if( argc != 2){
		cout << "usage: " << argv[0] << " image_file " << endl;
		return 0;
	}

	Mat image = imread(argv[1]);
	if ( image.empty() ){
		cerr << "image file : " << argv[1] << " is not exist " << endl;
		return 0;
	}
	
	cout << "image row: " << image.rows << " ,image col: " << image.cols 
		<< ", image channels: " << image.channels() << endl;
	imshow("image",image);
	waitKey(0);

	if(image.type() != CV_8UC1 && image.type() != CV_8UC3){
		cout << "image type error" << endl;
		return 0;
	}

	chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
	for(size_t i = 0; i < image.rows; ++i){
		for (size_t j = 0; j < image.cols; ++j){
			unsigned char * row_ptr = image.ptr<unsigned char> (i);
			unsigned char * data_ptr =  &row_ptr[j * image.channels()];
			for(int c = 0; c < image.channels(); ++c){
				unsigned char data = data_ptr[c];
			}
		}
		cout << endl;
	}
	chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
	chrono::duration<double> time_used =  chrono::duration_cast<chrono::duration<double>>(t2-t1);
	cout << "time used: " << time_used.count() << endl;

	// assign will not copy data
	Mat image_other = image;
	image_other (Rect(0,0,100,100)).setTo(0);
	imshow("image",image);
	waitKey(0);

	// use clone will copy data
	Mat image_clone = image.clone();
	image_clone (Rect(0,0,100,100)).setTo(0);
	imshow("image",image);
	imshow("image_clone",image_clone);
	waitKey(0);
	destroyAllWindows();
	return 0;
}

