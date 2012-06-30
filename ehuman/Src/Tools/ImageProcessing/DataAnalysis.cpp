#include "include/DataAnalysis.h"

DataAnalysis::DataAnalysis()
{
	//ctor
}

DataAnalysis::~DataAnalysis()
{
	//dtor
}

int DataAnalysis::getNumImagesInDirectory(string *dir)
{
	//Read in the number of images from a directory
	//************************************
	ifstream fin;
	string filepath;
	int num;
	DIR *dp;
	struct dirent *dirp;
	struct stat filestat;
	int jpegCounter = 0;
	bool terminal = false;
	string mgMatchLeft;
	string mgMatchRight;
	string ogMatchLeft;
	string ogMatchRight;

	//There are four directories where files can be found
	if (terminal){
		mgMatchLeft = "../../images/PicsMG/Matching_Pics_Left_Overlapping";
		mgMatchRight = "../../images/PicsMG/Matching_Pics_Right_Overlapping";
		ogMatchLeft = "../../images/PicsOG/Matching_Pics_Left_Overlapping";
		ogMatchRight = "../../images/PicsOG/Matching_Pics_Right_Overlapping";
	}
	else
	{
		mgMatchLeft = "../images/PicsMG/Matching_Pics_Left_Overlapping";
		mgMatchRight = "../images/PicsMG/Matching_Pics_Right_Overlapping";
		ogMatchLeft = "../images/PicsOG/Matching_Pics_Left_Overlapping";
		ogMatchRight = "../images/PicsOG/Matching_Pics_Right_Overlapping";
	}


	//For the images from the old Nao
	string mgMatchLeftOld = "../../images/PicsMGOld/Matching_MG_Left_old";
	string mgMatchRightOld = "../../images/PicsMGOld/Matching_MG_Right_old";
	string ogMatchLeftOld = "../../images/PicsOGOld/Matching_OG_Left_old";
	string ogMatchRightOld = "../../images/PicsOGOld/Matching_OG_Right_old";

	//std::cout << "dir to get files of: " << flush;
	//getline( cin, *dir);  // gets everything the user ENTERs
	int value = atoi((*dir).c_str());
	switch(value){
	case 1:
		*dir = mgMatchLeft;
		break;
	case 2:
		*dir = mgMatchRight;
		break;
	case 3:
		*dir = ogMatchLeft;
		break;
	case 4:
		*dir = ogMatchRight;
		break;
	case 5:
		*dir = mgMatchLeftOld;
		break;
	case 6:
		*dir = mgMatchRightOld;
		break;
	case 7:
		*dir = ogMatchLeftOld;
		break;
	case 8:
		*dir = ogMatchRightOld;
		break;
	default:
		std::cout<<"Nothing found"<<endl;
		break;

	}


	dp = opendir( (*dir).c_str() );
	if (dp == NULL)
	{
		//cout << "Error(" << errno << ") opening " << dir << endl;
		return 0;//errno;
	}

	while ((dirp = readdir( dp )))
	{
		filepath = *dir + "/" + dirp->d_name;

		// If the file is a directory (or is in some way invalid) we'll skip it
		if (stat( filepath.c_str(), &filestat )) continue;
		if (S_ISDIR( filestat.st_mode ))         continue;

		jpegCounter = jpegCounter +1;
	}

	closedir( dp );

	return jpegCounter;

}

void DataAnalysis::displayOutput(vector<cv::KeyPoint> keypoints, vector<cv::KeyPoint> keypoints2,float matchingScore, int i1, int i2)
{
	//std::cout<<"This corresponds to keypoints ("<<keypoints[i1].pt.y<<", "<<keypoints[i1].pt.x<<") and second ("<<keypoints2[i2].pt.y<<", "<<keypoints2[i2].pt.x<<")"<<endl;

	//cout<<"The distance between "<<i1<<" and "<<i2<<" is "<<1/matchingScore<<endl;
	//cout<<"The matching score between "<<i1<<" and "<<i2<<" is "<<matchingScore<<endl;

	std::cout<<"The size 1: "<<keypoints[i1].size<<" The size 2: "<<keypoints[i2].size<<endl;
	//std::cout<<"The angle 1: "<<keypoints[i1].angle<<" The angle 2: "<<keypoints[i2].angle<<endl;
	std::cout<<"The response 1: "<<keypoints[i1].response<<" The response 2: "<<keypoints[i2].response<<endl;
	std::cout<<"The octave 1: "<<keypoints[i1].octave<<" The octave 2: "<<keypoints[i2].octave<<endl;
	//std::cout<<"The class id 1: "<<keypoints[i1].class_id<<" The class id 2: "<<keypoints[i2].class_id<<endl;

}

void DataAnalysis::writeToFile(){




}

void DataAnalysis::help(char** argv){
	std::cout << "This command line tool lets you evaluate different keypoint "
			<< "detectors, descriptors and matchers." << std::endl
			<< "usage:" << std::endl
			<< argv[0] << " <dataset> <2nd> <detector> <descriptor> [descFile1 descFile1]" << std::endl
			<< "    " << "dataset:    Folder containing the images. The images must be of .ppm "<< std::endl
			<< "    " << "            format. They must be named img#.ppm, and there must be "<< std::endl
			<< "    " << "            corresponding homographies named H1to#." << std::endl
			<< "    " << "            You can also use the prefix rot-, then 2nd will be the" << std::endl
			<< "    " << "            rotation in degrees." << std::endl
			<< "    " << "2nd:        Number of the 2nd image (the 1st one is always img1.ppm)"<< std::endl
			<< "    " << "            or the rotation in degrees, if rot- used." << std::endl
			<< "    " << "detector:   Feature detector, e.g. AGAST, or BRISK. You can add the "<< std::endl
			<< "    " << "            threshold, e.g. BRISK80 or SURF2000"<< std::endl
			<< "    " << "descriptor: Feature descriptor, e.g. SURF, BRIEF, BRISK or U-BRISK."<< std::endl
			<< "    " << "[descFile]: Optional: files with descriptors to act as detected points."<< std::endl;
}
