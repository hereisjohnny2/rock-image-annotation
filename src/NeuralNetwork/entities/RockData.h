//
// Created by joao on 07/04/2022.
//

#ifndef ROCK_IMAGE_CPP_ROCKDATA_H
#define ROCK_IMAGE_CPP_ROCKDATA_H

#include <torch/data.h>

struct RockData {
    torch::Tensor rgbInfo, labels;
};

#endif //ROCK_IMAGE_CPP_ROCKDATA_H
