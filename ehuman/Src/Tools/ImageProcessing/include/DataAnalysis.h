#ifndef DATAANALYSIS_H
#define DATAANALYSIS_H
#include <fstream>
#include <iostream>
#include <list>
#include <opencv2/opencv.hpp>
#include "../include/brisk.h"

#include<string>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

class DataAnalysis
{
    public:
        DataAnalysis();
        ~DataAnalysis();
        int getNumImagesInDirectory(string *dir);
        void displayOutput(vector<cv::KeyPoint> keypoints, vector<cv::KeyPoint> keypoints2,float matchingScore, int i1, int i2);
        void writeToFile();
        void help(char**);
    protected:
    private:
};

#endif // DATAANALYSIS_H
