/**
 * @file NaturalLandmarkSensorModel.h
 *
 * Sensor model for updating samples by perceived keypoint matches
 *
 * @author Daniel Mankowitz
 */

#pragma once

/**
 * @class NaturalLandmarkSensorModel
 */

class NaturalLandmarkSensorModel: public SensorModel
{

private:
	/** Declare the natural landmark percept which contains a list of all
	 * the key points and the matching score
	 */
	//const NaturalLandmarkPercept &theNaturalLandmarkPercept;
	const NaturalLandmarkPerceptBrisk& theNaturalLandmarkPerceptBrisk;
	/** The threshold used to determine if the 2 images are a match	 */
	int threshold;

	ENUM(HeadDirection,
			FACING_LEFT,
			FACING_RIGHT,
			FACING_FRONT
	);

public:
	/** Constructor. */
//		NaturalLandmarkSensorModel(const SelfLocatorParameter& selfLocatorParameter,
//				const NaturalLandmarkPercept& naturalLandmarkPercept, const FrameInfo& frameInfo,
//				const FieldDimensions& fieldDimensions, const CameraMatrix& cameraMatrix,
//				const PerceptValidityChecker& perceptValidityChecker):
//					SensorModel(selfLocatorParameter, frameInfo, fieldDimensions, cameraMatrix,
//							perceptValidityChecker, Observation::NATURAL_LANDMARKS),
//							theNaturalLandmarkPercept(naturalLandmarkPercept)
//		{}

	NaturalLandmarkSensorModel(const SelfLocatorParameter& selfLocatorParameter,
			const NaturalLandmarkPerceptBrisk& naturalLandmarkPerceptBrisk, const FrameInfo& frameInfo,
			const FieldDimensions& fieldDimensions, const CameraMatrix& cameraMatrix,
			const PerceptValidityChecker& perceptValidityChecker) :
				SensorModel(selfLocatorParameter, frameInfo, fieldDimensions, cameraMatrix,
						perceptValidityChecker, Observation::NATURAL_LANDMARKS),
						theNaturalLandmarkPerceptBrisk(naturalLandmarkPerceptBrisk)
	{}

	/**This function will compute the weighting for particles depending on whether
	there was a match with one of the images representing the opponents goal.
	It takes all three head directions into account assuming the robot is placed anywhere
	on the field. However, the assumption is that if the robot is placed away
	from the half-way line, then it is placed facing the opponents goal. A routine
	could be added to make the robot turn around if it cannot find any matches.
	This would be initiated if the robot has tried to find matches twice and cannot
	seem to establish a match. */
	SensorModelResult computeWeightings(const SampleSet<SelfLocatorSample>& samples,
			const vector<int>& selectedIndices, vector<float>& weightings)
	{
		//Initialise the weights
		float strongWeight = 0.999;
		float epsilonWeight = 0.001;

		//Note that this function needs to know the orientation of the robot's head
		HeadDirection headDirection;

		//Determines whether or not a match has been found
		//bool matchFound = theNaturalLandmarkPercept.matchFound;
		bool matchFound = theNaturalLandmarkPerceptBrisk.matchFound;

		if (matchFound)
		{
			SensorModelResult result = FULL_SENSOR_UPDATE;
			for(int i = 0; i < samples.size(); ++i)
			{
				const SelfLocatorSample& s(samples.at(i));
				//			const Vector2<int> origin(int(s.translation.x), int(s.translation.y)),
				//					camRotation((rotCos * s.rotation.x - rotSin * s.rotation.y) >> 10,
				//							(rotCos * s.rotation.y + rotSin * s.rotation.x) >> 10);

				weightings[i] = 1.0f;

				//The weightings are updated here
				if(headDirection==FACING_LEFT){
					if(s.translation.y<0)
						weightings[i] *= epsilonWeight;
					else
						weightings[i] *= strongWeight;
				}
				else if(headDirection==FACING_RIGHT){
					if(s.translation.y>0)
						weightings[i] *= epsilonWeight;
					else
						weightings[i] *= strongWeight;
				}
				else if(headDirection==FACING_FRONT){
					if(s.translation.x>0)
						weightings[i] *= epsilonWeight;
					else
						weightings[i] *= strongWeight;
				}
			}


			return result;
		}
		else
			return NO_SENSOR_UPDATE;

	}


};
