#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <iostream>

typedef unsigned char    uchart;

using namespace cv;
#define REP(i, n)   for(int i=0;i<(int)(n);++i)
#define LOOP(i,b,n)   for(int i=(b);i<(int)(n);++i)

/// Global variables


int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = (char*)"Edge Map";

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
Mat CannyThreshold(int, void* , Mat src , Mat src_gray , Mat dst)
{
  Mat  detected_edges;

  /// Reduce noise with a kernel 3x3
  blur( src_gray, detected_edges, Size(3,3) );

  /// Canny detector
  Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

  /// Using Canny's output as a mask, we display our result
  dst = Scalar::all(0);

  src.copyTo( dst, detected_edges);
  Size s = detected_edges.size();
  // printf("detected_edges height = %d --- width = %d\n lowThreshold = %d\n ",s.height,s.width,lowThreshold);

  // if(lowThreshold >70 )
  //   REP(i,s.height)
  //     REP(j,s.width)
  //       if(detected_edges.at<int>(i,j) < 0)
  //         detected_edges.at<int>(i,j) = 0;


  // if(lowThreshold > 99){
  //   REP(i,s.height){
  //     REP(j,s.width){
  //       printf("%d ",detected_edges.at<int>(i,j) );
  //     }
  //     printf("\n");
  //   }
  // }

  return dst;
 }


/** @function main */
Mat call_canny(int _th , Mat _src )
{
  Mat src, src_gray ,dst;

  src = _src.clone() ;
  Size s = _src.size();
  int n = s.width, m=s.height;
  //printf(" n ,m %d %d\n",n,m );

  if( !src.data )
  { return Mat::zeros(0, 0, CV_32F);; }

  /// Create a matrix of the same type and size as src (for dst)
  dst.create( src.size(), src.type() );

  /// Convert the image to grayscale
  cvtColor( src, src_gray, COLOR_BGR2GRAY );

  /// Create a window
  // namedWindow( window_name, CV_WINDOW_AUTOSIZE );

  /// Create a Trackbar for user to enter threshold
  //createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
  lowThreshold = _th;

  /// Show the image
  Mat ret  =   CannyThreshold(0, 0 ,src  , src_gray , dst);

  Mat im_gray;
  cvtColor(ret,im_gray,COLOR_RGB2GRAY);
  Mat img_bw = im_gray > 40;


  /// Wait until user exit program by pressing a key
  // waitKey(0);

  return img_bw;
}
