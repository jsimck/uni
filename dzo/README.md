# vsb-dzo
Project used for exercises in DZO course on university. It's focused on practicing using OpenCV library and implementation of digital image processing methods.

## Projects
- **1-basics** - Introduction to OpenCV library.
- **2-gamma-contrast-correction** - Simple gamma and contrast correction method.
- **3-convolution** - Implementation of Convolution + some filters.
- **4-anisotropic-filter** - Implementation of Anisotropic filtering.
- **5-discrete-fourier-transform** - Implementation of Discrete Fourier Transform function and it's phase and power.
- **6-inverse-discrete-fourier-transform** - Implementation of Inverse Discrete Fourier Transform function, used on top of previous implementation of DFT.
- **7-dft-filters** - Usage of DFT and IDFT to implement high/low pass filters and straight line reduction filters.
- **8-geometric-distortion** - Implementation of geometric distortion removal.
- **9-histogram** - Implementation of histogram equalization to visualize and improve contrast of given image.
- **10-perspective-transform** - Generation of perspective transform matrix, which projects one image into specific place in another.
- **11-retinex** - projected image reconstruction.

## Requirements
This project is using shared libraries, so you need to have these installed on your computer.

### OpenCV 3.x
OpenCV 3.x is main library used in this projects. On macOS you can easily install this using homebrew:
```
brew install opencv3 --with-contrib
```
In time of working on this project OpenCV doesn't yet support macOS 10.12 Sierra. So you need to install opencv3 with `--HEAD` argument to get the latest 'nightly' version:
```
brew install --HEAD opencv3 --with-contrib
```








