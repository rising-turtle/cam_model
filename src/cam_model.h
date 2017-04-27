/*
 * David Z, Apr 21, 2015
 *
 * camera model, to access the parameters of a camera model, 
 * and, provide functions to convert (X,Y,Z) to (u,v,1)*lambda, 
 * convert (u,v,Z) to (X,Y,Z) with the parameters of the model
 *
 * */
#ifndef CAM_MODEL_H
#define CAM_MODEL_H

#include "opencv2/opencv.hpp"

#define SQ(x) ((x)*(x))

class CamModel
{
  public:
    CamModel(cv::Mat& mat)
    {
      _toCam(mat);
      z_offset = 0;
      mbDistRectify = false; 
    }
    CamModel(): z_offset(0), m_z_scale(1.), mbDistRectify(false){}
    CamModel(double fx1, double fy1, double cx1, double cy1, double k11=0, double k22=0, double p11=0, double p22=0, double k33=0, double k44=0, double k55=0, double k66=0): mbDistRectify(false),
    fx(fx1), fy(fy1), cx(cx1), cy(cy1), k1(k11), k2(k22), p1(p11), p2(p22), k3(k33), k4(k44), k5(k55), k6(k66), z_offset(0), m_z_scale(1.), m_rows(144), m_cols(176), width(m_cols), height(m_rows)
  {}
    virtual ~CamModel(){}
  public:
    // convert (u, v, Z) to (X, Y, Z) with the current parameters 
    void convertUVZ2XYZ(float u, float v, double z, double &ox, double &oy, double& oz); 
    void distortCorrection(double x_d, double y_d, double& x_u, double &y_u); 

    // convert (X,Y,Z) to (u,v,1)
    void convertXYZ2UV(float x, float y, float z, float &u, float &v); 

    void setDepthScale(double s){m_z_scale = s;}
    void setRows(int r){m_rows = r;}
    void setCols(int c){m_cols = c;}
    void print(); 

    void undistortImage(const cv::Mat& raw, cv::Mat& rectified); 
    cv::Mat m_undist_map1; 
    cv::Mat m_undist_map2; 
    bool mbDistRectify; 
    bool mbDistortion; 

    void _toMat(cv::Mat& mat)
    {
       double * _cm = (double*)mat.ptr(); 
      // camera matrix, fx, cx, fy, cy, 
      _cm[0] = fx; _cm[2] = cx; 
      _cm[4] = fy; _cm[5] = cy; 
      _cm[8] = 1.0; 
    }
    void _toCam(cv::Mat& mat)
    {
      double * _cm = (double*)mat.ptr(); 
      fx = _cm[0]; cx = _cm[2] ; 
      fy = _cm[4]; cy = _cm[5] ; 
    }

    // image size 
    int m_rows; 
    int m_cols; 
    int width; 
    int height;

    // intrinsic matrix 
    double fx; 
    double fy; 
    double cx; 
    double cy;
    
    // z_offset, maybe only used for swiss ranger 4k 
    double z_offset; 
    
    // z scale 
    double m_z_scale; 

    // distortion parameters
    double k1; 
    double k2; 
    double k3; 
    double k4;
    double k5; 
    double k6; 
    double p1; 
    double p2;
  
    // global cam model, to avoid pass the same parameters many time 
    static CamModel* gCamModel(); 
    static void updategCamModel(CamModel&);
};



#endif
