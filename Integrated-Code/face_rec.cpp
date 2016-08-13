/*
 * Copyright (c) 2011. Philipp Wagner <bytefish[at]gmx[dot]de>.
 * Released to public domain under terms of the BSD Simplified license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the organization nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 */
const int PREDICTION_NUM = 40;
int arr[PREDICTION_NUM] = {0};
int num=0;
#include "opencv2/core.hpp"
#include "opencv2/face.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace cv::face;
using namespace std;

#define deviceId 1

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(Error::StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            // Mat m = imread(path, 0);
            // Mat m2;
            // // cvtColor(m,m2,CV_BGR2GRAY);
            // m.copyTo(m2);
            // images.push_back(m2);
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}
int click=0,d=0;
int main(int argc, const char *argv[]) {
    // Check for valid command line arguments, print usage
    // if no arguments were given.
    /*if (argc != 4) {
        cout << "usage: " << argv[0] << " </path/to/haar_cascade> </path/to/csv.ext> </path/to/device id>" << endl;
        cout << "\t </path/to/haar_cascade> -- Path to the Haar Cascade for face detection." << endl;
        cout << "\t </path/to/csv.ext> -- Path to the CSV file with the face database." << endl;
        cout << "\t <device id> -- The webcam device id to grab frames from." << endl;
        exit(1);
    }*/
    // Get the path to your CSV:
    string fn_haar = string("/home/mrinaal/Humanoid/Huro_Summer_2016/haarcascade_frontalface_alt.xml");//argv[1]);
    string fn_csv = string("/home/mrinaal/Humanoid/Huro_Summer_2016/csv.csv");//argv[2]);
    // int deviceId = 0;//atoi(argv[3]);
    // These vectors hold the images and corresponding labels:
    vector<Mat> images;
    vector<int> labels;
    // Read in the data (fails if no valid input filename is given, but you'll get an error message):
    try {
        read_csv(fn_csv, images, labels);
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(1);
    }
    // Get the height from the first image. We'll need this
    // later in code to reshape the images to their original
    // size AND we need to reshape incoming faces to this size:
    int im_width = images[0].cols;
    int im_height = images[0].rows;
    // cerr<<im_height<<'\t'<<im_width<<'\n';
    // Create a FaceRecognizer and train it on the given images:
    Ptr<FaceRecognizer> model = createFisherFaceRecognizer();
    model->train(images, labels);
    /*Ptr<BasicFaceRecognizer> model2 = createEigenFaceRecognizer(0);
    model2->train(images, labels);
    Ptr<LBPHFaceRecognizer> model3 = createLBPHFaceRecognizer();
    model3->train(images, labels);*/
    // That's it for learning the Face Recognition model. You now
    // need to create the classifier for the task of Face Detection.
    // We are going to use the haar cascade you have specified in the
    // command line arguments:
    //
    CascadeClassifier haar_cascade;
    haar_cascade.load(fn_haar);
    // Get a handle to the Video device:
    VideoCapture cap(deviceId);
        
    // Check if we can use this device at all:
    if(!cap.isOpened()) {
        cerr << "Capture Device ID " << deviceId << "cannot be opened." << endl;
        return -1;
    }
    // Holds the current frame from the Video device:
    int max, found, face_max_num=0;
    Mat frame;
    Rect minFace(0, 0, 80, 80);
    Rect maxFace(0, 0, 250, 250);
    Rect face_i;
    waitKey(0);
    for(;;) {
        clock_t tim = clock();
        cap >> frame;
        // Clone the current frame:
        Mat original = frame.clone();
        // Convert the current frame to grayscale:
        Mat gray;
        Rect crop(frame.cols/5, 0, 3*frame.cols/5, frame.rows);
        rectangle(original, crop, Scalar(0, 0, 255), 1);
        cvtColor(original, gray, COLOR_BGR2GRAY);
        // cvtColor(original, gray, COLOR_BGR2GRAY);
        // Find the faces in the frame:
        vector< Rect_<int> > faces;
        haar_cascade.detectMultiScale(gray, faces, 1.2, 3, 0, minFace.size(), maxFace.size());        
        // cerr<<"face_num"<<faces.size()<<'\t'<<"area"<<faces[0].size();
        // At this point you have the position of the faces in
        // faces. Now we'll get the faces, make a prediction and
        // annotate it in the video. Cool or what?
        max = 3000,found =0;
        for(size_t i = 0; i < faces.size(); i++) {
            // Process face by face:
            face_i = faces[i];

            if(face_i.size().height*face_i.size().width>max){
                max=face_i.size().height*face_i.size().width;
                face_max_num=i; 
                found = 1;                       
            }
        }
        // cerr<<"area"<<face_i.size();

        if(found){
            // Crop the face from the image. So simple with OpenCV C++:
            Mat face = gray(faces[face_max_num]);
            // Resizing the face is necessary for Eigenfaces and Fisherfaces. You can easily
            // verify this, by reading through the face recognition tutorial coming with OpenCV.
            // Resizing IS NOT NEEDED for Local Binary Patterns Histograms, so preparing the
            // input data really depends on the algorithm used.
            //
            // I strongly encourage you to play around with the algorithms. See which work best
            // in your scenario, LBPH should always be a contender for robust face recognition.
            //
            // Since I am showing the Fisherfaces algorithm here, I also show how to resize the
            // face you have just found:
            /*if(click){
                Mat data;
                d++;
                cv::resize(face, data, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
                char str[50];
                strcpy(str,"/media/shrutij/07e59c2d-609c-49c8-8c97-067315c75258/database");
                char str1[2];
                str1[0] = (char)(d+48);
                str1[1] ='\0';
                strcat(str,str1);
                strcat(str,".jpg");
                cerr<<str;
                imwrite(str, data);
                click = 0;
            }
            FLAG*/
            Mat face_resized;
            cv::resize(face, face_resized, Size(im_width, im_height));//, 1.0, 1.0);
            imshow("re", face_resized);
            // Now perform the prediction, see how easy that is:
            int prediction = model->predict(face_resized);
            //int prediction2 = model2->predict(face_resized);
            //int prediction3 = model3->predict(face_resized);
            //arr[prediction]+=1;
            num++;

            if(num%51==0)
            {
                int mode =0;
                for(int j=1;j<PREDICTION_NUM;j++){
                    if(arr[j] > arr[mode])
                        mode=j;
                }
                prediction = mode;
                num=0;
            }
            int length;
            std::vector<char> name(length);
            // And finally write all we've found out to the original image!
            // First of all draw a green rectangle around the detected face:
            rectangle(original, faces[face_max_num], Scalar(0, 255,0), 1);
            string box_text;
            if(prediction==0) 
                box_text = format("Hi Mrinaal!");
                if(prediction==1) 
                box_text = format("Hi Rijak!");
                if(prediction==2) 
                 box_text = format("Hi Bhuvi!"); 
            if(prediction==3) 
                 box_text = format("Hi Shruti!"); 
            if(prediction==4) 
                 box_text = format("Hi Saurabh!"); 
               if(prediction==5) 
                box_text = format("Hi Nitish!");
                if(prediction==6) 
                 box_text = format("Hi Ujjwal!"); 
            
            // Create the text we will annotate the box with:
            // prediction2, prediction3);
            // Calculate the position for annotated text (make sure we don't
            // put illegal values in there):
            int pos_x = std::max(faces[face_max_num].tl().x - 10, 0);
            int pos_y = std::max(faces[face_max_num].tl().y - 10, 0);
            // And now put it into the image:
            //cerr<<"prediction"<<prediction<<'\t'<<"X,Y:"<<pos_x<<'\t'<<pos_y<<'\n';
            putText(original, box_text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, Scalar(128,128,128), 2);
        }
        faces.clear();        
        // Show the result:
        imshow("face_recognizer", original);
        cerr<<"\ttime\t"<<clock()-tim;
        // And display it:
        char key = (char) waitKey(20);
        // Exit this loop on escape:
        if(key == 27)
            break;
        if(key == 'c')
            click =1;
    }
    return 0;
}
