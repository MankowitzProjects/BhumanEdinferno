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
#include "Tools/ImageProcessing/ipoint.h"

/**
* @class MatchedKeypoint
* Description of a matched Keypoint
*/
class MatchedKeypoint: public Streamable
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
Ipoint keypoint;
//int x = keypoint.x;
//** The constructor */
MatchedKeypoint();



};

/**
* @class NaturalLandmarkPercept
* Description of naturalLandmarkPercepts
*/
class NaturalLandmarkPercept: public Streamable
{
/** Streaming function
  * @param in  streaming in ...
  * @param out ... streaming out.
  */
private:
void serialize(In *in, Out *out);


public:
/**Store a vector of perceived keypoints here */
vector <MatchedKeypoint> matchedPoints;
/**Store the matching score of the image with the test bank image */
float matchingScore;
/**Store whether or not a match has been found */
bool matchFound;

/** The constructor */
NaturalLandmarkPercept();

};












