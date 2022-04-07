//
// Created by joao on 07/04/2022.
//

#ifndef ROCK_IMAGE_CPP_ROCKIMAGEDATASET_H
#define ROCK_IMAGE_CPP_ROCKIMAGEDATASET_H


#include <torch/data.h>
#include <string>
#include "../entities/RockData.h"

class RockImageDataset : public torch::data::Dataset<RockImageDataset>
{
private:
    RockData data;

public:
    explicit RockImageDataset(const std::string &stringData): data(readDataFromString(stringData)) {}

    torch::data::Example<> get(size_t index) override;
    [[nodiscard]] torch::optional<size_t> size() const override;

private:
    static RockData readDataFromString(const std::string &stringData);
};



#endif //ROCK_IMAGE_CPP_ROCKIMAGEDATASET_H
