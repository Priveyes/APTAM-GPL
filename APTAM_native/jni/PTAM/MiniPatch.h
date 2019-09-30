// -*- c++ -*-
// Copyright 2008 Isis Innovation Limited
//
// MiniPatch.h
//
// Declares MiniPatch class
// 
// This is a simple pixel-patch class, used for tracking small patches
// it's used by the tracker for building the initial map

#ifndef __MINI_PATCH_H
#define __MINI_PATCH_H

#include <../../ndk-modules/cvd/installfiles/cvd/image.h>
#include <../../ndk-modules/cvd/installfiles/cvd/byte.h>
#include <../../ndk-modules/cvd/installfiles/cvd/utility.h>
#include <../../ndk-modules/TooN/include/TooN/TooN.h>
#include <vector>

namespace APTAM {

using namespace TooN;

struct MiniPatch
{
  void SampleFromImage(CVD::ImageRef irPos, CVD::BasicImage<CVD::byte> &im);  // Copy pixels out of source image
  bool FindPatch(CVD::ImageRef &irPos,           // Find patch in a new image
		 CVD::BasicImage<CVD::byte> &im, 
		 int nRange,
		 std::vector<CVD::ImageRef> &vCorners,
		 std::vector<int> *pvRowLUT = NULL);
  
  inline int SSDAtPoint(CVD::BasicImage<CVD::byte> &im, const CVD::ImageRef &ir); // Score function
  static int mnHalfPatchSize;     // How big is the patch?
  static int mnRange;             // How far to search? 
  static int mnMaxSSD;            // Max SSD for matches?
  CVD::Image<CVD::byte> mimOrigPatch;  // Original pixels
};


}

#endif
