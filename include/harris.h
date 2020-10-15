#ifndef _HARRIS_H_
#define _HARRIS_H_

/*
 *      Author: alexanderb
 */

#include <opencv2/opencv.hpp>

#include "util.h"
#include "../include/ldpc.h"

using namespace std;
using namespace cv;

#define ASSERTIONS_ON false
#define LDPC_ON true
#define CHECKPOINTING_ON false
#define ABFT_ON false

class Harris {
public:
    Harris(Mat img, float k, int filterRange, bool gauss);
	vector<pointData> getMaximaPoints(float percentage, int filterRange, int suppressionRadius);

private:
	Mat convertRgbToGrayscale(Mat& img);
	Derivatives computeDerivatives(Mat& greyscaleImg);	
	Derivatives applyMeanToDerivatives(Derivatives& dMats, int filterRange);
	Derivatives applyGaussToDerivatives(Derivatives& dMats, int filterRange);
	Mat computeHarrisResponses(float k, Derivatives& intMats);

	Mat computeIntegralImg(Mat& img);
	Mat meanFilter(Mat& intImg, int range);
	Mat gaussFilter(Mat& img, int range);

private:
	Mat m_harrisResponses;
	LDPC ldpc;
};

#endif // _HARRIS_H_