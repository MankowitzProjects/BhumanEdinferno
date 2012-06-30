#include "include/FeatureExtraction.h"

#define PI 3.14159265
#define FEATURE_DEBUG_MODE 0
#define FEATURE_DEBUG_MATCHES 0

FeatureExtraction::FeatureExtraction()
{
	//ctor
}

FeatureExtraction::~FeatureExtraction()
{
	//d
}

cv::Ptr<cv::FeatureDetector> FeatureExtraction::getDetector(int argc, char ** argv, cv::Ptr<cv::FeatureDetector> detector, int threshold, int testThreshold, int testFlag){

	if(argc==1){
		detector = new cv::BriskFeatureDetector(60,4);
	}
	else{
		if(strncmp("FAST", argv[3], 4 )==0){
			threshold = atoi(argv[3]+4);
			if(threshold==0)
				threshold = 30;
			detector = new cv::FastFeatureDetector(threshold,true);
		}
		else if(strncmp("AGAST", argv[3], 5 )==0){
			threshold = atoi(argv[3]+5);
			if(threshold==0)
				threshold = 30;
			detector = new cv::BriskFeatureDetector(threshold,0);
		}
		else if(strncmp("BRISK", argv[3], 5 )==0){

			if(testFlag==1)
				threshold = atoi(argv[3]+5);
			else
				threshold = testThreshold;

			if(threshold==0)
				threshold = 30;
			detector = new cv::BriskFeatureDetector(threshold,4);
		}
		else if(strncmp("SURF", argv[3], 4 )==0){
			threshold = atoi(argv[3]+4);
			if(threshold==0)
				threshold = 400;
			detector = new cv::SurfFeatureDetector(threshold);
		}
		else if(strncmp("SIFT", argv[3], 4 )==0){
			float thresh = 0.04 / cv::SIFT::CommonParams::DEFAULT_NOCTAVE_LAYERS / 2.0;
			float edgeThreshold=atof(argv[3]+4);
			if(edgeThreshold==0)
				thresh = 10.0;
			detector = new cv::SiftFeatureDetector(thresh,edgeThreshold);
		}
		else{
			detector = cv::FeatureDetector::create( argv[3] );
		}
		if (detector.empty()){
			std::cout << "Detector " << argv[3] << " not recognized. Check spelling!" << std::endl;
			return detector;
		}
	}

	return detector;

}

cv::Ptr<cv::DescriptorExtractor> FeatureExtraction::getExtractor(int argc, char** argv,bool hamming, cv::Ptr<cv::DescriptorExtractor> descriptorExtractor)
{
	if(argc==1){
		descriptorExtractor = new cv::BriskDescriptorExtractor();
	}
	else{
		if(std::string(argv[4])=="BRISK"){
			descriptorExtractor = new cv::BriskDescriptorExtractor();
		}
		else if(std::string(argv[4])=="U-BRISK"){
			descriptorExtractor = new cv::BriskDescriptorExtractor(false);
		}
		else if(std::string(argv[4])=="SU-BRISK"){
			descriptorExtractor = new cv::BriskDescriptorExtractor(false,false);
		}
		else if(std::string(argv[4])=="S-BRISK"){
			descriptorExtractor = new cv::BriskDescriptorExtractor(true,false);
		}
		else if(std::string(argv[4])=="BRIEF"){
			descriptorExtractor = new cv::BriefDescriptorExtractor(64);
		}
		else if(std::string(argv[4])=="CALONDER"){
			descriptorExtractor = new cv::CalonderDescriptorExtractor<float>("current.rtc");
			hamming=false;
		}
		else if(std::string(argv[4])=="SURF"){
			descriptorExtractor = new cv::SurfDescriptorExtractor();
			hamming=false;
		}
		else if(std::string(argv[4])=="SIFT"){
			descriptorExtractor = new cv::SiftDescriptorExtractor();
			hamming=false;
		}
		else{
			descriptorExtractor = cv::DescriptorExtractor::create( argv[4] );
		}
		if (descriptorExtractor.empty()){
			hamming=false;
			std::cout << "Descriptor " << argv[4] << " not recognized. Check spelling!" << std::endl;
			return descriptorExtractor;
		}
	}
	return descriptorExtractor;



}

cv::Ptr<cv::FeatureDetector> FeatureExtraction::getDetector(int argc, string feat_detector, cv::Ptr<cv::FeatureDetector> detector, int threshold, int testThreshold, int testFlag){


	if("FAST" == feat_detector){
		if(threshold==0)
			threshold = 30;
		detector = new cv::FastFeatureDetector(threshold,true);
	}
	else if("AGAS100T"== feat_detector){
		if(threshold==0)
			threshold = 30;
		detector = new cv::BriskFeatureDetector(threshold,0);
	}
	else if("BRISK"== feat_detector){
		//cout<<"Reached Here"<<endl;
		//Used for the threshold testing
		if(testFlag==2)
			threshold = testThreshold;
		if(threshold==0)
			threshold = 30;
		//The detector uses 0 layers. This removes the need to perform non-maximal suppression
		//on multiple layers. Sub-pixel refinement is also not necessary
		detector = new cv::BriskFeatureDetector(threshold,0);//Should be 4
	}
	else if("SURF"== feat_detector){
		if(threshold==0)
			threshold = 400;
		detector = new cv::SurfFeatureDetector(threshold);
	}
	else if("SIFT"== feat_detector){
		float thresh = 0.04 / cv::SIFT::CommonParams::DEFAULT_NOCTAVE_LAYERS / 2.0;
		float edgeThreshold=threshold;
		if(edgeThreshold==0)
			thresh = 10.0;
		detector = new cv::SiftFeatureDetector(thresh,edgeThreshold);
	}
	else if("OPENSURF"== feat_detector){
		//Initialise the 1D SURF detector
	}
	else{
		detector = cv::FeatureDetector::create( feat_detector );
	}
	if (detector.empty()){
		std::cout << "Detector " << feat_detector << " not recognized. Check spelling!" << std::endl;
		return detector;
	}

	return detector;

}

cv::Ptr<cv::DescriptorExtractor> FeatureExtraction::getExtractor(int argc, string feat_descriptor,bool hamming, cv::Ptr<cv::DescriptorExtractor> descriptorExtractor)
{

	if(feat_descriptor=="BRISK"){
		descriptorExtractor = new cv::BriskDescriptorExtractor();
	}
	else if(feat_descriptor=="U-BRISK"){
		descriptorExtractor = new cv::BriskDescriptorExtractor(false);
	}
	else if(feat_descriptor=="SU-BRISK"){
		descriptorExtractor = new cv::BriskDescriptorExtractor(false,false);
	}
	else if(feat_descriptor=="S-BRISK"){
		descriptorExtractor = new cv::BriskDescriptorExtractor(true,false);
	}
	else if(feat_descriptor=="BRIEF"){
		descriptorExtractor = new cv::BriefDescriptorExtractor(64);
	}
	else if(feat_descriptor=="CALONDER"){
		descriptorExtractor = new cv::CalonderDescriptorExtractor<float>("current.rtc");
		hamming=false;
	}
	else if(feat_descriptor=="SURF"){
		cout<<"Reached Here SURF"<<endl;
		descriptorExtractor = new cv::SurfDescriptorExtractor();
		hamming=false;
	}
	else if(feat_descriptor=="SIFT"){
		descriptorExtractor = new cv::SiftDescriptorExtractor();
		hamming=false;
	}
	else if(feat_descriptor=="OPENSURF"){
		//Create the 1D SURF Descriptor
	}
	else{
		descriptorExtractor = cv::DescriptorExtractor::create( feat_descriptor );
	}
	if (descriptorExtractor.empty()){
		hamming=false;
		std::cout << "Descriptor " << feat_descriptor << " not recognized. Check spelling!" << std::endl;
		return descriptorExtractor;
	}

	return descriptorExtractor;



}

void FeatureExtraction::performMatchingValidation(const cv::Mat & img, std::vector<cv::KeyPoint> &keypoints, std::vector<cv::KeyPoint> &keypoints2, std::vector<std::vector<cv::DMatch> > &matches, bool hamming)
{

	//Initialise the scoring variables
	imageMatchingScore = 0;
	imageMatchingScoreBest = 0;
	totalNumMatches = 0;
	totalNumValidMatches = 0;
	totalNumInvalidMatches = 0;
	totalNumBestMatches = 0;

	int allMatches = 0;
	int counter = 0;
	int matchSize = 0;
	bool isTrueMatchFound= false;

	//For matching
	int i1 = -1;
	int i2 = -1;
	float distanceMatch = 0;
	float matchingScore = 0;
	bool correctMatch = false;

#if (FEATURE_DEBUG_MATCHES)
	cout<<"The total number of keypoints in image 1 is: "<<keypoints.size()<<endl;
	cout<<"The total number of keypoints in image 2 is: "<<keypoints2.size()<<endl;
#endif

	//Loops through all of the matches
	for( size_t i = 0; i < matches.size(); i++ )
	{

		//Verify the Knn Neighbors matching criteria
		if (matches[i].size()>0){

			verifyKNNMatches(matches[i]);//Check if the KNN ratio holds

#if (FEATURE_DEBUG_MATCHES)
			cout<<"The index is: "<<i<<endl;
			cout<<"Is the knn criteria met? "<<isKnnMatch<<endl;
			cout<<"The number of matches is : "<<matches[i].size()<<endl;
			cout<<"****************************************"<<endl;
#endif
		}

		//Initialise the variables
		allMatches = matches[i].size();
		counter = 0;
		matchSize = matches[i].size();
		isTrueMatchFound = false;

		//Check if the matches satisifies the Knn Criteria
		if(isKnnMatch)
		{
			//If it satisfies the Knn criteria, then check the angle and distance criteria
			while(counter<matchSize)
			{
				//Corresponds to the left image. Used as the reference matching point
				//Therefore all matches are relative to this point
				i1 = matches[i][counter].queryIdx;

				//Corresponds to the right image
				i2 = matches[i][counter].trainIdx;

				//Determine the distance between matches
				distanceMatch = matches[i][counter].distance;

				//Give a constant reward for being under a certain threshold
				matchingScore = 0;
				if (distanceMatch==0)
					matchingScore=100;
				else{
					if (hamming)
						matchingScore = 1/distanceMatch;
					else
						matchingScore = 1/distanceMatch;
				}


#if (FEATURE_DEBUG_MATCHES)
				cout<<"*************"<<endl;
				cout<<"All matching keypoints: "<<matches.size()<<endl;
				cout<<"The number of matches is : "<<matches[i].size()<<endl;
				cout<<"Match Index: "<<i<<endl;
				cout<<"Counter Number: "<<counter<<endl;
				cout<<"Query Index: "<<i1<<endl;
				cout<<"Train Index: "<<i2<<endl;
				cout<<"*************"<<endl;
				cout<<"Keypoint indices i1, i2: "<<i1<<", "<<i2<<endl;
				cout<<"****************************"<<endl;
				cout<<"Keypoint Right  row,col : "<<(*(keypoints2.begin()+i2)).pt.y<<", "<<(*(keypoints2.begin()+i2)).pt.x<<endl;
				cout<<"Keypoint Left row,col : "<<(*(keypoints.begin() + i1)).pt.y<<", "<<(*(keypoints.begin() + i1)).pt.x<<endl;
				cout<<"****************************"<<endl;
#endif

#if (FEATURE_DEBUG_MATCHES)
				//dataAnalysis.displayOutput(keypoints2, keypoints, matchingScore, i1, i2);
#endif

				//Verify whether the match is correct or not based on angles and length of the match
				//****************************************************
				correctMatch = verifyMatch(img, keypoints[i1], keypoints2[i2]);
				//****************************************************

#if (FEATURE_DEBUG_MATCHES)
				cout<<"CorrectMatch: "<<correctMatch<<endl;
#endif
				//****************************************************
				//If the match is incorrect, remove the invalid match
				if (correctMatch==false)
				{
#if (FEATURE_DEBUG_MATCHES)
					cout<<"Keypoint Left to be erased row,col : "<<(*(keypoints.begin() + i1)).pt.y<<", "<<(*(keypoints.begin() + i1)).pt.x<<endl;
					cout<<"Keypoint Right to be erased row,col : "<<(*(keypoints2.begin()+i2)).pt.y<<", "<<(*(keypoints2.begin()+i2)).pt.x<<endl;
					cout<<"The number of matches before removal is : "<<matches[i].size()<<endl;
					cout<<"-----------------------------------------"<<endl;
					//Erase the corresponding match
					int delRight = (*(matches[i].begin()+counter)).trainIdx;
					int delLeft = (*(matches[i].begin()+counter)).queryIdx;

					double keyLeftR = (*(keypoints.begin()+delLeft)).pt.y;
					double keyLeftC = (*(keypoints.begin()+delLeft)).pt.x;

					double keyRightR = (*(keypoints2.begin()+delRight)).pt.y;
					double keyRightC = (*(keypoints2.begin()+delRight)).pt.x;

					cout<<"The Left image coordinate to be erased row, col is: "<<keyLeftR<<", "<<keyLeftC<<endl;
					cout<<"The right image coordinate to be erased row, col is: "<<keyRightR<<", "<<keyRightC<<endl;
#endif

					//Erase the matches
					//***************************************************
					matches[i].erase(matches[i].begin()+counter);
					//***************************************************

					//Get the size of the new match
					matchSize = matches[i].size();

#if (FEATURE_DEBUG_MATCHES)
					cout<<"The number of matches after removal is : "<<matchSize<<endl;
					cout<<"counter is : "<<counter<<endl;
					cout<<"----------------------------------------"<<endl;
#endif
					//Increment the total number of invalid matches
					totalNumInvalidMatches = totalNumInvalidMatches + 1;

#if (FEATURE_DEBUG_MATCHES)
					//This stores the coordinates of the invalid matches
					rightPoints.push_back(keypoints2[i2].pt);
					leftPoints.push_back(keypoints[i1].pt);
#endif
				}
				else
				{
					isTrueMatchFound = true;
					counter++;
				}

				//If all matches were deleted from the vector, then skip
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

		}//End if KnnMatches Criteria
		else{
			//Remove matches if there are matches to be removed
			if(matches[i].size()>0)
				for(size_t j=0;j<=matches[i].size();j++){
					matches[i].erase(matches[i].begin()+j);
				}

		}

		totalNumValidMatches = totalNumValidMatches + matches[i].size();
		totalNumMatches = totalNumMatches + allMatches;
	}

}

//Needs the image size and the keypoints
bool FeatureExtraction::verifyMatch(const cv::Mat & image,cv::KeyPoint &keypoint1, cv::KeyPoint &keypoint2)
{

	//Store the keypoint coordinates
	//Image Left
	int x1 = keypoint1.pt.x;//col
	int y1 = keypoint1.pt.y;//row


	//Image Right
	int x2 = keypoint2.pt.x;//col
	int y2 = keypoint2.pt.y;//row


#if (FEATURE_DEBUG_MODE)
	cout<<"rowL, colL: "<<y1<<", "<<x1<<endl;
	cout<<"rowR, colR: "<<y2<<", "<<x2<<endl;
#endif

	//Store the image col and rows
	int rows = image.rows;
	int cols = image.cols;

#if (FEATURE_DEBUG_MODE)
	cout<<"image rows: "<<rows<<endl;
	cout<<"image cols: "<<cols<<endl;
#endif

	//Find the absolute distance between the two coordinates if the images were placed together
	double xdistance, ydistance;
	xdistance = (cols - x1) + x2;//cols
	ydistance = y2-y1;//rows

#if (FEATURE_DEBUG_MODE)
	cout<<"col distance: "<<xdistance<<endl;
	cout<<"row distance: "<<ydistance<<endl;
#endif

	//Calculate the angle of the line
	double angle = 0;
	angle = atan2(ydistance, xdistance)*180/PI;

#if (FEATURE_DEBUG_MODE)
	cout<<"angle: "<<angle<<endl;
	//cout<<"Negative angle: "<<atan2(-10, 50)*180/PI<<endl;
#endif

	//If the angle is greater than a threshold, then invalid match
	if((angle<-10 || angle > 10) || (xdistance>(640+200) || xdistance<(640-200)))
	{
		return false;
	}
	else
		return true;

}

void FeatureExtraction::verifyKNNMatches(std::vector<cv::DMatch> &matches){

	dist1 = 0;
	dist2 = 0;
	//Get the distances between a keypoint and its nearest neighbors
	dist1 = matches[0].distance;
	dist2 = matches[1].distance;

	//Determine the ratio between the two distances
	distanceRatio = dist1/dist2;

	//If the ratio is greater than 0.7, then it is probably a false match
	//This means that the points are close together and the second match is
	//probably noise
	if(distanceRatio > 0.7)
		isKnnMatch = false;
	else
		isKnnMatch = true;
}

//Verfify that a match is indeed correct
//Needs the image size and the keypoints
void FeatureExtraction::verifyMatchingOrder(const cv::Mat & image,cv::Mat descriptors, cv::Mat descriptors2, std::vector<std::vector<cv::DMatch> > &matches)
{
	//Declare a matrix to store the matching scores between features
	std::vector<std::vector<float> > Eij;
	std::vector<std::vector<int> > Mij;


	//Initialise the 0 indices to 0 as the feature vectors are assumed to be 1-indexed
	//First initialise all rows, column 1 to 0
	for (int ii=0;ii<=descriptors.rows;ii++)
		Eij[ii][0] = 0;
	//Then initialise all columns, row 0 to 0
	for (int jj=0;jj<descriptors2.rows;jj++)
		Eij[0][jj] = 0;

	//Maximise the matching score by considering all combinations of matches
	for (int ii = 0;ii<=descriptors.rows;ii++)
	{
		for (int jj = 0;jj<descriptors2.rows;jj++)
		{
			//Calculate the matching score between the current pair of feature vectors
			double scoreij = calcEuclideanDistance(descriptors, descriptors2);

			if(Eij[ii-1][jj]>Eij[ii][jj-1] && Eij[ii-1][jj] > (Eij[ii-1][jj-1] + scoreij))
			{
				//The maximum is vertically below the current feature pairing
				Eij[ii][jj] = Eij[ii-1][jj];
				Mij[ii][jj] = 3;
			}else if(Eij[ii][jj-1]>Eij[ii-1][jj] && Eij[ii][jj-1] > (Eij[ii-1][jj-1] + scoreij))
			{
				//The maximum is horizontally to the left of the current feature pairing
				Eij[ii][jj] = Eij[ii][jj-1];
				Mij[ii][jj] = 2;
			}else if ((Eij[ii-1][jj-1] + scoreij) >Eij[ii-1][jj] &&  Eij[ii-1][jj-1] + scoreij > Eij[ii][jj-1])
			{
				//The maximum is diagonally away from the current feature pairing
				Eij[ii][jj] = Eij[ii-1][jj-1] + scoreij;
				Mij[ii][jj] = 1;
			}
		}
	}

	//Now to extract the optimum feature matching sequence
	int kp1Index = descriptors.rows;
	int kp2Index = descriptors2.rows;

	while(kp1Index!=0 && kp2Index!=0){

		switch(Mij[kp1Index][kp2Index]){

		case 1:
			//ip1Index matches ip2Index
			//TO DO
			//matches.push_back(std::make_pair(ipts1[kp1Index], ipts2[kp2Index]));
			kp1Index--; kp2Index--;
			break;
		case 2:
			//ip2Index is unmatched
			kp2Index--;
			break;
		case 3:
			//ip1Index is unmatched
			kp1Index--;
			break;
		}
	}



}

double FeatureExtraction::calcEuclideanDistance(cv::Mat d1, cv::Mat d2)
{

////	template<class Distance>
////	inline void BruteForceMatcher<Distance>::commonRadiusMatchImpl( BruteForceMatcher<Distance>& matcher,
////	                             const Mat& queryDescriptors, vector<vector<DMatch> >& matches, float maxDistance,
////	                             const vector<Mat>& masks, bool compactResult )
////	{
//	//BruteForceMatcher<Distance>& matcher;
//	    typedef typename Distance::ValueType ValueType;
//	    typedef typename Distance::ResultType DistanceType;
//	for( int qIdx = 0; qIdx < queryDescriptors.rows; qIdx++ )
//	    {
//	        if( matcher.isMaskedOut( masks, qIdx ) )
//	        {
//	            if( !compactResult ) // push empty vector
//	                matches.push_back( vector<DMatch>() );
//	        }
//	        else
//	        {
//	            matches.push_back( vector<DMatch>() );
//	            vector<vector<DMatch> >::reverse_iterator curMatches = matches.rbegin();
//	            for( size_t iIdx = 0; iIdx < imgCount; iIdx++ )
//	            {
//	                CV_Assert( DataType<ValueType>::type == matcher.trainDescCollection[iIdx].type() ||
//	                           matcher.trainDescCollection[iIdx].empty() );
//	                CV_Assert( queryDescriptors.cols == matcher.trainDescCollection[iIdx].cols ||
//							   matcher.trainDescCollection[iIdx].empty() );
//
//	                const ValueType* d1 = (const ValueType*)(queryDescriptors.data + queryDescriptors.step*qIdx);
//	                for( int tIdx = 0; tIdx < matcher.trainDescCollection[iIdx].rows; tIdx++ )
//	                {
//	                    if( masks.empty() || matcher.isPossibleMatch(masks[iIdx], qIdx, tIdx) )
//	                    {
//	                        const ValueType* d2 = (const ValueType*)(matcher.trainDescCollection[iIdx].data +
//	                                                                 matcher.trainDescCollection[iIdx].step*tIdx);
//	                        DistanceType d = matcher.distance(d1, d2, dimension);
//	                        if( d < maxDistance )
//	                            curMatches->push_back( DMatch( qIdx, tIdx, (int)iIdx, (float)d ) );
//	                    }
//	                }
//	            }
//	            std::sort( curMatches->begin(), curMatches->end() );
//	        }
//	    }
	return 1;
}
