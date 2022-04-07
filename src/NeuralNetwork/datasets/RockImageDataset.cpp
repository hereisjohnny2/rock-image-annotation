#include <vector>
#include "RockImageDataset.h"

torch::data::Example<> RockImageDataset::get(size_t index) {
    torch::Tensor sampleRGBInfo = data.rgbInfo[index];
    torch::Tensor sampleLabel = data.labels[index];
    return {sampleRGBInfo.clone(), sampleLabel.clone()};
}

torch::optional<size_t> RockImageDataset::size() const  {
    return data.labels.sizes()[0];
};

RockData RockImageDataset::readDataFromString(const std::string &stringData) {
    RockData output;

    std::vector<double> dataVector = {};
    std::stringstream stream(stringData);
    std::string tmp;

    while (stream >> tmp)
        dataVector.push_back(std::stod(tmp));

    output.rgbInfo = torch::zeros({(int)dataVector.size() / 4, 3});
    output.labels = torch::zeros({(int)dataVector.size() / 4});

    for (int i = 0; i < dataVector.size() / 4; i++)
    {
        output.rgbInfo[i][0] = dataVector[4*i    ];
        output.rgbInfo[i][1] = dataVector[4*i + 1];
        output.rgbInfo[i][2] = dataVector[4*i + 2];
        output.labels[i]     = dataVector[4*i + 3];
    }

    return output;
}
