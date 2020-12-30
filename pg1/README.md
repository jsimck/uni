# vsb-pg1
Project used for exercises in PG1 course on university. It's focused on learning ray tracing techniques using OpenCV and Embree C++ libraries.

## Results
| Lambert Shader                                                                                                            | Normal Shader                                                                                                          |
|--------------------------------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------|
| ![lambert](https://raw.githubusercontent.com/R1crd/vsb-pg1/master/data/results/result_lambert.png "Lambert Shader") | ![normal](https://raw.githubusercontent.com/R1crd/vsb-pg1/master/data/results/result_normal.png "Normal Shader") |

| Phong                                                                                                         | Path Tracer                                                                                                  |
|---------------------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------|
| ![phong](https://raw.githubusercontent.com/R1crd/vsb-pg1/master/data/results/result_phong.png "Phong Shader") | ![path](https://raw.githubusercontent.com/R1crd/vsb-pg1/master/data/results/result_path_tracer_final.png "Phong Shader") |


## Requirements
This project is using shared libraries, so you need to have these installed on your computer.

### OpenCV 3.x
OpenCV 3.x is used to display rendered results. On macOS you can easily install this using homebrew:
```
brew install opencv3 --with-contrib
```
In time of working on this project OpenCV doesn't yet support macOS 10.12 Sierra. So you need to install opencv3 with `--HEAD` argument to get the latest 'nightly' version:
```
brew install --HEAD opencv3 --with-contrib
```

### Embree 2.11.0
Just download and install this library from official website [https://embree.github.io/](https://embree.github.io/).

### GCC 6.x
Either this or any other C++11 capable compiler. _GCC 4.9 causes issues so you really need to use version 6.x_. There are also some system-specific functions and specific libraries used in this project, so you may need to fix these issues on your specific build.
```
brew install gcc --without-multilib --without-fortran
```
 
## Compiling
When compiling project, make sure to set working directory to project root, otherwise some textures and objects may not load. Also in *CMakeLists.txt* you may need to edit paths to `CMAKE_C_COMPILER` and `CMAKE_CXX_COMPILER` options.