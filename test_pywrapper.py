import os
import ctypes
import numpy as np
import matplotlib.pyplot as plt

from PIL import Image


if __name__ == '__main__':

    # load input data
    input_image = Image.open('demo.png').convert('RGBA')
    input_data = np.asarray(input_image)
    height, width, channels = input_data.shape

    # load libgpupixel_pywrapper.dll
    current_dir = os.path.dirname(os.path.abspath(__file__))
    os.environ['PATH'] += os.pathsep + current_dir
    dll_file = 'libgpupixel_pywrapper.dll'
    dll_path = os.path.abspath(dll_file)
    lib = ctypes.CDLL(dll_path, winmode=0)

    # define types of arguments and returns
    lib.GPUPixelWrapper_create.restype = ctypes.c_void_p
    lib.GPUPixelWrapper_initialize.argtypes = [ctypes.c_void_p]
    lib.GPUPixelWrapper_setCallbacks.argtypes = [ctypes.c_void_p]
    lib.GPUPixelWrapper_setParameters.argtypes = [ctypes.c_void_p, ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float]
    lib.GPUPixelWrapper_run.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_uint8), ctypes.c_int, ctypes.c_int, ctypes.c_int]
    lib.GPUPixelWrapper_run.restype = ctypes.POINTER(ctypes.c_uint8)
    lib.GPUPixelWrapper_release.argtypes = [ctypes.c_void_p]

    # create GPUPixelWrapper instance
    gpu_pixel = lib.GPUPixelWrapper_create()

    # intialization
    if lib.GPUPixelWrapper_initialize(gpu_pixel):
        # set callbacks for input and output
        lib.GPUPixelWrapper_setCallbacks(gpu_pixel)
        
        # set parameters for filters
        beautyValue = 10.0
        whithValue = 10.0
        thinFaceValue = 10.0
        bigeyeValue = 10.0
        lipstickValue = 10.0
        blusherValue = 10.0
        lib.GPUPixelWrapper_setParameters(gpu_pixel, beautyValue, whithValue, thinFaceValue, bigeyeValue, lipstickValue, blusherValue)
        
        # run image processing pipeline
        output_data_ptr = lib.GPUPixelWrapper_run(gpu_pixel, input_data.ctypes.data_as(ctypes.POINTER(ctypes.c_uint8)), width, height, channels)
        # convert output pointer to numpy array
        output_data = np.ctypeslib.as_array(output_data_ptr, shape=(height, width, channels))

        plt.figure(figsize=(9, 7))
        plt.subplot(121)
        plt.title('Input')
        plt.imshow(input_data[:, :, :3])
        plt.axis('off')
        plt.subplot(122)
        plt.title('Output')
        plt.imshow(output_data[:, :, :3])
        plt.axis('off')
        plt.tight_layout()
        plt.show()

        # release
        lib.GPUPixelWrapper_release(gpu_pixel)
