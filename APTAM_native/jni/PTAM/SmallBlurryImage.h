// -*- c++ -*-
// Copyright 2008 Isis Innovation Limited
//
// SmallBlurryImage - A small and blurry representation of an image.
// used by the relocaliser.

#ifndef __SMALLBLURRYIMAGE_H
#define __SMALLBLURRYIMAGE_H
#include <../../ndk-modules/cvd/installfiles/cvd/image.h>
#include <../../ndk-modules/cvd/installfiles/cvd/byte.h>
#include <../../ndk-modules/TooN/include/TooN/se2.h>
#include <../../ndk-modules/TooN/include/TooN/se3.h>
#include "KeyFrame.h"
#include "ATANCamera.h"

namespace APTAM {

  
class SmallBlurryImage
{
 public:
  SmallBlurryImage();
  SmallBlurryImage(KeyFrame &kf, double dBlur = 2.5);
  void MakeFromKF(KeyFrame &kf, double dBlur = 2.5);
  void MakeJacs();
  double ZMSSD(SmallBlurryImage &other);
  std::pair<SE2<>,double> IteratePosRelToTarget(SmallBlurryImage &other, int nIterations = 10);
  static SE3<> SE3fromSE2(SE2<> se2, ATANCamera camera);
  
protected:
  CVD::Image<CVD::byte> mimSmall;
  CVD::Image<float> mimTemplate;
  CVD::Image<Vector<2> > mimImageJacs;
  bool mbMadeJacs;
  static CVD::ImageRef mirSize;
};


}

#endif


