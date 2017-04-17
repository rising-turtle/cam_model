#include "SR_reader_cv.h"

namespace{
void map_raw_img_to_grey(unsigned short * pRaw, unsigned char* pGrey, int N);
void map_raw_img_to_grey_linear(unsigned short * pRaw, unsigned char* pGrey, int N);
}

CSReadCV::CSReadCV() : CSReader(){}
CSReadCV::~CSReadCV(){}

bool CSReadCV::getCurrentFrameCV(cv::Mat& i_img, cv::Mat& d_img)
{
  sr_data sr;
  bool finished; 
  sr = get_current_frame(finished); 
  if(finished) return false; 
  
  fromSRToCV(sr, i_img, d_img);
  return true;
}

bool CSReadCV::readOneFrameCV(string f_name, cv::Mat& i_img, cv::Mat& d_img)
{
  sr_data sr; 
  if(readOneFrame(f_name, sr))
  {
    fromSRToCV(sr, i_img, d_img);
    return true;
  }
  return false; 
}

bool CSReadCV::readOneFrameDatCV(string f_name, cv::Mat& i_img, cv::Mat& d_img)
{
  sr_data sr; 
  if(readOneFrameDat(f_name, sr))
  {
    fromSRToCV(sr, i_img, d_img); 
    return true; 
  }
  return false; 
}


void CSReadCV::fromSRToCV(sr_data& sr, cv::Mat& i_img, cv::Mat& d_img)
{
  // generate imgs and dpts 
  cv::Mat cv_d_img, cv_i_img; 

  if(!b_sr_new_version_) // old version
  {
    static vector<unsigned char> grey_img(SR_SIZE); 
    map_raw_img_to_grey(&sr.intensity_[0], &grey_img[0], SR_SIZE); 
    // map_raw_img_to_grey_linear(&sr.intensity_[0], &grey_img[0], SR_SIZE); 

    // cv::Mat raw_img = cv::Mat(SR_HEIGHT, SR_WIDTH, CV_16UC1, &sr.intensity_[0], SR_WIDTH*sizeof(unsigned short)); 
    // cv_i_img = cv::Mat(raw_img.size(), CV_8UC1); 
    // raw_img.convertTo(cv_i_img, CV_8UC1, 0.05, -25); 

    cv_i_img = cv::Mat(SR_HEIGHT, SR_WIDTH, CV_8UC1, &grey_img[0], SR_WIDTH*sizeof(unsigned char)); 
    
    static SR_IMG_TYPE dis_s[SR_SIZE] = {0}; 
    for(int i=0; i<SR_SIZE; ++i)
    {
      dis_s[i] = (SR_IMG_TYPE)(sr.z_[i]*1000);
    }
    cv_d_img = cv::Mat(SR_HEIGHT, SR_WIDTH, CV_16UC1, dis_s); 

  }else{
    // TODO: 
    
  }
  i_img = cv_i_img.clone(); 
  d_img = cv_d_img.clone();
}

namespace{
void map_raw_img_to_grey(unsigned short * pRaw, unsigned char* pGrey, int N)
{
  unsigned short limit_s = 65000;
  unsigned short* p1 = pRaw; 
  unsigned char* p2 = pGrey; 
  
  unsigned char max_c = 0; 
  unsigned char tc = 0; 
  static vector<float> sqrt_map_;
  
  double scale = 255./65000.;

  if(sqrt_map_.size() <= 0)
  {
    int N = 65535; 
    sqrt_map_.resize(N);
    for(int i=0; i<N; i++)
      sqrt_map_[i] = (unsigned char)(sqrt(double(i))); 
  }
  
  for(int i=0; i<N; i++)
  {
    if(*p1 >= limit_s) // delete the values larger than 65000
      tc = 0; 
    else 
      tc = sqrt_map_[*p1];
    if(tc > max_c) {max_c = tc; }
    *p2 = tc;
    ++p1; 
    ++p2; 
  }
  assert(max_c > 0);
  p2 = pGrey;
  float inv_max = (float)(255.0/max_c);
  for(int i=0; i<N; i++)
  {
    *p2 = (unsigned char)((*p2)*inv_max);
    // *p2 = *p2*scale;
    ++p2;
  }
}
void map_raw_img_to_grey_linear(unsigned short * pRaw, unsigned char* pGrey, int N)
{
  unsigned short limit_s = 65000;
  unsigned short* p1 = pRaw; 
  unsigned char* p2 = pGrey; 
  
  unsigned short max_c = 0; 
  unsigned short tc = 0; 
  
  for(int i=0; i<N; i++)
  {
    if(*p1 >= limit_s) // delete the values larger than 65000
      tc = 0; 
    else 
      tc = *p1;
    if(tc > max_c) {max_c = tc; } 
    ++p1; 
  }
  assert(max_c > 0);
  p2 = pGrey;
  double scale = (255.)/((double)max_c); 
  scale = scale > 1? 1 : scale ;
  p1 = pRaw; 
  for(int i=0; i<N; i++)
  {
    // *p2 = (unsigned char)((*p2)*inv_max);
    *p2 =(char)((double)(*p1)*scale);
    ++p2;
    ++p1;
  }
}
}
