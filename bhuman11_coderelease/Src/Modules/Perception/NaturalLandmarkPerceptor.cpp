/**
 * @file NaturalLandmarkPerceptor.cpp
 * @author Daniel Mankowitz //
 */
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "Tools/ImageProcessing/surflib.h"
#include "Tools/ImageProcessing/OSFeatureExtraction.h"
#include "NaturalLandmarkPerceptor.h"
#include "Tools/Debugging/DebugDrawings.h"
#include "Tools/Streams/InStreams.h"
#include "Tools/Math/Geometry.h"
#include "Tools/Range.h"
#include "Tools/Team.h"
#include "Tools/Debugging/ReleaseOptions.h"
#include <algorithm>
#include <iostream>

/** The constructor */
NaturalLandmarkPerceptor::NaturalLandmarkPerceptor()
{

}

/** The function used to extract features and update the landmarks */
void NaturalLandmarkPerceptor::update(NaturalLandmarkPercept &naturalLandmarkPercept)
{
	//The matching score threshold value
	int matchScoreThreshold = 20;

	//Get the vector of matched keypoints
	naturalLandmarkPercept.matchedPoints.clear();

	//Generate a new image and compare it to each of the images in the image bank
	//The directory where the files are stored TESTING
	std::string dir = "../../Tools/ImageProcessing/imageBank";


	//Names of the two image files
	std::string name1 = "10";

	std::string name2 = "20";

	std::cout<<"Image in directory 1: "<<name1<<", Image in directory 2: "<<name2<<endl;


	// names of the images
	std::string fname1;

	//The image file read from the Nao converted to GrayScale (Probably too slow)
	//****************************************************************
	//Calculate the horizon line
	Geometry::Line horizon = Geometry::calculateHorizon(theCameraMatrix, theCameraInfo);
	//Calculate the line at the edge of the image
	Vector2<> base(theImage.cameraInfo.resolutionWidth/2, 0);
	Vector2<> direction(0,1);
	Geometry::Line imageLine(base, direction);
	//Calculate the intersection of 2 lines
	Vector2<> intersection(0,0);
	Geometry::getIntersectionOfLines(horizon,imageLine, intersection);

	int criticalPoint = 0;
	//Find the highest point of the horizon line. This removes the need to scan the
	//entire image for features
	if(horizon.base.y>intersection.y)
		criticalPoint = (float)intersection.y;
	else
		criticalPoint = (float)horizon.base.y;

	// if no arguments are passed:
	//Declare the images
	IplImage *imgGray1, *imgGray2;

	//Read in images
	//*****************************************************************
	fname1 =dir + "/"+ name1+".jpg";
	imgGray1 = cvLoadImage(fname1.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
	//IplImage* img = cvLoadImage(fname1.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
	//imgGray1 = img(cvRect(0, 0, theImage.cameraInfo.resolutionWidth/2, criticalPoint));


	//Convert the current image to grayscale
	GrayScaleImage grayImage;
	grayImage.copyChannel(theImage.image, 2);
	//Create a Mat matrix to store the image data
	imgGray2=cvCreateImage(cvSize(theImage.cameraInfo.resolutionHeight,theImage.cameraInfo.resolutionWidth/2),IPL_DEPTH_8U,1);

	for (int rr=0;rr<criticalPoint-1; rr++)
	{
		for (int cc=0; cc<(theImage.cameraInfo.resolutionWidth/2)-1;cc++)
		{
			//cvmSet(imgGray2, rr,cc,(int)theImage.image[rr][cc].y);
			//imgGray2.at(rr,cc) =  (int)theImage.image[rr][cc].y;
			CvScalar s;
			s.val[0] = theImage.image[rr][cc].y;
			cvSet2D(imgGray2,rr,cc,s);
		}
	}
	//*****************************************************************
	//*****************************************************************

	//Image.copyChannel(image, 2);
	//Image.copyChannel(image, 2);

	//MC: Generate a vector of Interest Points
	//*****************************************************************
	IpVec interestPoints1, interestPoints2;
	//*****************************************************************

	// create the OpenSURF detector:
	//*****************************************************************
	surfDet(imgGray1,interestPoints1, 4, 3, 2, 0.00005f);
	surfDet(imgGray2,interestPoints2, 4, 3, 2, 0.00005f);
	//*****************************************************************

	// get the OpenSURF descriptors
	// first image. Computes the descriptor for each of the keypoints.
	//Outputs a 64 bit vector describing the keypoints.
	//*****************************************************************
	surfDes(imgGray1,interestPoints1, false);
	surfDes(imgGray2,interestPoints2, false);
	//*****************************************************************

	//OpenSURF matching
	//*****************************************************************
	IpPairVec matches;
	getMatches(interestPoints1, interestPoints2, matches);
	//*****************************************************************

	//Output the matches
	std::cout<<"First Match coords x,y: "<<matches[1].first.x<<", "<<matches[1].first.y<<endl;
	std::cout<<"Second Match coords x,y: "<<matches[1].second.x<<", "<<matches[1].second.y<<endl;
	std::cout<<"The distance between interest points is: "<<matches[1].first-matches[1].second<<endl;

	//Verify that the matches are indeed correct. If they are not, discard them
	//from the matches
	verifyMatches(imgGray1, interestPoints1, interestPoints2, matches);

	//Create a matchedKeypoint
	MatchedKeypoint mk;
	//Matching score
	float matchingScore = 0;

	//Update the keypoints variable
	for (size_t ii = 1;ii<=matches.size();ii++)
	{
		//Store the match that we desire in the matched keypoint
		mk.keypoint = matches[ii].first;
		//Add this to the matched keypoint vector
		naturalLandmarkPercept.matchedPoints.push_back(mk);

		//Calculate the matching score to determine if there is a match
		matchingScore = matchingScore + 1/(matches[ii].first - matches[ii].second);
	}


	//Add the matching score to the natural landmark percept. This will be used to determine
	//if the images sufficiently match
	naturalLandmarkPercept.matchingScore = matchingScore;

	//Check whether or not a match has occurred
	if(matchingScore>matchScoreThreshold)
		naturalLandmarkPercept.matchFound = true;
	else
		naturalLandmarkPercept.matchFound = false;
}

void NaturalLandmarkPerceptor::verifyMatches(IplImage  *imgRGB1, IpVec &interestPoint1, IpVec &interestPoint2,IpPairVec &matches){

	//Declare a Feature Extraction object that contains the methods for verifying matches
	OSFeatureExtraction featureExtraction;

	// Variables for matching statistics
	float imageMatchingScore = 0;
	float imageMatchingScoreBest = 0;
	int totalNumMatches = 0;
	int totalNumValidMatches = 0;
	int totalNumInvalidMatches = 0;
	int totalNumBestMatches = 0;

	for( size_t i = 0; i < matches.size(); i++ )
	{

		if (matches.size()>0){
#if (DEBUG_MODE)
			cout<<"The number of matches is : "<<matches.size()<<endl;
			cout<<"****************************************"<<endl;
#endif
		}

		for(size_t kk =0; kk<matches.size();kk++)
		{
			cout<<"Keypoint index : "<<i<<endl;
			cout<<"****************************"<<endl;
			cout<<"Keypoint Image 1  row,col : "<<matches[kk].first.x<<", "<<matches[kk].first.y<<endl;
			cout<<"Keypoint Image 2  row,col : "<<matches[kk].second.y<<", "<<matches[kk].second.x<<endl;
			cout<<"****************************"<<endl;
		}


		//int allMatches = matches.size();
		int counter = 0;
		int matchSize = matches.size();
		bool isTrueMatchFound = false;

		Ipoint ip1 = matches[i].first;
		Ipoint ip2 = matches[i].second;
		//Determine the distance between matches
		float distanceMatch = ip1 - ip2;

		//Give a constant reward for being under a certain threshold
		float matchingScore = 0;
		if (distanceMatch==0)
			matchingScore=100;
		else
			matchingScore = 1/distanceMatch;

		cout<<"Distance match is: "<<distanceMatch<<endl;
#if (DEBUG_MODE)
		cout<<"Keypoint index : "<<i<<endl;
		cout<<"****************************"<<endl;
		cout<<"Keypoint Image 1  row,col : "<<ip1.x<<", "<<ip1.y<<endl;
		cout<<"Keypoint Image 2  row,col : "<<ip2.y<<", "<<ip2.x<<endl;
		cout<<"****************************"<<endl;
#endif

		//	#if (OUTPUT)
		//				dataAnalysis.displayOutput(keypoints2, keypoints, matchingScore, i1, i2);
		//	#endif
		//
		//Verify whether the match is correct or not
		//****************************************************
		bool correctMatch = featureExtraction.verifyMatch(*imgRGB1, ip1, ip2);
#if (DEBUG_MODE)
		cout<<"CorrectMatch: "<<correctMatch<<endl;
#endif
		//****************************************************

		//If the match is incorrect, remove the invalid match
		if (correctMatch==false)
		{
#if (DEBUG_MODE)
			cout<<"Entered Here"<<endl;
			cout<<"Keypoint Left to be erased row,col : "<<ip1.y<<", "<<ip2.x<<endl;
			cout<<"Keypoint Right to be erased row,col : "<<ip1.y<<", "<<ip2.x<<endl;
#endif

#if(DEBUG_MODE)
			cout<<"The number of matches before removal is : "<<matches.size()<<endl;
			cout<<"-----------------------------------------"<<endl;
#endif
			//Erase the corresponding match
			std::vector<std::pair<Ipoint, Ipoint> >::iterator IptIt;
			IptIt = matches.begin();

			//TO DO: We need to find a way of erasing the matches
			matches[i].first.x = 0;
			matches[i].first.y = 0;
			matches[i].second.x = 0;
			matches[i].second.y = 0;

			//matches.erase(IptIt + i);

			matchSize = matches.size();
#if (DEBUG_MODE)
			cout<<"The number of matches after removal is : "<<matchSize<<endl;
			cout<<"counter is : "<<counter<<endl;
			cout<<"----------------------------------------"<<endl;
#endif

			totalNumInvalidMatches = totalNumInvalidMatches + 1;
#if (DEBUG_MODE)
			leftPoints.push_back(ip1);
			rightPoints.push_back(ip2);
#endif
		}
		else
		{
			isTrueMatchFound = true;
			counter++;
		}
		//cv::waitKey(500);

		if(matches.size()==0)
			break;


		cout<<"The counter is: "<<counter<<endl;

		//This only considers the best correct match.
		if(isTrueMatchFound && counter ==1){
			totalNumBestMatches = totalNumBestMatches + 1;
			totalNumValidMatches = totalNumValidMatches + 1;

			imageMatchingScoreBest = imageMatchingScoreBest + matchingScore;
			isTrueMatchFound = false;
		}

		imageMatchingScore = imageMatchingScore + matchingScore;


		totalNumMatches = totalNumMatches + 1;
	}

}


MAKE_MODULE(NaturalLandmarkPerceptor, Perception)
