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

RUN curl https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-shared-with-deps-1.11.0%2Bcpu.zip --output libtorch.zip

RUN unzip libtorch.zip

RUN mv libtorch /usr/local

COPY . /usr/src/rockimage

WORKDIR /usr/src/rockimage

RUN mkdir build

RUN cmake -B build -S . -DCMAKE_PREFIX_PATH=/6.2.0/gcc_64

CMD ["./tools/buildrun"]