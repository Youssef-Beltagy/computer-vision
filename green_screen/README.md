# Green Screen

This program is a green screen filter. It replaces the background of an input image using another image. The program computes a color histogram and uses it to identify the most common color range in the image. It then replaces all pixels in that color range with pixels from another image.

The program also experiments with OpenCV's edge detection (Canny), convert color, and smoothing functions to generate some interesting images: an edges image and a pyramid of photonegative images.

For the following inputs,

![](https://computer-vision.ybeltagy.com/pics/greenscreen_background_input.jpg)

![](https://computer-vision.ybeltagy.com/pics/greenscreen_foreground_input.jpg)

The following output is produced:

![](https://computer-vision.ybeltagy.com/pics/greenscreen_output.jpg)

![](https://computer-vision.ybeltagy.com/pics/greenscreen_canny_output.jpg)

![](https://computer-vision.ybeltagy.com/pics/greenscreen_photonegative_output.jpg)



