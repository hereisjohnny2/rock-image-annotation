#ifndef ROCK_IMAGE_CPP_ROCKIMAGEMODEL_H
#define ROCK_IMAGE_CPP_ROCKIMAGEMODEL_H

#include <torch/nn/module.h>
#include <torch/nn/modules/linear.h>

struct RockImageModel : torch::nn::Module {
    torch::nn::Linear fc1{nullptr}, fc2{nullptr}, fc3{nullptr};

    RockImageModel();
    torch::Tensor forward(torch::Tensor x);
};


#endif //ROCK_IMAGE_CPP_ROCKIMAGEMODEL_H
