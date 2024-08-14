#include "gpupixel_wrapper.h"

extern "C" {
    typedef void* GPUPixelWrapperPtr;

    GPUPixelWrapperPtr GPUPixelWrapper_create() {
        return new GPUPixelWrapper();
    }

    bool GPUPixelWrapper_initialize(GPUPixelWrapperPtr self) {
        return reinterpret_cast<GPUPixelWrapper*>(self)->initialize();
    }

    void GPUPixelWrapper_setCallbacks(GPUPixelWrapperPtr self) {
        reinterpret_cast<GPUPixelWrapper*>(self)->setCallbacks();
    }

    void GPUPixelWrapper_setParameters(GPUPixelWrapperPtr self, float beautyValue, float whithValue, float thinFaceValue, float bigeyeValue, float lipstickValue, float blusherValue) {
        reinterpret_cast<GPUPixelWrapper*>(self)->setParameters(beautyValue, whithValue, thinFaceValue, bigeyeValue, lipstickValue, blusherValue);
    }

    void* GPUPixelWrapper_run(GPUPixelWrapperPtr self, const uint8_t* inputData, int width, int height, int channel) {
        return reinterpret_cast<GPUPixelWrapper*>(self)->forward(inputData, width, height, channel);
    }

    void GPUPixelWrapper_release(GPUPixelWrapperPtr self) {
        reinterpret_cast<GPUPixelWrapper*>(self)->release();
    }

    void GPUPixelWrapper_destroy(uint8_t* outputData) {
        free(outputData);
    }
}
