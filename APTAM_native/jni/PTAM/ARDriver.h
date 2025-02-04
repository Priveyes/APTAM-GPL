// -*- c++ -*-
// Copyright 2008 Isis Innovation Limited
// Modified by ICGJKU 2015

// ARDriver.h
// This file declares the ARDriver class
//
// ARDriver provides basic graphics services for drawing augmented
// graphics. It manages the OpenGL setup and the camera's radial
// distortion so that real and distorted virtual graphics can be
// properly blended.
//
#ifndef __AR_Driver_H
#define __AR_Driver_H
#include <../../ndk-modules/TooN/include/TooN/se3.h>
#include "ATANCamera.h"
#include "GLWindow2.h"
#include "Map.h"
#include "OpenGL.h"
#include <../../ndk-modules/cvd/installfiles/cvd/image.h>
#include <../../ndk-modules/cvd/installfiles/cvd/rgb.h>
#include <../../ndk-modules/cvd/installfiles/cvd/byte.h>
#include "ARTester.h"
#include "../../ndk-modules/cvd/installfiles/cvd/abs.h"

namespace APTAM {

using namespace std;
using namespace CVD;

class ARDriver
{
 public:
  ARDriver(const ATANCamera &cam, ImageRef irFrameSize, GLWindow2 &glw, Map &map);
  void Render(Image<Rgb<byte> > &imFrame, SE3<> se3CamFromWorld);
  void Reset();
  void Init();

  void KeyPressed(std::string key);
  
 protected:
  ATANCamera mCamera;
  GLWindow2 &mGLWindow;
  void DrawFadingGrid();
  void MakeFrameBuffer();
  void DrawFBBackGround();
  void DrawDistortedFB();
  void SetFrustum();
    
  // Texture stuff:
  GLuint mnFrameBuffer;
  GLuint mnFrameBufferTex;
  GLuint mnFrameTex;
  GLuint mnFrameTexUV;
  
  int mnCounter;
  ImageRef mirFBSize;
  ImageRef mirFrameSize;
  SE3<> mse3;
  bool mbInitialised;

  Map &mMap;

  ARTester mGame;
};

}

#endif
