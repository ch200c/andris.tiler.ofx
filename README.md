# andris.tiler.ofx
OpenFX plugin that produces a tile effect with an adjustable offset and scaling

Input | Output
----- | -----
![Input](http://an-dris.com/images/tiler/input.jpg) | ![Output](http://an-dris.com/images/tiler/output.jpg)

Photo by Jakob Owens on Unsplash

## Dependencies
* [OpenFX](https://github.com/ofxa/openfx)
* [OpenCV](https://github.com/opencv/opencv)
* [andris.ofx.util](https://github.com/ch200c/andris.ofx.util)

## How to build
* Clone the repository
* Run `cmake` in this project's root folder with
    * `-DOPENFX_DIR:PATH` pointing to the root folder of OpenFX source
    * `-DOPENFX_SUPPORT_LIB:FILEPATH` pointing to the OpenFX support lib file
    * `-DOpenCV_DIR:PATH` pointing to the directory where you have built OpenCV
    * `-Dandris.ofx.util_DIR:PATH` pointing to the directory where you have built andris.ofx.util

## Acknowledgements
CMake files adapted from [how-to-export-cpp-library](https://github.com/robotology/how-to-export-cpp-library)