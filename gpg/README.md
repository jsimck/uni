# vsb-gpg
Project used for exercises in GPG course on university.

## Requirements
This project is using shared libraries, so you need to have these installed on your computer.

### OpenCV 3.x
OpenCV 3.x is used to display rendered results. On macOS:
```
brew install opencv3 --with-contrib
```

### Embree 2.11.0
[https://embree.github.io/](https://embree.github.io/).

### GCC 6.x
```
brew install gcc --without-multilib --without-fortran
```
 
## Compiling
When compiling project, make sure to set working directory to project root, otherwise some textures and objects may not load. Also in *CMakeLists.txt* you may need to edit paths to `CMAKE_C_COMPILER` and `CMAKE_CXX_COMPILER` options.