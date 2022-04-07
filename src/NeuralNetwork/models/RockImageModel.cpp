#include "RockImageModel.h"

RockImageModel::RockImageModel()
{
    fc1 = register_module("fc1", torch::nn::Linear(3,16));
    fc2 = register_module("fc2", torch::nn::Linear(16,16));
    fc3 = register_module("fc3", torch::nn::Linear(16,2));
}

torch::Tensor RockImageModel::forward(torch::Tensor x) {
    x = torch::relu(fc1->forward(x.reshape({x.size(0), 3})));
    x = torch::dropout(x, /*p=*/0.5, /*train=*/is_training());
    x = torch::relu(fc2->forward(x));
    x = torch::log_softmax(fc3->forward(x), /*dim=*/1);
    return x;
}
