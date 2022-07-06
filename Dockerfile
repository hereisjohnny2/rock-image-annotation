FROM ubuntu:latest

ENV TZ=America/Sao_Paulo

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get -y update && apt-get install -y

RUN apt-get install -y \
    build-essential \
    clang \
    cmake \
    qt5-default \
    pip

RUN pip install -U pip && pip install aqtinstall

RUN aqt install-qt linux desktop 6.2.0

RUN apt-get install -y curl unzip

ENV QT_DEBUG_PLUGINS=1

COPY . /usr/src/rockimage

WORKDIR /usr/src/rockimage

RUN chmod +x tools/buildrun

RUN mkdir build

RUN cmake -B build -S . -DCMAKE_PREFIX_PATH=/6.2.0/gcc_64

RUN cd build && make

CMD ["./build/rock_image_cpp"]