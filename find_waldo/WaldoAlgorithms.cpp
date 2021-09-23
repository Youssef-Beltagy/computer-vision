// WaldoAlgortihms.cpp
// Abstract: This file contains the .cpp of the WaldoAlgorithms namespace
//			 This .cpp is split into two categories:
//			 1. CURRENT IMPLEMNTATION
//			 2. DEPRECATED CODE
//			 The current implementation contains the Hough Circle + SIFT algorithm and its helper functions
//			 Section 2 contains the deprecated code used during development of the Hough Circle + SIFT
// Authors: Nathan Pham and Youssef Beltagy
// Revision Date: December 6th, 2019

#include "WaldoAlgorithms.h"
using namespace cv;
using namespace cv::xfeatures2d;
using std::cout;
using std::endl;
using std::vector;
/* 1 BEGIN: CURRENT IMPLEMENTATION*/

// findWaldoWithHoughSIFT
// Preconditions: the Waldo exemplar and world imaes are properly defined Mats
// Postconditions: Displays the results of the SIFT operation of world
//				   to find Waldo based on the locations of the Hough circles
// @isLowResolution: Detect edges in the world for a grainy world image
void WaldoAlgorithms::findWaldoWithHoughSIFT(Mat& waldoExemplar, Mat& world, bool isLowResolution)
{
	Mat edgeWorld;
	Mat filteredWorld = world.clone();
	Mat spareWorld = world.clone();
	const int LOW_RES_THRESH = 450;
	const int DEFAULT_THRESH = 750;
	if (isLowResolution)
		Canny(world, edgeWorld, LOW_RES_THRESH, LOW_RES_THRESH);
	else
		Canny(world, edgeWorld, DEFAULT_THRESH, DEFAULT_THRESH);
	namedWindow("World Edges", WINDOW_NORMAL);
	resizeWindow("Matches", edgeWorld.rows, edgeWorld.cols);
	cv::imshow("World Edges", edgeWorld);
	cv::waitKey();

	// Get all of the circles in the world
	vector<Vec3f> houghCircles;
	const int RESOLUTION_DOWNSIZE = 1;
	const int PIXEL_SPACE_BETWEEN_CIRCLES = 4;
	const int HIGH_GRADIENT_THRESH = 800;
	const int ACCUMULATOR_THRESH = 15;
	const int MIN_CIRCLE_PIXEL_RADIUS = 0;
	const int MAX_CIRCLE_PIXEL_RADIUS = 20;

	HoughCircles(edgeWorld, houghCircles, HOUGH_GRADIENT, RESOLUTION_DOWNSIZE, PIXEL_SPACE_BETWEEN_CIRCLES,
				 HIGH_GRADIENT_THRESH, ACCUMULATOR_THRESH ,MIN_CIRCLE_PIXEL_RADIUS, MAX_CIRCLE_PIXEL_RADIUS);

	if (houghCircles.size() <= 0)
	{
		cout << "The world map is too low resolution to find Waldo. No circles detected" << endl;
		return;
	}

	Mat edgeWaldo;
	Mat spareWaldo = waldoExemplar.clone();
	const int EXEMPLAR_EDGE_THRESH = 500;
	Canny(waldoExemplar, edgeWaldo, EXEMPLAR_EDGE_THRESH, EXEMPLAR_EDGE_THRESH);
	imshow("Waldo Edges", edgeWaldo);
	waitKey();

	// Get all of the circles in the Waldo exemplar
	vector<Vec3f> waldoHoughCircles;
	const int EXEMPLAR_HIGH_GRADIENT_THRESH = 200;
	HoughCircles(edgeWaldo, waldoHoughCircles, HOUGH_GRADIENT, RESOLUTION_DOWNSIZE, PIXEL_SPACE_BETWEEN_CIRCLES,
				 EXEMPLAR_HIGH_GRADIENT_THRESH, ACCUMULATOR_THRESH,
				 MIN_CIRCLE_PIXEL_RADIUS, MAX_CIRCLE_PIXEL_RADIUS);
	for (size_t i = 0; i < waldoHoughCircles.size(); i++)
	{
		circle(spareWaldo, Point(static_cast<int>(waldoHoughCircles[i][0]),
								 static_cast<int>(waldoHoughCircles[i][1])),
						   static_cast<int>(waldoHoughCircles[i][2]),
						   Scalar(0, 255, 0), 1, LINE_AA);
	}
	cv::imshow("Waldo Circles", spareWaldo);
	cv::waitKey();

	// Filter the detected keypoints in the world with colors that are appropriate for Waldo
	// Based on the exemplar image 
	Mat waldoColors;
	Mat mask;
	inRange(waldoExemplar, Scalar(0, 235, 0), Scalar(15, 255, 15), mask); // detect the green screen
	bitwise_not(mask, mask); // invert it to get Waldo Exemplar
	calculateHistograms(waldoExemplar, waldoColors, mask);

	vector<bool> isInvalid;
	for (size_t curCircle = 0; curCircle < houghCircles.size(); curCircle++)
	{
		// Get the submatrix at the circle
		Mat circleSubMatrix;
		getSubMatrix(world, circleSubMatrix, houghCircles[curCircle], false);

		// Create a normalized colored histogram for the submatrix at the keypoint
		Mat circleHist;
		Mat worldMask;
		calculateHistograms(circleSubMatrix, circleHist, worldMask);

		// Calculate the Bhattacharrya Distribution to sense strength of color matching
		double response = compareHist(waldoColors, circleHist, HISTCMP_BHATTACHARYYA);

		// If the is above the threshold for acceptability, it is an invalid keypoint
		// in terms of color
		if (response > 0.85)
		{
			//cout << "Bad Keypoint at index " << curCircle << endl;
			isInvalid.push_back(true);
		}
		else
		{
			//cout << "VALID POINT AT INDEX " << curCircle << endl;
			isInvalid.push_back(false);
		}
	}

	auto invalidIt = isInvalid.begin();
	auto houghCirclesIt = houghCircles.begin();
	while (invalidIt != isInvalid.end())
	{
		// if it is invalid, remove the circle
		if (*invalidIt == true)
		{
			// Remove the entry
			houghCirclesIt = houghCircles.erase(houghCirclesIt);
			invalidIt = isInvalid.erase(invalidIt);
		}
		// The circle is valid and continue on to the next circle
		else
		{
			invalidIt++;
			houghCirclesIt++;
		}
	}

	for (size_t i = 0; i < houghCircles.size(); i++)
	{
		circle(spareWorld, Point(static_cast<int>(houghCircles[i][0]),
								 static_cast<int>(houghCircles[i][1])), static_cast<int>(houghCircles[i][2]),
			   Scalar(0, 255, 0), 1, LINE_AA);
	}
	namedWindow("World Circles", WINDOW_AUTOSIZE);
	cv::imshow("World Circles", spareWorld);
	cv::imwrite("WorldCircles.jpg", spareWorld);
	cv::waitKey();

// For every circle in both the exemplar and the world,
// Perform SIFT on the submatrix equal to radius of the circle
// Calculate the exemplar's descriptors first

// Instantiate the SIFT operations
	int minSamples = 1000;
	Ptr<SIFT> detector = SIFT::create(minSamples);

	// Acquire the cumulative keypoints and descriptors for each circle detected in the exemplar
	vector<Mat> exemplarDescriptors;
	vector<KeyPoint> cumulativeExemplarKeypoints;
	Mat cumulativeExemplarDescriptors;
	Mat showExemplarKpts = waldoExemplar.clone();
	Mat img_matches;
	for (size_t i = 0; i < waldoHoughCircles.size(); i++)
	{
		Mat waldoSubMatrix;
		exemplarDescriptors.push_back(Mat());
		vector<KeyPoint> exemplarKeyPoints;
		getSubMatrix(waldoExemplar, waldoSubMatrix, waldoHoughCircles[i], false);
		detector->detectAndCompute(waldoSubMatrix, noArray(), exemplarKeyPoints, exemplarDescriptors[i]);

		// Copy the descriptors and keypoints to the cumulative
		for (size_t curKpt = 0; curKpt < exemplarKeyPoints.size(); curKpt++)
		{
			// Shift the keypoint's location by its center
			exemplarKeyPoints[curKpt].pt.x += waldoHoughCircles[i][0];
			exemplarKeyPoints[curKpt].pt.y += waldoHoughCircles[i][1];

			cumulativeExemplarKeypoints.push_back(exemplarKeyPoints[curKpt]);
		}
	}

	// Acquire the descriptors and keypoints from the world
	Mat cumulativeWorldDescriptors;
	vector<Mat> subMatrixDescriptors;
	vector<KeyPoint> cumulativeWorldKeyPoints;
	vector<float> circleRadii;

	drawKeypoints(showExemplarKpts, cumulativeExemplarKeypoints, showExemplarKpts, Scalar(127, 127, 127));
	cv::imshow("Waldo Keypoints", showExemplarKpts);
	cv::waitKey();
	vconcat(exemplarDescriptors, cumulativeExemplarDescriptors);

	detector->detectAndCompute(waldoExemplar, noArray(), cumulativeExemplarKeypoints,
		      cumulativeExemplarDescriptors);
	drawKeypoints(showExemplarKpts, cumulativeExemplarKeypoints, showExemplarKpts, Scalar(127, 127, 127));
	cv::imshow("Waldo Keypoints", showExemplarKpts);
	cv::waitKey();

	for (size_t i = 0; i < houghCircles.size(); i++)
	{
		subMatrixDescriptors.push_back(Mat());
		Mat worldSubMatrix;
		getSubMatrix(world, worldSubMatrix, houghCircles[i], false);

		// Detect the keypoints and descriptors for the submatrix
		vector<KeyPoint> subMatrixKeyPoints;
		detector->detectAndCompute(worldSubMatrix, noArray(),
								   subMatrixKeyPoints, subMatrixDescriptors[i]);
		
		// Copy the descriptors and keypoints to the cumulative
		for (size_t curKpt = 0; curKpt < subMatrixKeyPoints.size(); curKpt++)
		{
			// Shift the keypoint's location by its center
			subMatrixKeyPoints[curKpt].pt.x += houghCircles[i][0];
			subMatrixKeyPoints[curKpt].pt.y += houghCircles[i][1];
			circleRadii.push_back(houghCircles[i][2]);
			cumulativeWorldKeyPoints.push_back(subMatrixKeyPoints[curKpt]);
		}
	}
	vconcat(subMatrixDescriptors, cumulativeWorldDescriptors);
	Mat showWorldKpts = world.clone();
	drawKeypoints(showWorldKpts, cumulativeWorldKeyPoints, showWorldKpts, Scalar(127, 127, 127));
	namedWindow("World Keypoints", WINDOW_NORMAL);
	resizeWindow("World Keypoints", showWorldKpts.rows, showWorldKpts.cols);
	cv::imshow("World Keypoints", showWorldKpts);
	cv::waitKey();
	vconcat(exemplarDescriptors, cumulativeExemplarDescriptors);

	// Get the best 50 matches for each exemplar descriptor  
	vector<vector<DMatch>> vecMatches;
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
	matcher->knnMatch(cumulativeExemplarDescriptors, cumulativeWorldDescriptors, vecMatches, 50, Mat());

	// Draw the circles where the keypoints came from
	for (size_t i = 0; i < houghCircles.size(); i++)
	{
		circle(world, Point(static_cast<int>(houghCircles[i][0]),
							static_cast<int>(houghCircles[i][1])), static_cast<int>(houghCircles[i][2]),
			Scalar(255, 0, 255), 1, LINE_AA);
	}

	// Draw the matches
	vector < DMatch> matches;
	for (int j = 0; j < vecMatches.size(); j++) {

		matches = vecMatches[j];
		drawMatches(waldoExemplar, cumulativeExemplarKeypoints, world, cumulativeWorldKeyPoints,
			        matches, img_matches, Scalar::all(-1), Scalar::all(-1), vector<char>(),
			        DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
		namedWindow("Matches", WINDOW_NORMAL);
		resizeWindow("Matches", img_matches.rows, img_matches.cols);
		imshow("Matches", img_matches);
		waitKey(0);

	}
	cv::imwrite("WaldoHoughMatches0_Green.jpg", img_matches);
}

// getSubMatrix
// Preconditions: image is a properly defined Mat
//				  circle is of the form [center_x center_y radius]
//				  width and height are positive integers
// Postconditions: Returns a submatrix from the image of the height and width of the radius
//				   where the center of the rectangle is at the center of the circle
// @param isCloned: return a clone of the submatrix or a reference
void WaldoAlgorithms::getSubMatrix(Mat & image, Mat & subMatrix, Vec3f& circle, bool isCloned)
{
	// Limit the boundaries of the upperleftmost corner
	// Assume that the center of the circle is at the center of the rectangle
	float radius = static_cast<float>(2.0)*circle[2];
	if (radius < 1)
		radius = 1;
	int upperLeftMostRow = static_cast<int>(circle[1] - radius);
	if (upperLeftMostRow < 0)
		upperLeftMostRow = 0;
	int upperLeftMostCol = static_cast<int>(circle[0] - radius);
	if (upperLeftMostCol < 0)
		upperLeftMostCol = 0;

	// If the center of the radius plus the radius goes beyond the bounds of the image
	// reduce the radius sample to the minimum distance to the max bounds
	if (upperLeftMostRow + radius > image.rows)
		radius = static_cast<float>(image.rows) - static_cast<float>(upperLeftMostRow);
	if (upperLeftMostCol + radius > image.cols)
		radius = static_cast<float>(image.cols) - static_cast<float>(upperLeftMostCol);
	if (isCloned)
		subMatrix = image(Rect(upperLeftMostCol, upperLeftMostRow,
			static_cast<int>(radius), static_cast<int>(radius))).clone();
	else
		subMatrix = image(Rect(upperLeftMostCol, upperLeftMostRow,
			static_cast<int>(radius), static_cast<int>(radius)));
}

// calculateHistograms
// Preconditions: image and mask are a properly defined Mat in BGR
// Postconditions: returns a 32x32 bin normalized colored histogram in HSV of the image
void WaldoAlgorithms::calculateHistograms(Mat& image, Mat& hist, Mat& mask)
{
	// Create a normalized colored histogram for the image

	// 2D array with 3 channels
	int channels[] = { 0, 1 ,2 };
	int histSize[] = { 36, 10, 10 };


	// Ranges in HSV (flattened) 
	float hRanges[] = { 0, 180 };
	float sRanges[] = { 0, 256 };
	float vRanges[] = { 0, 256 };
	const float* ranges[] = { hRanges, sRanges, vRanges };
	Mat imageHSV;

	// Convert the submatrix colors to HSV Image for Histogram purposes
	cvtColor(image, imageHSV, COLOR_BGR2HSV);
	calcHist(&imageHSV, 1, channels, mask, hist, 3, histSize, ranges, true);
	normalize(hist, hist, 1, 0, NORM_L1);
}


//Description: Draws rectangles on input frame
//Preconditions: valid input
//Postconditions: frame will have rectangles drawn on it
void WaldoAlgorithms::drawRectangles(Mat& frame, vector<Rect>& rectangles, Scalar color, int borderWidth)
{

	for (size_t i = 0; i < rectangles.size(); i++)
	{
		rectangle(frame, rectangles[i], color, borderWidth);
	}

}

//Description: Finds possible locations of waldo by using a waldo trained cascade model
//Preconditions: valid input. filename is the name of the cascade trained model file
//Postconditions: rectangles contains possible locations of waldo
void WaldoAlgorithms::detectWaldo(Mat& frame, vector<Rect>& rectangles
	, const std::string& filename)
{
	CascadeClassifier face_cascade;

	if (!face_cascade.load(filename))
	{
		cout << "--(!)Error loading face cascade\n";
		return;
	};

	Mat frame_gray;
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);

	face_cascade.detectMultiScale(frame_gray, rectangles, 1.1, 2, 0, { 5, 5 }, { 90, 90 });

}

//Description: eliminates some of the possible locations of waldo by using sift
//Preconditions: valid input
//Postconditions: some elements will be eliminated from rectangles
void WaldoAlgorithms::siftElimination(Mat& input, Mat& exemplar, vector<Rect>& rectangles,
	const int numSamples = 1000, const float ratioThreshold = 0.9f, const unsigned int fractionDenominator = 2)
{


	vector<weightedRectangle> afterSift;

	Ptr<SIFT> detector = SIFT::create(numSamples);

	// Acquire the cumulative keypoints and descriptors for each circle detected in the exemplar
	Mat exemplarDescriptors;
	vector<KeyPoint> exemplarKeypoints;
	detector->detectAndCompute(exemplar, noArray(), exemplarKeypoints, exemplarDescriptors);

	// initialize matcher
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);

	// for every rectangle
	for (unsigned int i = 0; i < rectangles.size(); i++)
	{
		// get submatrix
		Mat subMatrix = input(rectangles[i]);

		// initialize descriptors and keypoints
		Mat subMatrixDescriptors;
		vector<KeyPoint> subMatrixKeyPoints;

		// detect features
		detector->detectAndCompute(subMatrix, noArray(), subMatrixKeyPoints, subMatrixDescriptors);


		vector<vector<DMatch>> knnMatches;

		matcher->knnMatch(exemplarDescriptors, subMatrixDescriptors, knnMatches, 2);

		//-- Filter matches using the Lowe's ratio test

		vector<DMatch> goodMatches;
		for (unsigned int i = 0; i < knnMatches.size(); i++)
		{

			if (knnMatches[i].size() == 1) { // if there is only one feature, just add it.
				goodMatches.push_back(knnMatches[i][0]);
			}

			if (knnMatches[i].size() == 2 && knnMatches[i][0].distance < ratioThreshold * knnMatches[i][1].distance)
			{
				goodMatches.push_back(knnMatches[i][0]);
			}

		}

		if (goodMatches.size() > 0) {

			double meanDistance = 0;
			for (unsigned int i = 0; i < goodMatches.size(); i++) {
				meanDistance += goodMatches[i].distance;
			}

			meanDistance /= goodMatches.size();

			weightedRectangle temp = { rectangles[i], meanDistance };

			afterSift.push_back(temp);

		}

	}

	sort(afterSift.begin(), afterSift.end(), compareRectangles);

	rectangles.clear();

	for (unsigned int i = 0; i < afterSift.size() / fractionDenominator; i++) { // just leave 1 / fractionDenominator of the best rectangles
		rectangles.push_back(afterSift[i].rect);
	}

}

//Description: Eliminates a fraction of possible waldo locations by using a color histogram
//Preconditions: valid input
//Postconditions: some elements will be eliminated from rectangles
void WaldoAlgorithms::colorHistElimination(Mat& input, Mat& exemplar, vector<Rect>& rectangles,
	const float ratioThreshold = 0.85f, const unsigned int fractionDenominator = 2)
{

	Mat waldoColors;

	Mat mask;
	inRange(exemplar, Scalar(0, 235, 0), Scalar(15, 255, 15), mask);//look for all green
	bitwise_not(mask, mask);
	WaldoAlgorithms::calculateHistograms(exemplar, waldoColors, mask);

	vector<weightedRectangle> afterHistogram;
	for (size_t i = 0; i < rectangles.size(); i++)
	{
		// Get the submatrix
		Mat cur = input(rectangles[i]);

		Mat curHist;
		Mat curMask;//consider adding agreen filter for world as well.
		WaldoAlgorithms::calculateHistograms(cur, curHist, curMask);

		// Calculate the Bhattacharrya Distribution to sense strength of color matching
		double response = compareHist(waldoColors, curHist, HISTCMP_BHATTACHARYYA);

		weightedRectangle temp = { rectangles[i], response };

		afterHistogram.push_back(temp);

	}

	sort(afterHistogram.begin(), afterHistogram.end(), compareRectangles);

	rectangles.clear();

	for (unsigned int i = 0; i < afterHistogram.size() / fractionDenominator; i++) { // just leave 1 / fractionDenominator of the best rectangles
		rectangles.push_back(afterHistogram[i].rect);
	}

};

//Description: uses cascade classifier to find possible waldo positions then narrows it down using sift
//Preconditions: valid input. cascade is the filename of the classifier trained model
//Postconditions: rectangles contains possible locations of waldo
void WaldoAlgorithms::findWaldoHaarCascade(Mat& input, Mat& waldoExemplar, vector<Rect>& rectangles, const std::string& cascade)
{

	detectWaldo(input, rectangles, cascade);

	WaldoAlgorithms::siftElimination(input, waldoExemplar, rectangles);


}

/* 1 END: CURRENT IMPLEMENTATION*/

/* 2 BEGIN: DEPRECATED CODE*/

/* These functions are kept for legacy main functions and in case we would need them again
 * They were removed because they were no longer needed, or inefficient, and kept because 
 * they may be useful for future improvements of the algorithm.
*/

// getPyrDownImage
// Preconditions: image and output are properly defined Mats
//				  pyrDownOctave is > 0
// Postconditions: returns the downsampled image pyramid of the desired
//					octave level
void WaldoAlgorithms::getPyrDownImage(Mat& image, Mat& output, const int& pyrDownOctave)
{
	int pyramidLevel = pyrDownOctave;
	// return the output image being equal to the input if invalid octave 
	if (pyrDownOctave <= 0)
		output = image.clone();
	else
	{
		if (pyrDownOctave > 3)
			pyramidLevel = 3;
		// downsample the image until the desired octave level is reached
		Mat intermediateMatrix;
		pyrDown(image, output, Size());
		for (int curOctave = 1; curOctave < pyramidLevel; curOctave++)
		{
			if (curOctave % 2 == 1)
			{
				pyrDown(output, intermediateMatrix, Size());
			}
			else
				pyrDown(intermediateMatrix, output, Size());
		}
	}
}

// removeRowInMatrix
void WaldoAlgorithms::removeRowInMatrix(Mat & image, int row)
{
	Size originalSize = image.size();
	Mat outputMatrix(originalSize.height - 1, originalSize.width, image.type());
	// Copies Everything above the row
	if (row > 0)
	{
		Rect upperPart(0, 0, originalSize.width, row);
		image(upperPart).copyTo(outputMatrix(upperPart));
	}

	// Copy everything below that row
	if (row < originalSize.height - 1)
	{
		Rect originalLowerHalf(0, row + 1, originalSize.width, originalSize.height - row - 1);
		Rect outputLowerHalf(0, row, originalSize.width, originalSize.height - row - 1);
		image(originalLowerHalf).copyTo(outputMatrix(outputLowerHalf));
	}

	// Copy the data from the output matrix to the original image
	outputMatrix.copyTo(image);
}

/* 2 END: DEPRECATED CODE*/