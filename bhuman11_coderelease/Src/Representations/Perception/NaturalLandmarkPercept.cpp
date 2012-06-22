/**
* @file NaturalLandmarkPercept.h
* Implementation of a class that represents the matchedKeypoint percepts
* @author Daniel Mankowitz
*/

#include "NaturalLandmarkPercept.h"

void NaturalLandmarkPercept::serialize(In *in, Out *out)
{
  //Sending a vector of matchedkeypoints
  STREAM_REGISTER_BEGIN();
  STREAM(matchedPoints);
  STREAM(matchingScore);
  STREAM_REGISTER_FINISH();

}

void NaturalLandmarkPercept::MatchedKeypoint::serialize(In *in, Out *out)
{

  //Sending a matchedkeypoint
//  STREAM_REGISTER_BEGIN();
//  STREAM(keypoint);
//  STREAM_REGISTER_FINISH();

}
