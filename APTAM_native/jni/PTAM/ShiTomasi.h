// Copyright 2008 Isis Innovation Limited
#ifndef __SHI_TOMASI__H
#define __SHI_TOMASI__H

#include <../../ndk-modules/cvd/installfiles/cvd/image.h>
#include <../../ndk-modules/cvd/installfiles/cvd/byte.h>

namespace APTAM {


double FindShiTomasiScoreAtPoint(CVD::BasicImage<CVD::byte> &image,
                                 int nHalfBoxSize,
                                 CVD::ImageRef irCenter);


}

#endif
