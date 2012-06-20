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
	 * the key points
	 */
	const NaturalLandmarkPercept &theNaturalLandmarkPercept;

public:
	/** Constructor. */
	NaturalLandmarkSensorModel(const SelfLocatorParameter& selfLocatorParameter,
			const NaturalLandmarkPercept& naturalLandmarkPercept, const FrameInfo& frameInfo,
			const FieldDimensions& fieldDimensions, const CameraMatrix& cameraMatrix,
			const PerceptValidityChecker& perceptValidityChecker):
				SensorModel(selfLocatorParameter, frameInfo, fieldDimensions, cameraMatrix,
						perceptValidityChecker, Observation::NATURAL_LANDMARKS),
						theNaturalLandmarkPercept(naturalLandmarkPercept)
	{}

	SensorModelResult computeWeightings(const SampleSet<SelfLocatorSample>& samples,
			const vector<int>& selectedIndices, vector<float>& weightings)
	{
		return FULL_SENSOR_UPDATE;
	}


};
