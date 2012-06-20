/**
* @file NaturalLandmarkPerceptor.h
* @author Daniel Mankowitz
*/

#pragma once

#include "Tools/Module/Module.h"
#include "Representations/Perception/NaturalLandmarkPercept.h"
#include "Representations/Perception/ImageCoordinateSystem.h"
#include "Representations/Perception/CameraMatrix.h"
#include "Representations/Configuration/ColorTable64.h"
#include "Representations/Configuration/FieldDimensions.h"
#include "Representations/Infrastructure/Image.h"
#include "Representations/Infrastructure/CameraInfo.h"
#include "Representations/Infrastructure/FrameInfo.h"
#include "Representations/Infrastructure/TeamInfo.h"
#include "Storage.h"
#include "Tools/Debugging/DebugImages.h"

MODULE(NaturalLandmarkPerceptor)
  REQUIRES(CameraMatrix)
  REQUIRES(ImageCoordinateSystem)
  REQUIRES(CameraInfo)
  REQUIRES(Image)
  REQUIRES(ColorTable64)
  REQUIRES(FrameInfo)
  PROVIDES_WITH_MODIFY(NaturalLandmarkPercept)
END_MODULE


/**
 * @class NaturalLandmarkPerceptor
 */

class NaturalLandmarkPerceptor: public NaturalLandmarkPerceptorBase
{

//class Parameters: public Streamable
//{
//
//private:
//	void serialize(In *in, Out *out){
//
//STREAM_REGISTER_BEGIN();
//STREAM()
//STREAM_REGISTER_FINISH();
//
//	}
//
//
//
//};

/** The update function for the natural landmark percept containing the keypoint */
void update (NaturalLandmarkPercept &naturalLandmarkPercept);

/** Declare a reference to the image provided by the Nao */
const Image* theImage;

public:
/** The constructor */
NaturalLandmarkPerceptor();

/** This function verifies whether or not there was a correct match */
void verifyMatches(IplImage* imgRGB1, IpVec &interestPoint1, IpVec &interestPoint2,IpPairVec &matches);

};







