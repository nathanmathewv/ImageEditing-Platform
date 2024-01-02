#include <vector>
#include "GaussianBlur.h"
#include <cmath>
using namespace std;

void blurpicture(vector<vector<Pixel>>& image, float amount) {
    int kernelSize = amount;
    if(amount<=3) kernelSize =3;    //kernelSize should be at least 3
    else if(!amount&1) kernelSize=amount-1; //kernelSize should be odd

    int width = image[0].size();
    int height = image.size();
    vector<float> kernel1D;
    for (int i = -kernelSize / 2; i <= kernelSize / 2; ++i) {   //create a 1D kernel using the Gaussian function
        float value = exp(-(i * i) / (2 * pow(kernelSize / 2.0, 2))) / (sqrt(2 * M_PI) * (kernelSize / 2.0));   //Gaussian function
        kernel1D.push_back(value);
    }

    // Normalize the kernel
    float sum = 0;
    for (float value : kernel1D) {  //find the sum of all the values in the kernel
        sum += value;
    }
    for (float& value : kernel1D) {
        value /= sum;
    }

    // Apply the 1D kernel vertically
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Pixel blurredPixel;
            blurredPixel.r = blurredPixel.b = blurredPixel.g = 0;   //initialize the blurred pixel to be black

            for (int i = -kernelSize / 2; i <= kernelSize / 2; ++i) {   //apply the kernel to the pixel
                int offsetX =  max(0,  min(x + i, width - 1));
                int offsetY =  max(0,  min(y + i, height - 1));

                blurredPixel.r += kernel1D[i + kernelSize / 2] * image[offsetY][x].r;   //multiply the kernel value by the pixel value and add it to the blurred pixel
                blurredPixel.g += kernel1D[i + kernelSize / 2] * image[offsetY][x].g;
                blurredPixel.b += kernel1D[i + kernelSize / 2] * image[offsetY][x].b;
            }

            image[y][x] = blurredPixel; //set the pixel to the blurred pixel
        }
    }
    // Apply the 1D kernel horizontally
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Pixel blurredPixel;
            blurredPixel.r = blurredPixel.b = blurredPixel.g = 0;

            for (int i = -kernelSize / 2; i <= kernelSize / 2; ++i) {
                int offsetY =  max(0,  min(y + i, height - 1)); //apply the kernel to the pixel
                int offsetX =  max(0,  min(x + i, width - 1));

                blurredPixel.r += kernel1D[i + kernelSize / 2] * image[y][offsetX].r;   //multiply the kernel value by the pixel value and add it to the blurred pixel
                blurredPixel.g += kernel1D[i + kernelSize / 2] * image[y][offsetX].g;
                blurredPixel.b += kernel1D[i + kernelSize / 2] * image[y][offsetX].b;
            }

            image[y][x] = blurredPixel;
        }
    }
}