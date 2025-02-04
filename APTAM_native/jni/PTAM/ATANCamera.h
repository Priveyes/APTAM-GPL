// *-* c++ *-*
// Copyright 2008 Isis Innovation Limited
// Modified by ICGJKU 2015

// N-th implementation of a camera model
// GK 2007
// Evolved a half dozen times from the CVD-like model I was given by
// TWD in 2000
// 
// This one uses the ``FOV'' distortion model of
// Deverneay and Faugeras, Straight lines have to be straight, 2001
//
// BEWARE: This camera model caches intermediate results in member variables
// Some functions therefore depend on being called in order: i.e.
// GetProjectionDerivs() uses data stored from the last Project() or UnProject()
// THIS MEANS YOU MUST BE CAREFUL WITH MULTIPLE THREADS
// Best bet is to give each thread its own version of the camera!
//
// Camera parameters are stored in a GVar, but changing the gvar has no effect
// until the next call to RefreshParams() or SetImageSize().
//
// Pixel conventions are as follows:
// For Project() and Unproject(),
// round pixel values - i.e. (0.0, 0.0) - refer to pixel centers
// I.e. the top left pixel in the image covers is centered on (0,0)
// and covers the area (-.5, -.5) to (.5, .5)
//
// Be aware that this is not the same as what opengl uses but makes sense
// for acessing pixels using ImageRef, especially ir_rounded.
//
// What is the UFB?
// This is for projecting the visible image area
// to a unit square coordinate system, with the top-left at 0,0,
// and the bottom-right at 1,1
// This is useful for rendering into textures! The top-left pixel is NOT
// centered at 0,0, rather the top-left corner of the top-left pixel is at 
// 0,0!!! This is the way OpenGL thinks of pixel coords.
// There's the Linear and the Distorting version - 
// For the linear version, can use 
// glMatrixMode(GL_PROJECTION); glLoadIdentity();
// glMultMatrix(Camera.MakeUFBLinearFrustumMatrix(near,far));
// To render un-distorted geometry with full frame coverage.
//

#ifndef __ATAN_CAMERA_H
#define __ATAN_CAMERA_H

#include <../../ndk-modules/TooN/include/TooN/TooN.h>
#include <cmath>
#include <../../ndk-modules/cvd/installfiles/cvd/vector_image_ref.h>
#include <../../ndk-modules/gvars3/installfiles/gvars3/gvars3.h>

#include "MapSerialization.h"

#ifdef __ANDROID__
#include <android/log.h>
#endif

namespace APTAM {
  
using namespace TooN;
using namespace tinyxml2;

#define NUMTRACKERCAMPARAMETERS 5

class CameraCalibrator;
class CalibImage;

// The parameters are:
// 0 - normalized x focal length
// 1 - normalized y focal length
// 2 - normalized x offset
// 3 - normalized y offset
// 4 - w (distortion parameter)

class ATANCamera
{
  public:
  ATANCamera();
  ATANCamera( std::string sName );
  ATANCamera(std::string sName, CVD::ImageRef irSize, Vector<NUMTRACKERCAMPARAMETERS> vParams);
  ATANCamera(std::string sName, CVD::ImageRef irSize, Vector<9> opencvParams);
    
  // Image size get/set: updates the internal projection params to that target image size.
  void SetImageSize(Vector<2> v2ImageSize);
  inline void SetImageSize(CVD::ImageRef irImageSize) {SetImageSize(vec(irImageSize));};
  inline Vector<2> GetImageSize() {return mvImageSize;};
  void RefreshParams();
  
  // Various projection functions
  Vector<2> Project(const Vector<2>& camframe); // Projects from camera z=1 plane to pixel coordinates, with radial distortion
  inline Vector<2> Project(CVD::ImageRef ir) { return Project(vec(ir)); }
  Vector<2> UnProject(const Vector<2>& imframe); // Inverse operation
  inline Vector<2> UnProject(CVD::ImageRef ir)  { return UnProject(vec(ir)); }
  
  Vector<2> UFBProject(const Vector<2>& camframe);
  Vector<2> UFBUnProject(const Vector<2>& camframe);
  inline Vector<2> UFBLinearProject(const Vector<2>& camframe);
  inline Vector<2> UFBLinearUnProject(const Vector<2>& fbframe);
  
  Matrix<2,2> GetProjectionDerivs(); // Projection jacobian
  
  inline bool Invalid() {  return mbInvalid;}
  inline double LargestRadiusInImage() {  return mdLargestRadius; }
  inline double OnePixelDist() { return mdOnePixelDist; }
  
  // The z=1 plane bounding box of what the camera can see
  inline Vector<2> ImplaneTL(); 
  inline Vector<2> ImplaneBR(); 

  // OpenGL helper function
  Matrix<4> MakeUFBLinearFrustumMatrix(double near, double far);

  // Feedback for Camera Calibrator
  double PixelAspectRatio() { return mvFocal[1] / mvFocal[0];}
  

  // Useful for gvar-related reasons (in case some external func tries to read the camera params gvar, and needs some defaults.)
  static const Vector<NUMTRACKERCAMPARAMETERS> mvDefaultParams;

  bool useOpenCVDistortion;

  XMLElement* save(MapSerializationHelper& helper)
  {
	  XMLDocument* doc = helper.GetXMLDocument();
	  XMLElement *camera = doc->NewElement("Camera");

	  camera->SetAttribute("useOpenCVDistortion",useOpenCVDistortion);
	  camera->SetAttribute("mvCameraParams",helper.saveVector(mvCameraParams).c_str());
	  camera->SetAttribute("opencv_camparams",helper.saveVector(opencv_camparams).c_str());
	  camera->SetAttribute("mvImageSize",helper.saveVector(mvImageSize).c_str());
	  camera->SetAttribute("msName",msName.c_str());

	  return camera;
  }

  void load(const XMLElement* camera, MapSerializationHelper& helper)
  {
	  camera->QueryAttribute("useOpenCVDistortion",&useOpenCVDistortion);

	  mvCameraParams = helper.loadVector<NUMTRACKERCAMPARAMETERS>(string(camera->FindAttribute("mvCameraParams")->Value()));
	  opencv_camparams = helper.loadVector<9>(string(camera->FindAttribute("opencv_camparams")->Value()));
	  mvImageSize = helper.loadVector<2>(string(camera->FindAttribute("mvImageSize")->Value()));

	  msName = string(camera->FindAttribute("msName")->Value());

	  if(useOpenCVDistortion)
	     {
	  	   mvCameraParams[0] = opencv_camparams[0]/mvImageSize[0];
	  	   mvCameraParams[1] = opencv_camparams[1]/mvImageSize[1];
	  	   mvCameraParams[2] = (opencv_camparams[2]+0.5)/mvImageSize[0];
	  	   mvCameraParams[3] = (opencv_camparams[3]+0.5)/mvImageSize[1];
	  	   mvCameraParams[4] = 0;
	     }

	  RefreshParams();

  }

 protected:
  GVars3::gvar3<Vector<NUMTRACKERCAMPARAMETERS> > mgvvCameraParams; // The actual camera parameters
  Vector<NUMTRACKERCAMPARAMETERS> mvCameraParams; //hack to allow overwrite
  
  Matrix<2, NUMTRACKERCAMPARAMETERS> GetCameraParameterDerivs();
  void UpdateParams(Vector<NUMTRACKERCAMPARAMETERS> vUpdate);
  void DisableRadialDistortion();
  
  // Cached from the last project/unproject:
  Vector<2> mvLastCam;      // Last z=1 coord
  Vector<2> mvLastIm;       // Last image/UFB coord
  Vector<2> mvLastDistCam;  // Last distorted z=1 coord
  DefaultPrecision mdLastR;           // Last z=1 radius
  DefaultPrecision mdLastDistR;       // Last z=1 distorted radius
  DefaultPrecision mdLastFactor;      // Last ratio of z=1 radii
  bool mbInvalid;           // Was the last projection invalid?
  
  // Cached from last RefreshParams:
  DefaultPrecision mdLargestRadius; // Largest R in the image
  DefaultPrecision mdMaxR;          // Largest R for which we consider projection valid
  DefaultPrecision mdOnePixelDist;  // z=1 distance covered by a single pixel offset (a rough estimate!)
  DefaultPrecision md2Tan;          // distortion model coeff
  DefaultPrecision mdOneOver2Tan;   // distortion model coeff
  DefaultPrecision mdW;             // distortion model coeff
  DefaultPrecision mdWinv;          // distortion model coeff
  DefaultPrecision mdDistortionEnabled; // One or zero depending on if distortion is on or off.
  Vector<2> mvCenter;     // Pixel projection center
  Vector<2> mvFocal;      // Pixel focal length
  Vector<2> mvInvFocal;   // Inverse pixel focal length
  Vector<2> mvImageSize;  
  Vector<2> mvUFBLinearFocal;
  Vector<2> mvUFBLinearInvFocal;
  Vector<2> mvUFBLinearCenter;
  Vector<2> mvImplaneTL;   
  Vector<2> mvImplaneBR;

  Vector<9> opencv_camparams;

  // Radial distortion transformation factor: returns ration of distorted / undistorted radius.
  inline double rtrans_factor(double r)
  {
    if(r < 0.001 || mdW == 0.0)
      return 1.0;
    else 
      return (mdWinv* atan(r * md2Tan) / r);
  };

  // Inverse radial distortion: returns un-distorted radius from distorted.
  inline double invrtrans(double r)
  {
    if(mdW == 0.0)
      return r;
    return(tan(r * mdW) * mdOneOver2Tan);
  };
  
  std::string msName;

  friend class CameraCalibrator;   // friend declarations allow access to calibration jacobian and camera update function.
  friend class CalibImage;
};

// Some inline projection functions:
inline Vector<2> ATANCamera::UFBLinearProject(const Vector<2>& camframe)
{
  Vector<2> v2Res;
  v2Res[0] = camframe[0] * mvUFBLinearFocal[0] + mvUFBLinearCenter[0];
  v2Res[1] = camframe[1] * mvUFBLinearFocal[1] + mvUFBLinearCenter[1];
  return v2Res;
}

inline Vector<2> ATANCamera::UFBLinearUnProject(const Vector<2>& fbframe)
{
  Vector<2> v2Res;
  v2Res[0] = (fbframe[0] - mvUFBLinearCenter[0]) * mvUFBLinearInvFocal[0];
  v2Res[1] = (fbframe[1] - mvUFBLinearCenter[1]) * mvUFBLinearInvFocal[1];
  return v2Res;
}


}

#endif

