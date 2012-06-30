#ifndef FEATUREEXTRACTION_H
#define FEATUREEXTRACTION_H

#include <opencv2/opencv.hpp>
#include "brisk.h"
#include <fstream>
#include <iostream>
#include <list>
#include <iomanip>
#include <math.h>

class FeatureExtraction
{
    public:
        FeatureExtraction();
        ~FeatureExtraction();
        //Functions for gathering data
        cv::Ptr<cv::FeatureDetector> getDetector(int argc, char** argv, cv::Ptr<cv::FeatureDetector> detector, int threshold, int testThreshold, int testFlag);
        cv::Ptr<cv::DescriptorExtractor> getExtractor(int argc,char** argv,bool hamming, cv::Ptr<cv::DescriptorExtractor> descriptor );


        //Override functions for actual implementation
        cv::Ptr<cv::DescriptorExtractor> getExtractor(int argc, string feat_descriptor,bool hamming, cv::Ptr<cv::DescriptorExtractor> descriptorExtractor);
        cv::Ptr<cv::FeatureDetector> getDetector(int argc, string feat_detector, cv::Ptr<cv::FeatureDetector> detector, int threshold, int testThreshold, int testFlag);

        //Verfify that the matches are indeed correct
        void performMatchingValidation(const cv::Mat & image, std::vector<cv::KeyPoint> &keypoints, std::vector<cv::KeyPoint> &keypoints2, std::vector<std::vector<cv::DMatch> > &matches, bool hamming);
        bool verifyMatch(const cv::Mat &image, cv::KeyPoint &keypoint1, cv::KeyPoint &keypoint2);
        void verifyKNNMatches(std::vector<cv::DMatch>  &matches);
        void verifyMatchingOrder(const cv::Mat & image,cv::Mat descriptors, cv::Mat descriptors2, std::vector<std::vector<cv::DMatch> > &matches);
        double calcEuclideanDistance(cv::Mat d1, cv::Mat d2);

        //The matching variables
        float imageMatchingScore;
        float imageMatchingScoreBest;
        int totalNumValidMatches;
        int totalNumMatches;
        int totalNumInvalidMatches;
        int totalNumBestMatches;
        //Stores the incorrect matches
    	std::vector<cv::Point2f> leftPoints;
    	std::vector<cv::Point2f> rightPoints;

    protected:
    private:
        //The distances between a keypoint and its nearest neighbors
        float dist1;
        float dist2;
        float distanceRatio;
        //KNN variables
        bool isKnnMatch;



};

#endif // FEATUREEXTRACTION_H
