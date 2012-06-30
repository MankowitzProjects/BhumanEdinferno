/**
* @file NaturalLandmarkPercept.h
* Implementation of a class that represents the matchedKeypoint percepts
* @author Daniel Mankowitz
*/

#include "NaturalLandmarkPerceptBrisk.h"

void NaturalLandmarkPerceptBrisk::serialize(In *in, Out *out)
{
  //Sending a vector of matchedkeypoints
  STREAM_REGISTER_BEGIN();
  STREAM(matchedPoints);
  STREAM(matchingScore);
  STREAM(matchFound);
  STREAM_REGISTER_FINISH();

}

void NaturalLandmarkPerceptBrisk::MatchedKeypointBrisk::serialize(In *in, Out *out)
{

  //Sending a matchedkeypoint
//  STREAM_REGISTER_BEGIN();
//  STREAM(keypoint);
//  STREAM_REGISTER_FINISH();

}
