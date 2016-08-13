/* HURO - Robotics Club IITK */

#include <sstream>
#include <string>
#include <iostream>
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include "opencv2/core.hpp"
#include <fstream>

using namespace std;
using namespace cv;

int counter=0;

ofstream fout;

int t;

int bias = 0;

int h_min =10;
int s_min = 15;
int v_min = 20;
int h_max = 130;
int s_max = 130;
int v_max = 85;


void createTrackbars() 
{
    namedWindow("Trackbars",0);
    createTrackbar("H_MIN", "Trackbars", &h_min, 255);
    createTrackbar("H_MAX", "Trackbars", &h_max, 255);
    createTrackbar("S_MIN", "Trackbars", &s_min, 255);
    createTrackbar("S_MAX", "Trackbars", &s_max, 255);
    createTrackbar("V_MIN", "Trackbars", &v_min, 255);
    createTrackbar("V_MAX", "Trackbars", &v_max, 255);

}


int main() 
{
    
    Mat cameraFeed, hsv, mask, temp;
    
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;

    Moments moment;
    int x=0,y=0;
    bool track = false;
    VideoCapture cam;
    cam.open(1);
    createTrackbars();
    double minMaxCx = (bias > 0 ?  1000000 : -1000000);

    while (true) 
    {
        cam >> cameraFeed;
        cameraFeed.copyTo(hsv);
        Mat roi(hsv, Rect(10, 2*hsv.rows/3, hsv.cols-20, hsv.rows/12));     //roi -> region of interest
       
        inRange(roi, Scalar(h_min, s_min, v_min), Scalar(h_max, s_max, v_max), mask);
        medianBlur(mask, mask, 11);
        dilate(mask, mask, getStructuringElement(MORPH_RECT, Size(5,5)));
        mask.copyTo(temp);
        findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
        
        imshow("a",roi);
        if(track && contours.size() > 0)
        {            
            int j, maxindex, maxarea=0, area;
            for(j=0; j< contours.size(); j++)
            {
                moment = moments((Mat)contours.at(j));
                area= moment.m00;
                if(area>maxarea)
                {
                    maxarea = area;
                    maxindex = j;
                }
            }
            
            moment = moments((Mat)contours.at(maxindex));

            if (moment.m00 > 0.0)
            {
                Rect r = boundingRect(contours.at(maxindex));
                double cx;
                cx = r.x ;
                if (cx > minMaxCx)
                {
                    minMaxCx = cx;
                }
                
                if (minMaxCx > cx)
                {
                        minMaxCx = cx;
                }
            }

            if (minMaxCx==1000000||minMaxCx==-1000000)
            minMaxCx = roi.cols/2;
            t = (1.0f - 2.0f*(float)(minMaxCx/roi.cols))*5;
            

            char ch;
            if(t>-0.20&&t<0.20)
                ch='f';
            else if(t<-0.20)
                ch='r';
            else if(t>0.20)
                ch='l';
            
            
            t= abs(t);
            
            char chr[3];
            chr[0]=ch;
            char t1 = (char)(t+'0');
            chr[1]=t1;
            chr[2]='-';
            
            cerr<<chr<<'\n';
            
            if(counter>(2*t))
                {
                    fout.open("/dev/ttyACM1");
                    fout<<chr;
                    fout.flush();
                    fout.close();
                    counter=0;
                }
                counter++;
        }

        rectangle(cameraFeed, Rect(10, 2*hsv.rows/3, hsv.cols-20, hsv.rows/12), Scalar(172,255,135), 1, 8, 0 );
        
        imshow("Camera", mask);
        imshow("CameraFeed", cameraFeed);
        
        char chr;
        char ch = waitKey(50);
        if (ch == 'q')
        {
            chr='s';
            f.open("/dev/ttyACM1");
            f<<chr;
            f.close();
            exit(0);

        }
        if (ch == 't')
            track = !track;
        
    }

    return 0;
}