// -*- c++ *--
// Copyright 2008 Isis Innovation Limited
// Modified by ICGJKU 2015
//
// VideoSource.h
// Declares the VideoSource class
// 
// This is a very simple class to provide video input; this can be
// replaced with whatever form of video input that is needed.  It
// should open the video input on construction, and provide two
// function calls after construction: Size() must return the video
// format as an ImageRef, and GetAndFillFrameBWandRGB should wait for
// a new frame and then overwrite the passed-as-reference images with
// GreyScale and Colour versions of the new frame.
//
#ifndef __VideoSource_H
#define __VideoSource_H
#include <../../ndk-modules/cvd/installfiles/cvd/image.h>
#include <../../ndk-modules/cvd/installfiles/cvd/byte.h>
#include <../../ndk-modules/cvd/installfiles/cvd/rgb.h>
#include <../../ndk-modules/cvd/installfiles/cvd/rgba.h>

namespace APTAM {

struct VideoSourceData;

class VideoSource
{
 public:
  VideoSource();
  void GetAndFillFrameBWandRGB(CVD::Image<CVD::byte> &imBW, CVD::Image<CVD::Rgb<CVD::byte> > &imRGB);
  CVD::ImageRef Size();
  
 private:
  void *mptr;
  CVD::ImageRef mirSize;
#ifdef __ANDROID__
  void getSize(int * sizeBuffer);
  void getFrame(CVD::Image<CVD::byte>* imBW, CVD::Image<CVD::Rgb<CVD::byte> >* imRGB, int width, int height);
 public:
  void changeBrightness(int change);
#endif
};


}
#endif
