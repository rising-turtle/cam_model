#include "cam_cov.h"
#include "cam_model.h"
#include <iostream>

using namespace std; 

CamCov::CamCov(CamModel& model) : 
  mp_CamModel(new CamModel(model))
{

}
CamCov::CamCov() : 
  mp_CamModel(0)
{
}

CamCov::~CamCov()
{
  if(mp_CamModel) 
  {
    delete mp_CamModel;   
  }
}

double CamCov::getStaticSigma()
{
  double sigma_depth =  0.014; // 0.014; 0.05  // 1.4 cm according to the documentation of SR4000  
  return (sigma_depth*sigma_depth); 
}

void CamCov::getSigma(float u, float v, double z, double& Sxx, double& Syy, double& Szz)
{
  // base class, use constant noise model 
  // Szz = 0.05*0.05; // 0.05*0.05; // 5, 2 cm
  Szz = CamCov::getStaticSigma();
  double epsilon = 1e-3;
  double dz = 2*epsilon; 
  double dxdz, dydz; 
  double px, py, pz, qx, qy, qz; 
  if(mp_CamModel == 0) 
  {
    cout << __FILE__<< " use constant Sxx Syy Szz" <<endl; 
    Sxx = Syy = Szz; 
    return ; 
  }
  
  mp_CamModel->convertUVZ2XYZ(u, v, z+epsilon, px, py, pz); 
  mp_CamModel->convertUVZ2XYZ(u, v, z-epsilon, qx, qy, qz); 
  
  dxdz = (px-qx)/dz; dydz = (py-qy)/dz; 
  Sxx = dxdz*dxdz*Szz; 
  Syy = dydz*dydz*Szz;
  return ;
}





