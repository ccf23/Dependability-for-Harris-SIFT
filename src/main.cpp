/*
 *      Author: alexanderb
 */

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <chrono>
using namespace std::chrono;

#include "../include/harris.h"

using namespace cv;
using namespace std;

//Harris algorithm parameters
// Specifies the sensitivity factor of the Harris algorithm (0 < k < 0.25)
float k = 0.25;
// Size of the box filter that is applied to the integral images
int boxFilterSize = 3;
// dimension of the maxima suppression box around a maxima
int maximaSuppressionDimension = 10;

//UI parameters
// dimension of the objects showing a maxima in the image
int markDimension = 5;
// constant for the slider-value division
float divisionConstant = 1000000;

//Global variables
int slider_valueMaxsp = 10;
int slider_valueMeanfilter = 2;
int slider_valuePercentage = 1000;
float percentage = 50e-5;
bool gauss = true;
Mat m_img;
string filename;

void doHarris() {
    
    // compute harris
    auto t_before = high_resolution_clock::now();
    Harris harris(m_img, k, boxFilterSize, gauss);
    auto t_after = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(t_after - t_before);
    cout << "Total time to compute Harris: " << duration.count() << " us" << endl;

    // get vector of points wanted
    t_before = high_resolution_clock::now();
    vector<pointData> resPts = harris.getMaximaPoints(percentage, boxFilterSize, maximaSuppressionDimension);
    t_after = high_resolution_clock::now();
    duration = duration_cast<microseconds>(t_after - t_before);
    cout << "Total time to get vector of points: " << duration.count() << " us" << endl;
    // cout << resPts.size() << " Points" << endl;

    t_before = high_resolution_clock::now();
    Mat _img = Util::MarkInImage(m_img, resPts, markDimension);
    t_after = high_resolution_clock::now();
    duration = duration_cast<microseconds>(t_after - t_before);
    cout << "Time to mark image: " << duration.count() << " us" << endl;

    imshow("HarrisCornerDetector", _img);
}

//-----------------------------------------------------------------------------------------------
int main(int argc, char** argv) {
    // read image from file + error handling
    Mat img;

    if (argc == 1) {
        cout << "No image provided! Usage: ./Ex1 [path to image]" << endl << "Using default image: haus.jpg" << endl;

        img = imread("haus.jpg");
    } else {
        filename = argv[1];
        img = imread(argv[1]);
    }

    img.copyTo(m_img);

    auto t_before = high_resolution_clock::now();
    doHarris();
    auto t_after = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(t_after - t_before);
    cout << "Total execution time: " << duration.count() << " us" << endl;
    
    waitKey(0);

    return 0;

}





// ----- OLD Stuff from automation -----
/*
//-----------------------------------------------------------------------------------------------
void CallbackTrackbarMaxsp(int value, void* userdata) {
    maximaSuppressionDimension = slider_valueMaxsp;
    cout << "maximaSuppressionDimension:" << maximaSuppressionDimension << endl;

    doHarris();
}

//-----------------------------------------------------------------------------------------------
void CallbackTrackbarMeanfilter(int value, void* userdata) {
    boxFilterSize = slider_valueMeanfilter;

    cout << "boxFilterSize:" << boxFilterSize << endl;

    doHarris();
}

//-----------------------------------------------------------------------------------------------
void CallbackTrackbarPercentage(int value, void* userdata) {
    // calculate percentage of the points with top-harris response wished to display
    percentage = slider_valuePercentage / (float) divisionConstant;

    cout << "percentage:" << percentage << endl;

    doHarris();
}

void CallBackFuncMouse(int event, int x, int y, int flags, void* userdata) {
    if  ( event == EVENT_LBUTTONDOWN ) {
        if(gauss) {
            cout << "Changed to gaussian filter." << endl;
            gauss = false;
        } else {
            cout << "Changed to mean filter." << endl;
            gauss = true;
        }

        doHarris();
    }
}
*/
