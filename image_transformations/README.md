# Image Transformations

This program transforms an input image using six parameters which map to a 2X3 transformation matrix. It is similar to OpenCV's (`wrapAffine`)[https://docs.opencv.org/4.5.3/d4/d61/tutorial_warp_affine.html] function.

Give the following parameters `1.5 1.5 20.0 40.0 20.0 0.5`, which correspond to the following variables:

```c++
double colS; // scale of the cols
double rowS; // scale of the rows
double colT; // translation of the cols
double rowT; // translation of the rows
double theta;// rotation angle in degrees (clockwise)
double shear;// shear factor
```

The program will transform this input image

![](https://computerp-vision.ybeltagy.com/pics/transformations_input.gif)

To this output image

![](https://computerp-vision.ybeltagy.com/pics/transformations_output.gif)