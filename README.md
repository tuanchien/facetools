# Facetools

A C++ frontend library for face detection and recognition.

### Facegrep

A command line tool to search for similar faces (grep for faces), using the facetools framework.

## Prerequisites
1. CMake 2.8 or later.
2. dlib 19.4 or later.
3. Google Test (if you want to compile the tests).

## Build

```
mkdir build && cd build
cmake ..
make
```

To fetch the models during the build process use ```cmake -DGET_MODELS=1 ..``` instead of ```cmake ..```

#### Example facegrep usage
From within the ```facetools/test_data/facegrep/searchdir``` directory,

```
facegrep bruce0.jpg .
```
will produce
```
./bruce/bruce1.jpg
./bruce/bruce2.jpg
./bruce/bruce3.jpg
./bruce0.jpg
```
as results.
