// -*- c++ -*-
// Copyright 2008 Isis Innovation Limited
// Modified by ICGJKU 2015

#ifndef __CALIB_CORNER_PATCH_H
#define __CALIB_CORNER_PATCH_H

#include <../../ndk-modules/TooN/include/TooN/TooN.h>
#include <../../ndk-modules/cvd/installfiles/cvd/image.h>
#include <../../ndk-modules/cvd/installfiles/cvd/byte.h>

namespace APTAM {
using namespace TooN;

class CalibCornerPatch
{
public:
  struct Params
  {
    Params();
    Matrix<2> m2Warp();
    Vector<2> v2Pos;
    Vector<2> v2Angles;
    double dMean;
    double dGain;
  };
  
  CalibCornerPatch(int nSideSize = 8);
  bool IterateOnImage(Params &params, CVD::Image<CVD::byte> &im);
  bool IterateOnImageWithDrawing(Params &params, CVD::Image<CVD::byte> &im);

 protected:
  void MakeTemplateWithCurrentParams();
  void FillTemplate(CVD::Image<float> &im, Params params);
  double Iterate(CVD::Image<CVD::byte> &im);
  Params mParams;
  CVD::Image<float> mimTemplate;
  CVD::Image<Vector<2> > mimGradients;
  CVD::Image<Vector<2> > mimAngleJacs;
  
  void MakeSharedTemplate();
  static CVD::Image<float> mimSharedSourceTemplate;

  double mdLastError;
};

}
#endif

