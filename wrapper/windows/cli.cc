#include <iostream>
#include "gpupixel_wrapper.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


int main(int argc, char* argv[]) {
    if (argc != 9) {
        std::cerr << "Usage: " << argv[0] << " <inputPath> <outputPath> <beautyValue> <whithValue> <thinFaceValue> <bigeyeValue> <lipstickValue> <blusherValue>" << std::endl;
        return 1;
    }

    std::string inputPath = argv[1];
    std::string outputPath = argv[2];
    float beautyValue = std::stof(argv[3]);
    float whithValue = std::stof(argv[4]);
    float thinFaceValue = std::stof(argv[5]);
    float bigeyeValue = std::stof(argv[6]);
    float lipstickValue = std::stof(argv[7]);
    float blusherValue = std::stof(argv[8]);

    // create GPUPixelWrapper instance
    GPUPixelWrapper wrapper;

    // initialize GPUPixelWrapper
    if (!wrapper.initialize()) {
        std::cerr << "Failed to initialize GPUPixelWrapper" << std::endl;
        return 1;
    }

    // set callbacks for input and output
    wrapper.setCallbacks();

    // set parameters for filters
    wrapper.setParameters(beautyValue, whithValue, thinFaceValue, bigeyeValue, lipstickValue, blusherValue);

    // read input image
    int width, height, channel;
    uint8_t* inputData = stbi_load(inputPath.c_str(), &width, &height, &channel, 0);
    if (inputData == nullptr) {
        std::cerr << "Failed to read image" << std::endl;
        return -1;
    }

    // run image processing pipeline
    uint8_t* outputBuffer = wrapper.forward(inputData, width, height, channel);

    // save ouput image
    stbi_write_png(outputPath.c_str(), width, height, channel, outputBuffer, width * channel);

    // release input
    stbi_image_free(inputData);

    // release output
    wrapper.release();

    return 0;
}
