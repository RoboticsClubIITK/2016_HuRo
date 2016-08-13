#include <sstream>
#include <string>
#include <iostream>
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <limits>
#include <cmath>
#include <list>
#include "opencv2/core.hpp"
#include <fstream>

using namespace std;
using namespace cv;

ofstream f;

int bias = 0;

int h_min =10;
int s_min = 15;
int v_min = 20;
int h_max = 60;
int s_max = 60;
int v_max = 40;

void createTrackbars() {
    namedWindow("Trackbars",0);
    createTrackbar("H_MIN", "Trackbars", &h_min, 255);
    createTrackbar("H_MAX", "Trackbars", &h_max, 255);
    createTrackbar("S_MIN", "Trackbars", &s_min, 255);
    createTrackbar("S_MAX", "Trackbars", &s_max, 255);
    createTrackbar("V_MIN", "Trackbars", &v_min, 255);
    createTrackbar("V_MAX", "Trackbars", &v_max, 255);

}

int main() {
    int c=1;
    Mat cameraFeed, hsv, mask, temp;
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //List<MatOfPoint> contours = new ArrayList<MatOfPoint>();

    Moments moment;
    int x=0,y=0;
    bool track = false;
    VideoCapture cam;
    // cam.open("http://192.168.0.100:8080/video?x.mjpg");
    cam.open(0);
    createTrackbars();
    double minMaxCx = (bias > 0 ?  1000000 : -1000000);

    while (true) {
        cam >> cameraFeed;
        // cvtColor(cameraFeed, hsv, COLOR_BGR2HSV);
        cameraFeed.copyTo(hsv);
        //Mat roi = hsv;
        Mat roi(hsv, Rect(10, 2*hsv.rows/3, hsv.cols-20, hsv.rows/12));
        //Mat roi = hsv(Rect(10, 2*hsv.rows/3, hsv.cols-20, hsv.rows/12));
       
        inRange(roi, Scalar(h_min, s_min, v_min), Scalar(h_max, s_max, v_max), mask);
        medianBlur(mask, mask, 11);
        dilate(mask, mask, getStructuringElement(MORPH_RECT, Size(5,5)));
        mask.copyTo(temp);
        findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
        
        imshow("a",roi);
        if(track && contours.size() > 0){

            
            //Moments mu = moments(contours, false);
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
                // if (bias > 0)
                // {
                    cx = r.x ;//+ r.width - 12;
                    if (cx > minMaxCx)
                    {
                        minMaxCx = cx;
                    }
                // }
                // else
                // {
                    // cx = r.x + 12;
                    if (minMaxCx > cx)
                    {
                        minMaxCx = cx;
                    }
                // }
            }

            if (minMaxCx==1000000||minMaxCx==-1000000)
            minMaxCx = roi.cols/2;
            float t= (1.0f - 2.0f*(float)(minMaxCx/roi.cols))*1000;
            char ch;
            if(t>-0.20&&t<0.20)
                ch='f';
            else if(t<-0.20)
                ch='r';
            else if(t>0.20)
                ch='l';
            int count5=0;
            if(count5%100==0)
            {
                cerr<<ch;
                f.open("/dev/ttyACM0");
                f<<ch;
                f.close();
            }
            count5++;


        //     int j, maxindex, maxarea=0, area;
        //     for(j=0; j< contours.size(); j++)
        //     {
        //         moment = moments((Mat)contours.at(j));
        //         area= moment.m00;
        //         if(area>maxarea)
        //         {
        //             maxarea = area;
        //             maxindex = j;
        //         }
        //     }
        //     moment = moments((Mat)contours.at(maxindex));
        //     x = moment.m10/maxarea;
        //     y = moment.m01/maxarea;
        //     circle(cameraFeed, Point(x,y), 20, Scalar(0,255,0), 2);
        //     drawContours(cameraFeed,contours,maxindex,(0,255,0),2);
         }

        rectangle(cameraFeed, Rect(10, 2*hsv.rows/3, hsv.cols-20, hsv.rows/12), Scalar(172,255,135), 1, 8, 0 );
        
        imshow("Camera", mask);
        imshow("CameraFeed", cameraFeed);
        /*if(c%1000==0)
            cout<<x<<"\t"<<y<<"\n";*/
        char chr;
        char ch = waitKey(1);
        if (ch == 'q')
        {
            chr='s';
            f.open("/dev/ttyACM0");
            f<<chr;
            f.close();
            exit(0);

        }
        if (ch == 't')
            track = !track;
        // if((c%100)==0)
        //     cout<<x<<'\t'<<y<<'\n';
        // c++;
    }

    return 0;
}
