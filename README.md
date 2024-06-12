# GPUPixel Python Wrapper

A simple python wrapprer for [gpupixel](https://github.com/pixpark/gpupixel) using SourceRawDataInput and TargetRawDataOutput.<br/>
Code in this repository was only tested on Windows 10.

## 1. Modify and build gpupixel

### 1.1 Install CMake and MinGW64

- Download and install [cmake](https://cmake.org/download/).
- Download MinGW64 from [here](https://sourceforge.net/projects/mingw-w64/files/mingw-w64/mingw-w64-release/). Select **MinGW-W64 GCC-8.1.0 x86_64-posix-seh** to download.
- Unzip MinGW64 to ```MINGW64_DIR``` where the installation path of your choice.
- Add a system variable with the name ```MinGW_HOME```, which has the value ```MINGW64_DIR```. 
- Append ```MINGW64_DIR\bin``` to the system ```PATH```.

### 1.2 Clone and modify gpupixel

Clone gpupixel:
```bash
git clone --depth 1 --branch v1.2.5 git@github.com:pixpark/gpupixel.git
# or try to use the latest code
# git clone git@github.com:pixpark/gpupixel.git
```

Clone gpupixel_pywrapper:
```bash
git clone git@github.com:quqixun/gpupixel_pywrapper.git
```

Modify ```gpupixel\src\target\target_raw_data_output.cc``` according to [issue](https://github.com/pixpark/gpupixel/issues/66#issuecomment-2144263415).<br/>
Or you can copy [target_raw_data_output.cc](./src/target/target_raw_data_output.cc) to ```gpupixel\src\target``` to replace the older one.<br/>
Copy [stb_image_write.h](./src/third_party/stb/stb_image_write.h) to ```gpupixel\src\third_party\stb``` for writing image in test.<br/>
Find more infomation about [stb](https://github.com/nothings/stb).

### 1.3 Build gpupixel

Build library of gpupixel:
```bash
cd gpupixel
cmake -G "MinGW Makefiles" -B build -S src
cmake --build build --config Debug  # or Release
```
Then, ```libgpupixel.dll``` can be found in ```gpupixel\output\library\windows```.

## 2. C++ Wrapper (Optional)

[GPUPixelWrapper](./wrapper/windows/gpupixel_wrapper.h) is a C++ encapsulation for gpupixel using SourceRawDataInput and TargetRawDataOutput.<br/>
[cli.cc](./wrapper/windows/cli.cc) demonstrates its usage.<br/>
Copy [wrapper](./wrapper/) directory to ```gpupixel``` and build it:
```bash
cd gpupixel
cmake -G "MinGW Makefiles" -B wrapper/build -S wrapper
cmake --build wrapper/build --config Debug  # or Release
```
In ```gpupixel\output\wrapper\windows```, run test:
```bash
gpupixel_cli.exe demo.png output.png 10 10 10 10 10 10
```

This step is not necessary for building python wrapper.

## 3. Python Wrapper

Finally, we just wrap the GPUPixelWrapper in [gpupixel_pywrapper.cc](./pywrapper/windows/gpupixel_pywrapper.cc) and it's ready for python to call.<br/>
Copy [pywrapper](./pywrapper/) directory to ```gpupixel``` and build it:
```bash
cd gpupixel
cmake -G "MinGW Makefiles" -B pywrapper/build -S pywrapper
cmake --build pywrapper/build --config Debug  # or Release
```
```libgpupixel_pywrapper.dll``` can be found in ```gpupixel\output\pywrapper\windows```.<br/>
Copy [test_pywrapper.py](./test_pywrapper.py) to ```gpupixel\output\pywrapper\windows```, and run ```python test_pywrapper.py``` to check result.

---

I'm not a C++ developer, the code in this repository was developed with the assistance of ChatGPT.<br/>
This may not be the best solution, but it is good enough for my project at the moment.<br/>
I am always looking forward to a better one.