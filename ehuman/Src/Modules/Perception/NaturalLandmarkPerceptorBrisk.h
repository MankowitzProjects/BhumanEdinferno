/**
* @file NaturalLandmarkPerceptorBrisk.h
* @author Daniel Mankowitz
*/

#pragma once

#include "Tools/Module/Module.h"
#include "Representations/Perception/NaturalLandmarkPerceptBrisk.h"
#include "Representations/Perception/ImageCoordinateSystem.h"
#include "Representations/Perception/CameraMatrix.h"
#include "Representations/Configuration/ColorTable64.h"
#include "Representations/Configuration/FieldDimensions.h"
#include "Representations/Infrastructure/Image.h"
#include "Representations/Perception/GrayScaleImage.h"
#include "Representations/Infrastructure/CameraInfo.h"
#include "Representations/Infrastructure/FrameInfo.h"
#include "Representations/Infrastructure/TeamInfo.h"
#include "Storage.h"
#include "Tools/Debugging/DebugImages.h"

MODULE(NaturalLandmarkPerceptorBrisk)
  REQUIRES(CameraMatrix)
  REQUIRES(ImageCoordinateSystem)
  REQUIRES(CameraInfo)
  REQUIRES(Image)
  REQUIRES(ColorTable64)
  REQUIRES(FrameInfo)
  PROVIDES_WITH_MODIFY(NaturalLandmarkPerceptBrisk)
END_MODULE


/**
 * @class NaturalLandmarkPerceptor
 */

class NaturalLandmarkPerceptorBrisk: public NaturalLandmarkPerceptorBriskBase
{

//class Parameters: public Streamable
//{

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
void update (NaturalLandmarkPerceptBrisk &naturalLandmarkPerceptBrisk);

///** Declare a reference to the image provided by the Nao */
//const Image* theImage;

public:
/** The constructor */
NaturalLandmarkPerceptorBrisk();

/** This function verifies whether or not there was a correct match */
void verifyMatches(cv::Mat  *imgGray1, vector<cv::KeyPoint> &keypoints, vector<cv::KeyPoint> &keypoints2,	std::vector<std::vector<cv::DMatch> > &matches, FeatureExtraction &feature, DataAnalysis &dataAnalysis);

};







