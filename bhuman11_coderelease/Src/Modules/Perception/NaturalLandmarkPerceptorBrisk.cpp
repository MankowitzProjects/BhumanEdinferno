/**
 * @file NaturalLandmarkPerceptorBrisk.cpp
 * @author Daniel Mankowitz
 */
#include "opencv/cv.h"
#include "opencv/highgui.h"
//#include "Tools/ImageProcessing/surflib.h"
//#include "Tools/ImageProcessing/OSFeatureExtraction.h"
#include "Tools/ImageProcessing/include/brisk.h"
#include "Tools/ImageProcessing/include/FeatureExtraction.h"
#include "Tools/ImageProcessing/include/DataAnalysis.h"
#include "NaturalLandmarkPerceptorBrisk.h"
#include "Tools/Debugging/DebugDrawings.h"
#include "Tools/Streams/InStreams.h"
#include "Tools/Math/Geometry.h"
#include "Tools/Range.h"
#include "Tools/Team.h"
#include "Tools/Debugging/ReleaseOptions.h"
#include <algorithm>
#include <iostream>

#define DEBUG_MODE 0

/** The constructor */
NaturalLandmarkPerceptorBrisk::NaturalLandmarkPerceptorBrisk()
{

}

/** The function used to extract features and update the landmarks */
void NaturalLandmarkPerceptorBrisk::update(NaturalLandmarkPerceptBrisk &naturalLandmarkPerceptBrisk)
{
	//Get the vector of matched keypoints
	naturalLandmarkPerceptBrisk.matchedPoints.clear();

	//*****************************************************************
	//Set the arguments
	//std::string feat_detector = "SURF";
	//int threshold = 1000;
	bool hamming=true;
	std::string feat_detector = "BRISK";
	int threshold = 100;
	int hammingDistance = 85;//BRISK BRISK
	double radius = 0.15;//BRISK SURF
	std::string feat_descriptor = "BRISK";

	//Create the Feature extraction object
	FeatureExtraction feature;

	//Create data analysis object
	DataAnalysis dataAnalysis;

	// Declare the extractor. Only needs to be performed once since it computes lookup
	//tables for each of the various patterns on initialisation
	//*****************************************************************
	cv::Ptr<cv::DescriptorExtractor> descriptorExtractor;
	descriptorExtractor = feature.getExtractor(7, feat_descriptor, hamming, descriptorExtractor);
	//*****************************************************************

	//The directory where the files are stored
	std::string dir = "../images/PicsMG/Matching_Pics_Right_Overlapping";
	std::string dir1 = "../images/PicsMG/Matching_Pics_Right_Overlapping";//PicsOG/Matching_Images_OG_Left
	//Names of the two image files
	std::string name1 = "9";
	std::string name2 = "2";

	//For changing the threshold
	int testThreshold = 10;

	//Choose the images to compare
	//    name1 = to_string<int>(ii);
	//    if(ii==jj)
	//    continue;
	//
	//    name2 = to_string<int>(jj);

	cout<<"Image in directory 1: "<<name1<<", Image in directory 2: "<<name2<<endl;


	// names of the two images
	std::string fname1;
	std::string fname2;

	cv::Mat imgGray1;
	cv::Mat imgGray2;
	//cv::Mat imgRGB3;

	//bool do_rot=false;
	// standard file extensions
	std::vector<std::string> fextensions;
	fextensions.push_back(".jpeg");
	fextensions.push_back(".jpg");

	//	timespec ts, te, matchings, matchinge, detectors, detectore, extractors, extractore;
	//	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);


	// if no arguments are passed:
	//Read in images
	//*****************************************************************
	fname1 =dir + "/"+ name1+".jpg";
	fname2 = dir1+ "/"+ name2+".jpg";
	imgGray1 = cvLoadImage(fname1.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
	imgGray2 = cvLoadImage(fname2.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
	//*****************************************************************

	// convert to grayscale
	//*****************************************************************
	//	cv::Mat imgGray1;
	//	cv::cvtColor(imgRGB1, imgGray1, CV_BGR2GRAY);
	//	cv::Mat imgGray2;
	//	if(!do_rot){
	//		cv::cvtColor(imgRGB2, imgGray2, CV_BGR2GRAY);
	//	}
	//*****************************************************************

	//MC: Generate a vector of keypoints
	std::vector<cv::KeyPoint> keypoints, keypoints2;

	//clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &detectors);
	// create the detector:
	//*****************************************************************
	cv::Ptr<cv::FeatureDetector> detector;
	detector = feature.getDetector(7, feat_detector, detector, threshold, testThreshold,1);
	//*****************************************************************

	// run the detector:
	//*****************************************************************
	detector->detect(imgGray1,keypoints);
	detector->detect(imgGray2,keypoints2);
	//*****************************************************************
	//clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &detectore);
	//double detectionTime = diff(detectors,detectore).tv_nsec/1000000;

	//*****************************************************************
	// get the descriptors
	cv::Mat descriptors, descriptors2;
	std::vector<cv::DMatch> indices;

	//clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &extractors);
	// first image. Computes the descriptor for each of the keypoints.
	//Outputs a 64 bit vector describing the keypoints.
	descriptorExtractor->compute(imgGray2,keypoints2,descriptors2);
	// and the second one
	descriptorExtractor->compute(imgGray1,keypoints,descriptors);

	//clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &extractore);
	//double extractionTime = diff(extractors,extractore).tv_nsec/1000000;

	//clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &matchings);

	// matching
	//*****************************************************************
	std::vector<std::vector<cv::DMatch> > matches;
	cv::Ptr<cv::DescriptorMatcher> descriptorMatcher;
	if(hamming)
		descriptorMatcher = new cv::BruteForceMatcher<cv::HammingSse>();
	else
		descriptorMatcher = new cv::BruteForceMatcher<cv::L2<float> >();
	if(hamming)
		descriptorMatcher->radiusMatch(descriptors2,descriptors,matches,hammingDistance);
	else{
		//Messing with the maxdistance value will drastically reduce the number of matches
		descriptorMatcher->radiusMatch(descriptors2,descriptors,matches,radius);
		//descriptorMatcher->knnMatch(descriptors2,descriptors,matches,3);
	}
	//For the above method, we could use KnnMatch. All values less than 0.21 max distance are selected

	//clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &matchinge);
	//double matchingTime = diff(matchings,matchinge).tv_nsec/1000;
	//*****************************************************************


	//clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &te);
	//double overallTime = diff(ts,te).tv_nsec/1000000;

	//Verify that the matches are indeed correct. If they are not, discard them
	//from the matches
	verifyMatches(&imgGray1, keypoints, keypoints2, matches, feature, dataAnalysis);

	//Create a matchedKeypoint
	NaturalLandmarkPerceptBrisk::MatchedKeypoint mk;

	//Update the keypoints variable. Note keypoints refers to the current image
	for (size_t ii = 1;ii<=matches.size();ii++)
	{
		//Corresponds to the current image
		int i1 = matches[ii][0].trainIdx;
		//Store the match that we desire in the matched keypoint
		mk.keypoint = keypoints[i1];
		//Add this to the matched keypoint vector
		naturalLandmarkPerceptBrisk.matchedPoints.push_back(mk);
	}
}

void NaturalLandmarkPerceptorBrisk::verifyMatches(cv::Mat *imgGray1, vector<cv::KeyPoint> &keypoints, vector<cv::KeyPoint> &keypoints2,	std::vector<std::vector<cv::DMatch> > &matches, FeatureExtraction &feature, DataAnalysis &dataAnalysis){

	// Variables for matching statistics
	float imageMatchingScore = 0;
	float imageMatchingScoreBest = 0;
	int totalNumMatches = 0;
	int totalNumValidMatches = 0;
	int totalNumInvalidMatches = 0;
	int totalNumBestMatches = 0;

	//To store the incorrect matches
#if(DEBUG_MODE)
	std::vector<cv::Point2f> leftPoints;
	std::vector<cv::Point2f> rightPoints;
#endif

#if (DEBUG_MODE)
	cout<<"The total number of keypoints in image 1 is: "<<keypoints.size()<<endl;
	cout<<"The total number of keypoints in image 2 is: "<<keypoints2.size()<<endl;
#endif

	for( size_t i = 0; i < matches.size(); i++ )
	{

		if (matches[i].size()>0){
#if (DEBUG_MODE)
			cout<<"The number of matches is : "<<matches[i].size()<<endl;
			cout<<"****************************************"<<endl;
#endif
		}

		int allMatches = matches[i].size();
		int counter = 0;
		int matchSize = matches[i].size();
		bool isTrueMatchFound = false;

		//for(size_t j = 0; j < matches[i].size(); j++ )
		while(counter<matchSize)
		{


			//Corresponds to the first image
			int i1 = matches[i][counter].trainIdx;
			//Corresponds to the second image
			int i2 = matches[i][counter].queryIdx;
			//Determine the distance between matches
			float distanceMatch = matches[i][counter].distance;

			//Give a constant reward for being under a certain threshold
			float matchingScore = 0;
			if (distanceMatch==0)
				matchingScore=100;
			else
				matchingScore = 1/distanceMatch;

#if (DEBUG_MODE)
			cout<<"Keypoint indices i1, i2: "<<i1<<", "<<i2<<endl;
			cout<<"****************************"<<endl;
			cout<<"Keypoint Left  row,col : "<<(*(keypoints2.begin()+i2)).pt.y<<", "<<(*(keypoints2.begin()+i2)).pt.x<<endl;
			cout<<"Keypoint Right row,col : "<<(*(keypoints.begin() + i1)).pt.y<<", "<<(*(keypoints.begin() + i1)).pt.x<<endl;
			cout<<"****************************"<<endl;
#endif

#if (DEBUG_MODE)
			//dataAnalysis.displayOutput(keypoints2, keypoints, matchingScore, i1, i2);
#endif

			//Verify whether the match is correct or not
			//****************************************************
			bool correctMatch = feature.verifyMatch(*imgGray1, keypoints2[i2], keypoints[i1]);
#if (DEBUG_MODE)
			cout<<"CorrectMatch: "<<correctMatch<<endl;
#endif
			//****************************************************
			//If the match is incorrect, remove the invalid match
			if (correctMatch==false)
			{
#if (DEBUG_MODE)
				cout<<"Entered Here"<<endl;
				cout<<"Keypoint Left to be erased row,col : "<<(*(keypoints2.begin()+i2)).pt.y<<", "<<(*(keypoints2.begin()+i2)).pt.x<<endl;
				cout<<"Keypoint Right to be erased row,col : "<<(*(keypoints.begin() + i1)).pt.y<<", "<<(*(keypoints.begin() + i1)).pt.x<<endl;
#endif

#if(DEBUG_MODE)
				cout<<"The number of matches before removal is : "<<matches[i].size()<<endl;
				cout<<"-----------------------------------------"<<endl;
#endif
				//Erase the corresponding match
				matches[i].erase(matches[i].begin()+counter);

				matchSize = matches[i].size();
#if (DEBUG_MODE)
				cout<<"The number of matches after removal is : "<<matchSize<<endl;
				cout<<"counter is : "<<counter<<endl;
				cout<<"----------------------------------------"<<endl;
#endif

				totalNumInvalidMatches = totalNumInvalidMatches + 1;
#if (DEBUG_MODE)
				leftPoints.push_back(keypoints2[i2].pt);
				rightPoints.push_back(keypoints[i1].pt);
#endif
			}
			else
			{
				isTrueMatchFound = true;
				counter++;
			}
			//cv::waitKey(500);

			if(matches[i].size()==0)
				break;



			//This only considers the best correct match.
			if(isTrueMatchFound && counter ==1){
				totalNumBestMatches = totalNumBestMatches + 1;
				imageMatchingScoreBest = imageMatchingScoreBest + matchingScore;
				isTrueMatchFound = false;
			}

			imageMatchingScore = imageMatchingScore + matchingScore;

		}
		totalNumValidMatches = totalNumValidMatches + matches[i].size();
		totalNumMatches = totalNumMatches + allMatches;
	}

}


MAKE_MODULE(NaturalLandmarkPerceptorBrisk, Perception)
