/*
 * Feb. 7, 2017, David Z
 *
 * RealSense R200 Wrapper 
 *
 * */

#ifndef RS_R200_READER_H
#define RS_R200_READER_H

#include <opencv/cv.h>
#include <string>

class CRSR200Wrapper 
{
  public:
    CRSR200Wrapper();
    virtual ~CRSR200Wrapper(); 

    bool readOneFrameCV(std::string f_dir, int f_id, cv::Mat& rgb, cv::Mat& dpt); 
    bool readOneFrameCV(std::string f_rgb, std::string f_dpt, cv::Mat& rgb, cv::Mat& dpt); 
    bool readOneFrameGrey(std::string f_dir, int f_id, cv::Mat& grey, cv::Mat& dpt); 

    bool readImgCV(std::string f, cv::Mat& img); 
};



#endif
