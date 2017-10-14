#include <iostream>
#include <cmath>
#include <string>
#include <fstream>

#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

double norm(double x, double w, double min, double max) {
	return x / w * (max - min) + min;
}

int update(string file, double x, double dx, double y, double dy, string file_out, double max, double min, int orint) {
	ofstream fout(file_out);
    VideoCapture cap(file);
    if(!cap.isOpened())
        return -1;


    Mat frame;
    Mat crop;
    Mat grey;
    Mat bin;
    Mat thr;
    int nframes = cap.get(CV_CAP_PROP_FRAME_COUNT);
    cout << nframes << " frames" << endl;
    int i = 0;

    VideoWriter video("out.avi", CV_FOURCC('M','P','4','2'), 30, Size(cap.get(CV_CAP_PROP_FRAME_WIDTH) * 2 * dx, cap.get(CV_CAP_PROP_FRAME_HEIGHT) * 2 * dy), true);
    bool flag = true;
    Mat bg;
    Mat newbg;

    Point center;
    Scalar color(0, 0, 255);
    while(i < nframes)
    {
    	if (i / (nframes / 100) == 100)
    		break;
    	if (++i % (nframes / 100) == 0)
    		cout << "(" << i / (nframes / 100) << "%) " << i << "/" << nframes << endl;
        
        cap >> frame;
        crop = frame(Rect((x - dx) * frame.cols, (y - dy) * frame.rows, 2 * dx * frame.cols, 2 * dy * frame.rows));
        
        for (int x_ = 0; x_ < crop.cols; x_++)
        	for (int y_ = 0; y_ < crop.rows; y_++) {
        		//crop.at<uchar>(y_, x_) = 128;
        	}


        cvtColor (crop, grey, CV_BGR2GRAY);
        //imshow("gs", grey);

        if (i % 5 == 1) {
        	
        	if (i == 1)
        		bg = grey.clone();
        	else
    	    	bg = newbg;
        	newbg = grey.clone();
        	
        	//imshow("bg0", bg);
        	//imshow("bg1", newbg);
        	//bg = grey.clone();

        	flag = false;
        }
        grey = bg - grey;
        //grey *= 2;
        threshold(grey, thr, 100, 255, THRESH_BINARY);
        //imshow("gr1", thr);
        //imshow("bin", thr);
        int largest_area = 0;
        int largest_contour_index = 0;
        Rect bounding_rect;

        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(thr, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
        for (int i = 0; i < contours.size(); i++ ) {
       		double a = contourArea(contours[i], false);  //  Find the area of contour
       		if(a > largest_area){
       			largest_area = a;
       			largest_contour_index = i;                //Store the index of largest contour
       			bounding_rect = boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
       		}
		}
		
		center = (bounding_rect.br() + bounding_rect.tl()) / 2;
		if (orint == 0)
			fout << i << "\t" << norm(center.x, thr.cols, min, max) << endl;
		else
			fout << i << "\t" << norm(center.y, thr.rows, min, max) << endl;
		drawContours(crop, contours,largest_contour_index, color, CV_FILLED, 8, hierarchy );
		rectangle(crop, bounding_rect, Scalar(0, 255, 0), 4, 8, 0); 
        imshow("norm", crop);
        video.write(crop);
        if (waitKey(1) >= 0) break;
    }
    return 0;
}

int main() {
	namedWindow("norm",1);
	//namedWindow("gs",1);
	//namedWindow("bin",1);
	ifstream fin("main.conf");

	string vname, fname;
	double x, dx, y, dy, min, max;
	int mode, orint;
	int n;
	fin >> n;
	for (int i = 0; i < n; i++) {
		fin >> mode >> vname >> x >> dx >> y >> dy >> fname >> min >> max >> orint;
		cout << mode << endl;
		if (mode == 1)
			update(vname, x, dx, y, dy, fname, min, max, orint);
	}
}