/*
 *      Author: alexanderb
 */

// Include Guard
#ifndef HARRIS_H
#define HARRIS_H

#include <opencv2/opencv.hpp>

#include "util.h"

using namespace std;
using namespace cv;

#define ASSERTIONS_ON false
#define LDPC_ON false
#define TMR_ON true
#define ABFT_ON false

#define DATA_COLLECTION_MODE false

#if TMR_ON

  typedef struct state { // Each var saved is a checkpoint
    Mat original;// A may seem like do not need to save original image because it does not change, but saving it prevents imread
    Mat grey_actual;
    Mat greyA; // B
    Mat greyB; // B
    Mat greyC; // B
    Mat derivx_actual;
    Mat derivxA;// C
    Mat derivxB;// C
    Mat derivxC;// C
    Mat derivy_actual;
    Mat derivyA;  //C
    Mat derivyB;  //C
    Mat derivyC;  //C
    Mat derivxy_actual;
    Mat derivxyA; //C
    Mat derivxyB; //C
    Mat derivxyC; //C
    Mat mderivx_actual;
    Mat mderivxA;// D
    Mat mderivxB;// D
    Mat mderivxC;// D
    Mat mderivy_actual;
    Mat mderivyA;  //D
    Mat mderivyB;  //D
    Mat mderivyC;  //D
    Mat mderivxy_actual;
    Mat mderivxyA; //D
    Mat mderivxyB; //D
    Mat mderivxyC; //D
    Mat corners_actual;
    Mat cornersA; //E
    Mat cornersB; //E
    Mat cornersC; //E
  } state_t;

  Mat validate_tmr(Mat a, Mat b, Mat c) { //TMR logic
  if (!a.empty() && !b.empty() && !c.empty()) {
    // compare each with some kind of operator
    double voteA = cv::sum(a)[0];
    double voteB = cv::sum(b)[0];
    double voteC = cv::sum(c)[0];
    if (voteA == voteB == voteC)  { // all matrices are equal, no data errors
      return c;
    } else if (voteA == voteB)  { // two of them are equal, likely error in C
      return b;
    } else if (voteB == voteC)  { // two of them are equal, likely error in A
      return c;
    } else if (voteA == voteC)  { // two of them are equal, likely error in B
      return c;
    }
    // A != B != C

    return c; // c is potentially broken because none of them are equal
    //
  } else {
    return a;
  }
}

// validate_state - ...
void validate_state(state_t& state);

#endif

class Harris {
public:
  Harris(Mat img, float k, int filterRange, bool gauss);
	vector<pointData> getMaximaPoints(float percentage, int filterRange, int suppressionRadius);
  #if TMR_ON
    state ck;
  #endif
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
};


#endif
