#include "rs_r200_wrapper.h"
#include <sstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv; 

CRSR200Wrapper::CRSR200Wrapper(){}
CRSR200Wrapper::~CRSR200Wrapper(){}

bool CRSR200Wrapper::readImgCV(std::string file, cv::Mat& img)
{
  img = imread(file.c_str(), -1); 
  return (img.data != NULL); 
}

bool CRSR200Wrapper::readOneFrameCV(std::string f_rgb, std::string f_dpt, cv::Mat& rgb, cv::Mat& dpt)
{
  return (readImgCV(f_rgb, rgb) && readImgCV(f_dpt, dpt)); 
}

bool CRSR200Wrapper::readOneFrameCV(string f_dir, int id, cv::Mat& rgb, cv::Mat& dpt)
{
  stringstream ss_rgb, ss_dpt; 
  ss_rgb<<f_dir<<"/color/"<<setfill('0')<<setw(6)<<id<<".png";  // setw(4)
  ss_dpt<<f_dir<<"/depth/"<<setfill('0')<<setw(6)<<id<<".png";  // setw(4)

  // load rgb and dpt 
  rgb = imread(ss_rgb.str().c_str(), -1); 
  dpt = imread(ss_dpt.str().c_str(), -1); 
  return (rgb.data != NULL) && (dpt.data != NULL); 
}

bool CRSR200Wrapper::readOneFrameGrey(string f_dir, int id, cv::Mat& grey, cv::Mat& dpt)
{
  cv::Mat rgb; 
  if(!readOneFrameCV(f_dir, id, rgb, dpt))
  {
    cerr<<__FILE__<<" failed in readOneFrameCV"<<endl;
    return false; 
  }
  
  grey = cv::Mat(rgb.size(), CV_8UC1); 
  cv::cvtColor(rgb, grey, CV_RGB2GRAY); 
  return true; 

}
