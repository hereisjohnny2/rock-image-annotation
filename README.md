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
- [x] Create Docker image

## Technologies and Dependencies 💻

- [C++ 17](https://en.cppreference.com/w/cpp/17)
- [QT 6](https://www.qt.io/product/qt6)
- [LibTorch](https://pytorch.org/)
- [Docker](https://docs.docker.com)
- [Docker Compose](https://docs.docker.com/compose/gettingstarted/)

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

### Converting Images

So far this project only support few image formats, such as JPG, BMP and PNG. Use the `convimg` script to convert the images from other formats to PNG and use with the project.

```shell
# tools/convimg <director-with-images> <original-format>
$ tools/convimg ~/Images tif
```

### Running with docker 

It's possible to run this project on a docker container. First make sure that docker and docker compose are installed locally. If not follow the [instructions](https://docs.docker.com/engine/install/) in the official page.

```shell
$ docker -v
$ docker-compose -v
```
 
Then grant access to everyone with `xhost`. It's going to be necessary to run the GUI.
```shell
$ xhost +
```

Finally, build the image and run the container with `docker-compose`.

```shell
$ docker-compose build
$ docker-compose up
```

It's also possible to build and run the container without `docker-compose`.

```shell
$ docker build -t rockimage . 
$ docker run --name rockimagecpp --rm \
  -e "DISPLAY=$DISPLAY" \
  -v "$HOME/.Xauthority:/root/.Xauthority:ro" \
  -v "$pwd:/usr/src/rockimage" \
  --network host \
   rockimage\

```

## Testing 🧪


