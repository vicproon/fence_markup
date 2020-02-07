# fence_markup
Test task materials for Cogintive Robotics candidates

## Instructions

### Description
Project contains two source files: 
* projections.hpp which contains methods and classes to work with pinhole projection
* main.cpp which contains an example of usage.

### Resources
`data` folder contains projection matrices

`imgs` folder contains images

### How to build
To run the example do
```
mkdir build && cd build
cmake ..
make
./fence_markup
```
To stop viewing press Esc

### Requirements
[OpenCV](https://github.com/opencv/opencv) (Tested 3.4.5)

[CMake](https://cmake.org/) (Tested 3.15)

gcc (Tested 7.3.0)
