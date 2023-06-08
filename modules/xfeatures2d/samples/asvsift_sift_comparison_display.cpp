#include "/usr/local/include/opencv4/opencv2/core.hpp"
#include "/usr/local/include/opencv4/opencv2/highgui.hpp"
#include "/usr/local/include/opencv4/opencv2/imgproc.hpp"
#include "/usr/local/include/opencv4/opencv2/features2d.hpp"
#include "/usr/local/include/opencv4/opencv2/xfeatures2d.hpp"
#include "/usr/local/include/opencv4/opencv2/xfeatures2d/nonfree.hpp"

#include <iostream>

using namespace std;
using namespace cv;

static const int SCALE = 2; // window size scale

// main - Examine matches created from two different descriptors.
int main(int argc, char* argv[])
{
    // Read in the input images
    Mat img1 = imread("../datasets/fischer/09_cool_car/_original.ppm");
    Mat img2 = imread("../datasets/fischer/09_cool_car/zoom6.ppm");

    // Create two detectors/descriptors.
    Ptr<SIFT> detectorSIFT = SIFT::create();

    Ptr<xfeatures2d::ASVSIFT> detectorASV = xfeatures2d::ASVSIFT::create();

    /*
       Ptr<xfeatures2d::ASVSIFT> detectorASV = 
       xfeatures2d::ASVSIFT::create(0, 3, 0.04, 10, 1.6,
       50, (1.0f / 6.0f), 3.0f);
       */

    // Declare keypoints for each image.
    vector<KeyPoint> siftKpts1; 
    vector<KeyPoint> siftKpts2;

    vector<KeyPoint> asvKpts1; 
    vector<KeyPoint> asvKpts2;

    // Declare descriptors for each image.
    Mat siftDesc1;
    Mat siftDesc2;

    Mat asvDesc1;
    Mat asvDesc2;

    // Obtain both keypoints and descriptors from both input images.
    detectorSIFT->detectAndCompute(img1, noArray(), siftKpts1, siftDesc1);
    detectorSIFT->detectAndCompute(img2, noArray(), siftKpts2, siftDesc2);

    detectorASV->detectAndCompute(img1, noArray(), asvKpts1, asvDesc1);
    detectorASV->detectAndCompute(img2, noArray(), asvKpts2, asvDesc2);

    // Draw the keypoints over the input images.
    Mat siftImg1 = img1.clone();
    Mat siftImg2 = img2.clone();

    Mat asvImg1 = img1.clone();
    Mat asvImg2 = img2.clone();

    drawKeypoints(img1, siftKpts1, siftImg1);
    drawKeypoints(img2, siftKpts2, siftImg2);

    drawKeypoints(img1, asvKpts1, asvImg1);
    drawKeypoints(img2, asvKpts2, asvImg2);

    // Create a brute-force matcher.
    Ptr<BFMatcher> siftMatcher = BFMatcher::create();
    Ptr<BFMatcher> asvMatcher = BFMatcher::create();

    // Compute matches between the keypoints/descriptors in the two images.
    vector<DMatch> siftMatches;
    vector<DMatch> asvMatches;

    siftMatcher->match(siftDesc1, siftDesc2, siftMatches);
    asvMatcher->match(asvDesc1, asvDesc2, asvMatches);

    // Draw the matches.
    Mat img_matchesSIFT;
    Mat img_matchesASV;

    drawMatches(siftImg1, siftKpts1, siftImg2, siftKpts2, siftMatches, img_matchesSIFT);
    drawMatches(asvImg1, asvKpts1, asvImg2, asvKpts2, asvMatches, img_matchesASV);

    Mat bothMatches;
    vconcat(img_matchesSIFT, img_matchesASV, bothMatches);

    // Display the resulting images side-by-side with the matches.
    namedWindow("SIFT vs. ASV", WINDOW_NORMAL);
    resizeWindow("SIFT vs. ASV", bothMatches.cols / SCALE, bothMatches.rows / SCALE);
    imshow("SIFT vs. ASV", bothMatches);

    // Save the matches to disk.
    imwrite("output.jpg", bothMatches);
    waitKey(0);

    return 0;
}
