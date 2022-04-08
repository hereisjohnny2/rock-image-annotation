# Rock Image Annotation Tool

Rock Image C++ is simple GUI application build in C++ and Qt to label layers from rocks for neural networks training.

![screenshot1](./.github/screenshot1.png)

## About 📕

Rock Image C++ is simple GUI application build in C++ and Qt to label layers from rocks for neural networks training.

### Todo

- [x] Check for ImageMagik Lib to convert any image type to PNG or other file format that Qt is able to work with.
  - [x] Create script to convert images.
- [ ] Customize pen color and size for each layer.
- [ ] Fix Zoom In and Out.
- [ ] Add AI services to provide image segmentation. </br>
  - [x] Provide classes and functions to create and run AI models.
  - [x] Provide classes and functions to create a dataset.

## Technologies and Dependencies 💻

- [C++ 17](https://en.cppreference.com/w/cpp/17)
- [QT 6](https://www.qt.io/product/qt6)
- [LibTorch](https://pytorch.org/)

## Getting Started 🚀

First create a folder to receive the build files.

```shell
$ mkdir build
```

Then, with the `cmake` run the follow command in your terminal.

```shell
$ cmake -B build -S .
```

Now, inside the build folder run the make command to compile the project.

```shell
$ cd build && make
```

After the compilation is completed run the executable.

```shell
$ ./rock_image_cpp
```

### Running with docker

```shell
$ xhost +
$ docker-compose build
$ docker-compose up
```

## Testing 🧪


