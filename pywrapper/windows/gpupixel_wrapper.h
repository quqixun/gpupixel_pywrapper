#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "gpupixel.h"
using namespace gpupixel;


class GPUPixelWrapper {

    private:
        GLFWwindow* window;
        std::shared_ptr<SourceRawDataInput> source_raw_input;
        std::shared_ptr<TargetRawDataOutput> target_raw_output;
        std::shared_ptr<BeautyFaceFilter> beauty_face_filter;
        std::shared_ptr<FaceReshapeFilter> face_reshape_filter;
        std::shared_ptr<LipstickFilter> lipstick_filter;
        std::shared_ptr<BlusherFilter> blusher_filter;
        uint8_t* outputBuffer;
        int width, height, channel;

    public:
        GPUPixelWrapper() : window(nullptr), outputBuffer(nullptr) {}

        ~GPUPixelWrapper() {
            release();
        }

        bool initialize() {
            // initialize OpenGL functions
            window = GPUPixelContext::getInstance()->GetGLContext();
            if (window == nullptr) {
                std::cerr << "Failed to create GLFW window" << std::endl;
                return false;
            }
            glfwMakeContextCurrent(window);
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                std::cerr << "Failed to initialize GLAD" << std::endl;
                return false;
            }

            // initialize instance of input, output and filters
            source_raw_input = SourceRawDataInput::create();
            target_raw_output = TargetRawDataOutput::create();
            beauty_face_filter = BeautyFaceFilter::create();
            face_reshape_filter = FaceReshapeFilter::create();
            lipstick_filter = LipstickFilter::create();
            blusher_filter = BlusherFilter::create();

            // create image processing pipeline
            source_raw_input->addTarget(lipstick_filter)
                            ->addTarget(blusher_filter)
                            ->addTarget(face_reshape_filter)
                            ->addTarget(beauty_face_filter)
                            ->addTarget(target_raw_output);

            return true;
        }

        void setCallbacks() {
            // set callbacks for input instance
            source_raw_input->RegLandmarkCallback([&](std::vector<float> landmarks) {
                lipstick_filter->SetFaceLandmarks(landmarks);
                blusher_filter->SetFaceLandmarks(landmarks);
                face_reshape_filter->SetFaceLandmarks(landmarks);
            });

            // set callbacks for output instance
            target_raw_output->setPixelsCallbck([&](const uint8_t* outputData, int outputWidth, int outputHeight, int64_t ts) {
                // copy data to outputBuffer
                size_t dataSize = outputWidth * outputHeight * channel;
                for (size_t i = 0; i < dataSize; ++i) {
                    outputBuffer[i] = outputData[i];
                }
            });
        }

        void setParameters(float beautyValue, float whithValue, float thinFaceValue, float bigeyeValue, float lipstickValue, float blusherValue) {
            // check parameters
            if (beautyValue < 0.0) beautyValue = 0.0;
            if (beautyValue > 10.0) beautyValue = 10.0;
            if (whithValue < 0.0) whithValue = 0.0;
            if (whithValue > 10.0) whithValue = 10.0;
            if (thinFaceValue < 0.0) thinFaceValue = 0.0;
            if (thinFaceValue > 10.0) thinFaceValue = 10.0;
            if (bigeyeValue < 0.0) bigeyeValue = 0.0;
            if (bigeyeValue > 10.0) bigeyeValue = 10.0;
            if (lipstickValue < 0.0) lipstickValue = 0.0;
            if (lipstickValue > 10.0) lipstickValue = 10.0;
            if (blusherValue < 0.0) blusherValue = 0.0;
            if (blusherValue > 10.0) blusherValue = 10.0;

            // set parameters for filters
            beauty_face_filter->setBlurAlpha(beautyValue / 10);
            beauty_face_filter->setWhite(whithValue / 20);
            face_reshape_filter->setFaceSlimLevel(thinFaceValue / 200);
            face_reshape_filter->setEyeZoomLevel(bigeyeValue / 100);
            lipstick_filter->setBlendLevel(lipstickValue / 10);
            blusher_filter->setBlendLevel(blusherValue / 10);
        }

        uint8_t* forward(const uint8_t* inputData, int width, int height, int channel) {
            // initialize outputBuffer
            this->width = width;
            this->height = height;
            this->channel = channel;
            size_t dataSize = width * height * channel;
            outputBuffer = new uint8_t[dataSize];

            // upload input and run image processing pipeline
            source_raw_input->uploadBytes(inputData, width, height, width);

            return outputBuffer;
        }

        void release() {
            // release outputBuffer
            if (outputBuffer != nullptr) {
                delete[] outputBuffer;
                outputBuffer = nullptr;
            }
        }
};
