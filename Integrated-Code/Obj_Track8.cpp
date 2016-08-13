#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <fstream>

using namespace std;
using namespace cv;

char t1, t2='\0';

char window_name[30] = "HSV Segemtation";
 Mat src;
 Mat Frame;
 Mat hsv2;
 Mat imgthreshold;
 char c='\0';
 int count5=0; 
 int arr[10]={0,0,0,0,0,0,0,0,0,0};
ofstream fout;		//  /dev/ttyACM2

int flag=0;
Point2f centre(-1,-1);

void movement(int nx, int ny);

int main()
{
  int maxH, minH; 
  int maxS, minS; 
  int maxV, minV; 
  int count = 0;

  VideoCapture cam(0); 
  cam.set(CV_CAP_PROP_FRAME_WIDTH,200);
  cam.set(CV_CAP_PROP_FRAME_HEIGHT,200);
  char key;

  minH = 0;
  maxH = 25;
  minS = 72;
  maxS = 250;
  minV = 180;
  maxV = 255;

  Mat imgTmp;
  cam.read(imgTmp); 
  Mat imgCircles = Mat::zeros( imgTmp.size(), CV_8UC3 );

  while(1) 
  {
    char key = waitKey(33); 
    //if(key==27) return 0; 
  
    cam.read(src); 
    cvtColor(src,hsv2,CV_BGR2HSV);
 
    inRange(hsv2, Scalar(minH, minS, minV), Scalar(maxH, maxS, maxV), imgthreshold); 
    medianBlur(imgthreshold, imgthreshold, 11);
  
    vector<vector<Point> > contour;
    vector<Vec4i> hierarchy;
    vector<Point> approx;
    findContours(imgthreshold, contour, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, Point(0,0));
    if(contour.size())
    {
      double maxarea = contourArea(contour[0]);
      double area;
      int maxindex=0;
      for(int i=1; i<contour.size(); i++)
      {
        area = contourArea(contour[i]);
        if(area>maxarea)
        {
          maxarea = area;
          maxindex = i;
        }
      }
      approxPolyDP(Mat(contour[maxindex]), approx, arcLength(Mat(contour[maxindex]), true)*0.02, true );
      if( fabs(contourArea(contour[maxindex])) <0.1 || !isContourConvex(approx))
      {
        flag = 0;
      }
      else  if( approx.size() >7)
            {
              float radius;
              
              minEnclosingCircle(contour[maxindex], centre, radius);
              circle(src,centre,radius,Scalar(0,255,0),2);
  
              flag = 1;
            }
    
    }
    else
    {
      flag=0;
    }
  
    movement(centre.x, centre.y);

    if(t1=='4')
      arr[4]++;
    else if(t1=='5')
          arr[5]++;
         else if(t1=='6')
                arr[6]++;
              else if(t1=='7')
                    arr[7]++;
                  else if(t1=='8')
                        arr[8]++;  
  
    if((count5 % 5)==0)
    {
      int most=0;
      for(int j=5; j<9; j++)
      {
        if(t1=='0')
        break;
        else
        {
          most=0;
          if(arr[j]>arr[most])
          {
            most=j;
          }
        }
      }

      switch(most)
      {
        case 4: t1='4';break;
        case 5: t1='5';break;
        case 6: t1='6';break;
        case 7: t1='7';break;
        case 8: t1='8';break;

      }

      for(int k=4; k<9; k++)     
      {
        arr[k]=0;
      }
    
      for( int i=0; i<10; i++)
      {
        fout.open("/dev/ttyACM2");
        cerr << t1 ;
        fout<<t1;
        fout.close();
      }
    }
    count5++;

    imshow("Original",src ); //show the original image

    int c = waitKey(10);
    if( (char)c == 27 ) { break; } // escape
  } 
	cerr<<endl;
  return 0; 
}

void movement(int nx, int ny)
{
  if(flag)
  {
    if( nx < src.cols/3)
    {
      t1 = '4';
    }
    else if( nx > (2*src.cols)/3)
          {
            t1 = '6';
          }
          else
          {
            if( ny > (3*src.rows)/4)
            {
              t1 = '0';
            }
            else
            {
              t1 = '5';
            }
          }
  }
  else
  {
    if( nx == -1 )
    {
      t1 = '8';
    }
    else if( nx < (src.cols)/5 || nx > (4*src.cols)/5)
          {
            t1 = '8';
          }
          else if( ny > (6*src.rows)/7 )
                {
                  t1 = '0';
                }
               else
               {
                  t1 = '7';
               }
  }
}