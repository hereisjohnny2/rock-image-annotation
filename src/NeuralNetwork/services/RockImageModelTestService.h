//
// Created by joao on 07/04/2022.
//

#ifndef ROCK_IMAGE_CPP_ROCKIMAGEMODELTESTSERVICE_H
#define ROCK_IMAGE_CPP_ROCKIMAGEMODELTESTSERVICE_H

#include <memory>
#include <iomanip>
#include <utility>

#include <torch/optim.h>
#include "../models/RockImageModel.h"


class RockImageModelTestService {
private:
    std::shared_ptr<RockImageModel> model;

public:
    explicit RockImageModelTestService(std::shared_ptr<RockImageModel> _model)
        : model(std::move(_model)) {};

    template<typename DataLoader>
    void execute(int epoch, int datasetSize, DataLoader &dataLoader)
    {
        torch::NoGradGuard();
        model->eval();

        double testLoss = 0;
        int correct = 0;

        for(auto &batch : dataLoader)
        {
            auto data = batch.data;
            auto target = batch.target.squeeze();

            data = data.to(torch::kF32);
            target = target.to(torch::kInt64);

            auto output = model->forward(data);
            testLoss += torch::nll_loss(output, target, {}, torch::Reduction::Sum).template item<double>();

            auto pred = output.argmax(1);
            correct += pred.eq(target).sum().template item<int64_t>();
        }

        testLoss /= datasetSize;
        std::printf(
                "\nTest set: Average loss: %.4f | Accuracy: %.3f\n",
                testLoss,
                static_cast<double>(correct) / datasetSize
        );
    }

};


#endif //ROCK_IMAGE_CPP_ROCKIMAGEMODELTESTSERVICE_H
