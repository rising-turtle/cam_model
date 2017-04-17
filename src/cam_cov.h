/*
 * Nov. 8, 2016, David Z
 *
 * Analysis camera observation covariance 
 *
 * */


#ifndef CAM_COV_H
#define CAM_COV_H

class CamModel; 

class CamCov
{
  public:
    CamCov(CamModel& ); 
    CamCov(); 
    virtual ~CamCov(); 
    
    virtual void getSigma(float u, float v, double z, double& Sxx, double& Syy, double& Szz);  // 
    static double getStaticSigma();  // assume the sigma of depth measurement 
  private:
    CamModel* mp_CamModel;  // point to the type of camera 
};

#endif
