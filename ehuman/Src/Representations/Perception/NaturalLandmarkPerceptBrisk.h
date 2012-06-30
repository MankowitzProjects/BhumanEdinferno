/**
* @file NaturalLandmarkPercept.h
* Declaration of a class that represents the natural landmark percepts
* This class stores the representation of a matched keypoint and a set of matched keypoints
* @author daniel
*/
#pragma once

//Include files here
#include "Tools/ColorClasses.h"
#include "Tools/Streams/Streamable.h"
#include "Tools/Math/Vector2.h"
#include <opencv2/opencv.hpp>



/**
* @class NaturalLandmarkPercept
* Description of naturalLandmarkPercepts
*/
class NaturalLandmarkPerceptBrisk: public Streamable
{
/** Streaming function
  * @param in  streaming in ...
  * @param out ... streaming out.
  */
private:
void serialize(In *in, Out *out);

public:
/**
* @class MatchedKeypoint
* Description of a matched Keypoint
*/
class MatchedKeypointBrisk: public Streamable
{
private:
/** Streaming function
  * @param in  streaming in ...
  * @param out ... streaming out.
  */
void serialize(In *in, Out *out);

public:
//Store the parameters of each keypoint here
/** The keypoint object that has been matched with an image in the test bank. */
cv::KeyPoint keypoint;
//int x = keypoint.x;
//** The constructor */
MatchedKeypointBrisk() {}



};

/**Store a vector of perceived keypoints here */
vector <MatchedKeypointBrisk> matchedPoints;
/**Store the matching score of the image with the test bank image */
float matchingScore;
/**Store whether or not a match has been found */
bool matchFound;

/** The constructor */
NaturalLandmarkPerceptBrisk(): matchingScore(0), matchFound(false) {}

};












