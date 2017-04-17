/*
 * David Z, Apr 21, 2015
 *
 * camera model, to access the parameters of a camera model, 
 * and, provide functions to convert (X,Y,Z) to (u,v,1)*lambda, 
 * convert (u,v,Z) to (X,Y,Z) with the parameters of the model
 *
 * */

#include <iostream>
#include "cam_model.h"
#define SQ(x) ((x)*(x))

using namespace std; 


CamModel* CamModel::gCamModel()
{
  static CamModel* gCam = new CamModel(); 
  return gCam; 
}

void CamModel::updategCamModel(CamModel& new_cam)
{
  *(gCamModel()) = new_cam; 
  return ;
}

void CamModel::distortCorrection(double x_d, double y_d, double& x_u, double &y_u)
{
    if(k1 == 0) // no distortion correction needed 
    {
      x_u = x_d; 
      y_u = y_d;
    }else if(k2 == 0)// only k1 is given 
    {
      // TODO :  not  quite understand what's the magic 
      double rd2 = SQ(x_d) + SQ(y_d); 
      double r_distoration = 1 + k1*rd2; 
      double ru2 = rd2 / r_distoration; 
      r_distoration = 1 + k1*ru2; 
      x_u = x_d / r_distoration; 
      y_u = y_d / r_distoration; 
      // cout <<"UV2XYZ: r2 = "<<rd2<<" t1= "<<r_distoration<<" xu: "<<x_u<<" yu: "<<y_u<<" xd: "<<x_d<<" yd: "<<y_d<<endl;
    }else
    {
        // using iterative fixed_point root finding algorithm 
        x_u = x_d; 
        y_u = y_d; 

        double rd2, rd4, rd6, k_radial, dx, dy; 
        for(int i=0; i<20; i++)
        {
          rd2 = SQ(x_u) + SQ(y_u); 
          rd4 = SQ(rd2); 
          rd6 = rd4 * rd2; 
          k_radial = 1 + k1 * rd2 + k2 * rd4 + k3 * rd6; 
          dx = 2 * p1* x_u * y_u + p2 * (rd2 + 2 * SQ(x_u)); 
          dy = p1 * (rd2 + 2 * SQ(y_u)) + 2 * p2 * x_u * y_u; 
          x_u = (x_d - dx)/k_radial; 
          y_u = (y_d - dy)/k_radial; 
        }
    }
    return ;
} 

void CamModel::convertXYZ2UV(float x, float y, float iz, float& ou, float& ov)
{
  float z = iz - z_offset;
  if(z <= 0)
  {
    // std::cerr<<__FILE__<<" convertXYZ2UV has z = "<<z<<" <= 0"<<std::endl; 
    return ; 
  }
  
  double x1 = x/z; 
  double y1 = y/z; 
  double r2 ; 
  double x2, y2, t1, t2; 
  
  if(k1 == 0) // no distortion correction needed 
  {
    x2 = x1; 
    y2 = y1;
  }else if(k2 == 0)// only k1 is given 
  {
    r2 = SQ(x1) + SQ(y1); 
    t1 = 1 + r2*k1; 
    x2 = x1*t1; 
    y2 = y1*t1;
    
    // cout <<"XYZ2UV: r2 = "<<r2<<" t1= "<<t1<<" x1: "<<x1<<" y1: "<<y1 <<" x2: "<<x2<<" y2: "<<y2<<endl;
  }else
  {
    // using iterative fixed_point root finding algorithm 
    r2 = SQ(x1) + SQ(y1); 
    double r4, r6; 
    r4 = SQ(r2); 
    r6 = r2*r4; 
    t1 = 1 + k1*r2 + k2*r4 + k3*r6; 
    // t2 = 1 + k4*r2 + k5*r4 + k6*r6; 
    x2 = x1*t1 + 2*p1*x1*y1 + p2*(r2+2*SQ(x1)); 
    y2 = y1*t1 + p1*(r2+2*SQ(y1)) + 2*p2*x1*y1;
  }

  ou = fx * x2 + cx; 
  ov = fy * y2 + cy; 
  return ; 
}

void CamModel::convertUVZ2XYZ(float u, float v, double z, double &ox, double& oy, double &oz)
{
  // just assume x' = x'', without considering the distortion effect 
  double fx_inv = 1./fx; 
  double fy_inv = 1./fy;
  // double x = (u - cx)*fx_inv;
  // double y = (v - cy)*fy_inv; 
  double x_d = (u-cx)*fx_inv;  // distorted point on the normalized image plane 
  double y_d = (v-cy)*fy_inv;   
  double x_u, y_u;  // undistorted point on the normalized image plane 
  
  distortCorrection(x_d, y_d, x_u, y_u);

  oz = z + z_offset; 
  ox = x_u*oz;
  oy = y_u*oz; 
}

