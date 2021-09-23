/*
Author: Youssef Beltagy

Date Last modified: 10/21/2019

Description: This is program two of CSS 487 (computer vision)
This file tests the tranformation functions to smooth and detect edges in an image.

*/

#include "Image.h"
#include "Transformations.h"


void test() {


	Image image("test2.gif");// initialize image

	for (int row = 0; row < image.getRows(); row++) {// set floats
		for (int col = 0; col < image.getCols(); col++) {

			image.setFloat(row, col, image.getPixel(row, col).grey);

		}
	}


	Transformations::smooth(image, 0); // smooth

	Image edges = Transformations::detectEdges(image);

	for (int row = 0; row < image.getRows(); row++) {// set floats
		for (int col = 0; col < image.getCols(); col++) {

			image.setGrey(row, col, (byte) image.getPixel(row, col).floatVal);

		}
	}

	image.writeGreyImage("my smooth.gif");
	edges.writeGreyImage("my Edges.gif");

	{
		int counterx = 0, countery = 0, countermag = 0, countersmooth = 0, counteredges = 0;

		Image myedges("my Edges.gif");
		Image mysmooth("my smooth.gif");
		Image myx("my dx.gif");
		Image myy("my dy.gif");
		Image mymag("my mag.gif");

		Image compareedges("edges0.gif");
		Image comparesmooth("smooth0.gif");
		Image comparex("gx.gif");// get an image to compare to
		Image comparey("gy.gif");
		Image comparemag("gmag.gif");

		for (int row = 0; row < mymag.getRows(); row++) {
			for (int col = 0; col < mymag.getCols(); col++) {

				byte value = 0;

				if (myedges.getPixel(row, col).grey != compareedges.getPixel(row, col).grey) {
					counteredges++;

					value = 255;

					cout << endl;
					cout << "Edge diff found for pixels at (" << col << ", " << row << ")" << endl;
					cout << "Sample pixle Value: " << (int)compareedges.getPixel(row, col).grey << endl;
					cout << "output pixel value: " << (int)myedges.getPixel(row, col).grey << endl;
				}

				myedges.setGrey(row, col, value);


				if (mysmooth.getPixel(row, col).grey != comparesmooth.getPixel(row, col).grey) {
					countersmooth++;
				}

				if (myy.getPixel(row, col).grey != comparey.getPixel(row, col).grey) {
					countery++;
				}

				if (myx.getPixel(row, col).grey != comparex.getPixel(row, col).grey) {
					counterx++;
				}

				if (mymag.getPixel(row, col).grey != comparemag.getPixel(row, col).grey) {
					countermag++;
				}

			}
		}

		myedges.writeGreyImage("edge diff.gif");

		std::cout << endl << endl << endl << endl;
		std::cout << "Num of dx diffs: " << counterx << endl;
		std::cout << "Num of dy diffs: " << countery << endl;
		std::cout << "Num of mag diffs: " << countermag << endl;
		std::cout << "Num of smooth diffs: " << countersmooth << endl;
		std::cout << "Num of edge diffs: " << counteredges << endl;

	}
}


// main method
// Description:
//	tests the smoothing and edge detection
// Preconditions:  test2.gif exists and is a correctly formatted GIF image
// Postconditions: Creates images smooth.gif, that is the smoothing of test2.gif, 
//	and edges.gif, that is the edges found in test2.gif after smoothing.
int main(int argc, char* argv[]) {

	test();

	//if (argc != 3) {
	//	cout << "This program only works with 1 argument." <<
	//		 " Please re-run with one int argument other than the prognam name." << endl;
	//	return -1;
	//}

	//int iteration; // num of smoothing

	//// read the values from the function arguments.
	//sscanf_s(argv[1], "%ld", &iteration);

	//Image image("test2.gif");// initialize image

	//// images read from disk are gruanteed to have the grey byte set.

	//for (int row = 0; row < image.getRows(); row++) {// set floats to greys
	//	for (int col = 0; col < image.getCols(); col++) {

	//		image.setFloat(row, col, (float) image.getPixel(row, col).grey);

	//	}
	//}


	//Transformations::smooth(image, iteration); // smooth

	//Image edges = Transformations::detectEdges(image); // find edges


	//for (int row = 0; row < image.getRows(); row++) {// set image to grey again
	//	for (int col = 0; col < image.getCols(); col++) {

	//		image.setGrey(row, col, (byte) image.getPixel(row, col).floatVal);

	//	}
	//}

	//image.writeGreyImage("smooth.gif");
	//edges.writeGreyImage("edges.gif");

	//cout << "Program end" << endl;

	//return 0;

}