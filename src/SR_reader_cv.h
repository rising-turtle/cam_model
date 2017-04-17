/*
 *
 *  Oct. 4, 2016, David Z
 *
 * readout as opencv mat
 *
 * */


#ifndef SR_READER_CV_H
#define SR_READER_CV_H

#include <opencv/cv.h>
#include "SR_reader.h"

class CSReadCV : public CSReader
{
  public:
    CSReadCV(); 
    virtual ~CSReadCV(); 
    
    bool getCurrentFrameCV(cv::Mat& , cv::Mat&);
    bool readOneFrameCV(string f_name, cv::Mat&, cv::Mat&); 
    bool readOneFrameDatCV(string f_name, cv::Mat&, cv::Mat&); 

    void fromSRToCV(sr_data& , cv::Mat&, cv::Mat& );

};



#endif
