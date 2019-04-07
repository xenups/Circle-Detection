#include <QCoreApplication>
#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"

using namespace cv;
using namespace std;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    VideoCapture cap(0);
    //Mat bgr = imread("/home/xenups/Desktop/1.jpg");
    while(cap.isOpened())
    {
        Mat bgr2;
        cap.read(bgr2);
        cout<<"1"<<endl;
        Mat src1;
        src1 = bgr2;
        namedWindow( "Original image", CV_WINDOW_AUTOSIZE );
        imshow( "Original image", src1 );
        cout<<"2"<<endl;
        Mat grey;
        cvtColor(src1, grey, CV_BGR2GRAY);
        cout<<"3"<<endl;
        Mat sobelx;
        Sobel(grey, sobelx, CV_32F, 1, 0);

        double minVal, maxVal;
        minMaxLoc(sobelx, &minVal, &maxVal); //find minimum and maximum intensities
        std::cout << "minVal : " << minVal << std::endl << "maxVal : " << maxVal << std::endl;

        Mat draw;
        sobelx.convertTo(draw, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));

        namedWindow( "Display window", CV_WINDOW_AUTOSIZE );
        Mat gray;
        gray = draw;

             Canny(gray, gray, 100, 200, 3);
        /// Find contours
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        RNG rng(12345);
        findContours( gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
        /// Draw contours
        Mat drawing = Mat::zeros( gray.size(), CV_8UC3 );
        for( int i = 0; i< contours.size(); i++ )
        {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
        }

        Mat src, gray2;
        src = drawing;
        cvtColor( src, gray2, CV_BGR2GRAY );

        // Reduce the noise so we avoid false circle detection
        GaussianBlur( gray2, gray2, Size(9, 9), 2, 2 );

        vector<Vec3f> circles;

        // Apply the Hough Transform to find the circles
        HoughCircles( gray2, circles, CV_HOUGH_GRADIENT, 1, 30, 200, 50, 0, 0 );

        // Draw the circles detected
        for( size_t i = 0; i < circles.size(); i++ )
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );// circle center
            circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );// circle outline
            std::cout << "center : " << center << "\nradius : " << radius << std::endl;
        }

        imshow( "Result window", src );

        waitKey(2);

    }

    return a.exec();
}
