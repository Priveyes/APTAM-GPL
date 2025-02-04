// -*- c++ -*-
// Copyright 2008 Isis Innovation Limited
// Modified by ICGJKU 2015

//
// This header declares the data structures to do with keyframes:
// structs KeyFrame, Level, Measurement, Candidate.
// 
// A KeyFrame contains an image pyramid stored as array of Level;
// A KeyFrame also has associated map-point mesurements stored as a vector of Measurment;
// Each individual Level contains an image, corner points, and special corner points
// which are promoted to Candidate status (the mapmaker tries to make new map points from those.)
//
// KeyFrames are stored in the Map class and manipulated by the MapMaker.
// However, the tracker also stores its current frame as a half-populated
// KeyFrame struct.


#ifndef __KEYFRAME_H
#define __KEYFRAME_H

#include <../../ndk-modules/TooN/include/TooN/TooN.h>
#include <../../ndk-modules/TooN/include/TooN/se3.h>
#include <../../ndk-modules/cvd/installfiles/cvd/image.h>
#include <../../ndk-modules/cvd/installfiles/cvd/byte.h>
#include <vector>
#include <set>
#include <map>

#include "MapSerialization.h"

namespace APTAM {
using namespace TooN;
using namespace tinyxml2;

class MapPoint;
class SmallBlurryImage;

#define LEVELS 4

// Candidate: a feature in an image which could be made into a map point
struct Candidate
{
  CVD::ImageRef irLevelPos;
  Vector<2> v2RootPos;
  double dSTScore;
};

// Measurement: A 2D image measurement of a map point. Each keyframe stores a bunch of these.
struct Measurement
{
  int nLevel;   // Which image level?
  bool bSubPix; // Has this measurement been refined to sub-pixel level?
  Vector<2> v2RootPos;  // Position of the measurement, REFERED TO PYRAMID LEVEL ZERO
  enum {SRC_TRACKER, SRC_REFIND, SRC_ROOT, SRC_TRAIL, SRC_EPIPOLAR} Source; // Where has this measurement come frome?

  inline XMLElement* save(MapSerializationHelper& helper)
 {
	  XMLDocument* doc = helper.GetXMLDocument();
	  XMLElement *measurement = doc->NewElement("Measurement");

	  measurement->SetAttribute("nLevel",nLevel);
	  measurement->SetAttribute("bSubPix",bSubPix);
	  measurement->SetAttribute("v2RootPos",helper.saveVector(v2RootPos).c_str());
	  measurement->SetAttribute("Source",Source);

	  return measurement;
 }

 inline void load(const XMLElement* measurement, MapSerializationHelper& helper)
 {
	   measurement->QueryAttribute("nLevel",&nLevel);
	   measurement->QueryAttribute("bSubPix",&bSubPix);
	   v2RootPos = helper.loadVector<2>(string(measurement->FindAttribute("v2RootPos")->Value()));
	   measurement->QueryAttribute("Source",(int*)&Source);
 }
};

// Each keyframe is made of LEVELS pyramid levels, stored in struct Level.
// This contains image data and corner points.
struct Level
{
  inline Level()
  {
    bImplaneCornersCached = false;
  };
  
  CVD::Image<CVD::byte> im;                // The pyramid level pixels
  std::vector<CVD::ImageRef> vCorners;     // All FAST corners on this level
  std::vector<int> vCornerRowLUT;          // Row-index into the FAST corners, speeds up access
  std::vector<CVD::ImageRef> vMaxCorners;  // The maximal FAST corners
  Level& operator=(const Level &rhs);

  std::vector<Candidate> vCandidates;   // Potential locations of new map points
  
  bool bImplaneCornersCached;           // Also keep image-plane (z=1) positions of FAST corners to speed up epipolar search
  std::vector<Vector<2> > vImplaneCorners; // Corner points un-projected into z=1-plane coordinates

  int index;
};

// The actual KeyFrame struct. The map contains of a bunch of these. However, the tracker uses this
// struct as well: every incoming frame is turned into a keyframe before tracking; most of these 
// are then simply discarded, but sometimes they're then just added to the map.
struct KeyFrame
{
  inline KeyFrame()
  {
    pSBI = NULL;
	for(int i=0; i<LEVELS; i++)
	  aLevels[i].index = i;
  }
  SE3<> se3CfromW;    // The coordinate frame of this key-frame as a Camera-From-World transformation
  bool bFixed;      // Is the coordinate frame of this keyframe fixed? (only true for first KF!)
  Level aLevels[LEVELS];  // Images, corners, etc lives in this array of pyramid levels
  std::map<MapPoint*, Measurement> mMeasurements;           // All the measurements associated with the keyframe
  
  void MakeKeyFrame_Lite(CVD::BasicImage<CVD::byte> &im);   // This takes an image and calculates pyramid levels etc to fill the 
                                                            // keyframe data structures with everything that's needed by the tracker..
  void MakeKeyFrame_Rest();                                 // ... while this calculates the rest of the data which the mapmaker needs.
  
  double dSceneDepthMean;      // Hacky hueristics to improve epipolar search.
  double dSceneDepthSigma;
  
  SmallBlurryImage *pSBI; // The relocaliser uses this


  XMLElement* save(MapSerializationHelper& helper)
    {
  	  XMLDocument* doc = helper.GetXMLDocument();
  	  XMLElement *keyframe = doc->NewElement("KeyFrame");

  	  int kfid = helper.GetKeyFrameID(this);
  	  keyframe->SetAttribute("ID",kfid);

  	  helper.SaveImage(aLevels[0].im,kfid);

  	  keyframe->SetAttribute("se3CfromW",helper.saveVector(se3CfromW.ln()).c_str());
  	  keyframe->SetAttribute("bFixed",bFixed);
  	  keyframe->SetAttribute("dSceneDepthMean",dSceneDepthMean);
  	  keyframe->SetAttribute("dSceneDepthSigma",dSceneDepthSigma);

  	  XMLElement *measurements = doc->NewElement("Measurements");
  	  for(auto outer_iter=mMeasurements.begin(); outer_iter!=mMeasurements.end(); ++outer_iter)
  	  {
  		  XMLElement *measurement = outer_iter->second.save(helper);
  		  measurement->SetAttribute("MapPointID",helper.GetMapPointID(outer_iter->first));
  		  measurements->InsertEndChild(measurement);
  	  }

  	  keyframe->InsertEndChild(measurements);

      return keyframe;
    }

    void load(const XMLElement* keyframe, MapSerializationHelper& helper)
    {

    	int kfid = helper.GetKeyFrameID(this);
    	CVD::Image<CVD::byte> im;
    	helper.LoadImage(im,kfid);

    	MakeKeyFrame_Lite(im);

    	se3CfromW = SE3<>(helper.loadVector<6>(string(keyframe->FindAttribute("se3CfromW")->Value())));
	    keyframe->QueryAttribute("bFixed",&bFixed);
	    keyframe->QueryAttribute("dSceneDepthMean",&dSceneDepthMean);
	    keyframe->QueryAttribute("dSceneDepthSigma",&dSceneDepthSigma);

	    const XMLElement *measurements = keyframe->FirstChildElement("Measurements");
	    if(measurements!=NULL)
	    {
	    	const XMLElement *measurement = measurements->FirstChildElement("Measurement");
	    	while(measurement!=NULL)
	    	{
	    		Measurement m;
	    		m.load(measurement,helper);
	    		int mpid;
	    		measurement->QueryAttribute("MapPointID",&mpid);
	    		mMeasurements[helper.GetMapPoint(mpid)] = m;

	    		measurement = measurement->NextSiblingElement("Measurement");
	    	}
	    }

	    MakeKeyFrame_Rest();
    }
};

typedef std::map<MapPoint*, Measurement>::iterator meas_it;  // For convenience, and to work around an emacs paren-matching bug

}


#endif

