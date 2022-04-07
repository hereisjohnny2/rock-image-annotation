//
// Created by joao on 07/04/2022.
//

#ifndef ROCK_IMAGE_CPP_ROCKIMAGEMODELTRAINSERVICE_H
#define ROCK_IMAGE_CPP_ROCKIMAGEMODELTRAINSERVICE_H

#include <memory>
#include <iomanip>
#include <utility>

#include <torch/optim.h>

#include "../models/RockImageModel.h"

class RockImageModelTrainService {
private:
    std::shared_ptr<RockImageModel> model;
    torch::optim::Optimizer &optimizer;

public:
    RockImageModelTrainService(std::shared_ptr<RockImageModel> _model, torch::optim::Optimizer &_optimizer)
        : model(std::move(_model)), optimizer(_optimizer) {};

    template<typename DataLoader>
    void execute(int epoch, int datasetSize, DataLoader &dataLoader)
    {
        model->train();
        size_t batchIndex = 0;

        for(auto &batch : dataLoader)
        {
            auto data = batch.data;
            auto target = batch.target.squeeze();

            data = data.to(torch::kF32);
            target = target.to(torch::kInt64);

            optimizer.zero_grad();

            auto output = model->forward(data);
            auto loss = torch::nll_loss(output, target);

            loss.backward();
            optimizer.step();

            if (batchIndex++ % 10 == 0) {
                std::printf(
                        "\rEpoch: %d [%5ld/%5d] Loss: %f",
                        epoch,
                        batchIndex * batch.data.size(0),
                        datasetSize,
                        loss.template item<float>()
                );
            }
        }
    }


};


#endif //ROCK_IMAGE_CPP_ROCKIMAGEMODELTRAINSERVICE_H
