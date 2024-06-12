# Detect platform
# ------
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET(CURRENT_OS "linux")
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    SET(CURRENT_OS "windows")
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(CURRENT_OS "macos")
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "iOS")
    SET(CURRENT_OS "ios")
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Android")
    SET(CURRENT_OS "android")
ELSE()
    MESSAGE(FATAL_ERROR "NOT SUPPORT THIS SYSTEM")
ENDIF()

# Config build output path
# ------
SET(OUTPUT_INSTALL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../output")
SET(CMAKE_INCLUDE_OUTPUT_DIRECTORY "${OUTPUT_INSTALL_PATH}/include")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_INSTALL_PATH}/library/${CURRENT_OS}")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_INSTALL_PATH}/library/${CURRENT_OS}")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_INSTALL_PATH}/pywrapper/${CURRENT_OS}")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})


LINK_DIRECTORIES(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
SET(PYWRAPPER_RESOURCE_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
SET(COPY_DST_RUNTIME_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

 
# Config source and header file
# -------
# header include path
INCLUDE_DIRECTORIES(
    ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/third_party/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/third_party/stb
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/third_party/glad/include
)
 
# Add common source file
FILE(GLOB SOURCE_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/windows/*" 
    )
 

# Add platform source and header and lib link search path
IF(${CURRENT_OS} STREQUAL "windows")                                                        # windows
    # link libs find path
    LINK_DIRECTORIES( 
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/third_party/glfw/lib-mingw-w64)
ELSEIF(${CURRENT_OS} STREQUAL "linux")    
    # # Source 
    # FILE(GLOB GLAD_SOURCE_FILE  "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/src/*.c" )
    # list(APPEND SOURCE_FILES ${GLAD_SOURCE_FILE})
ENDIF()

# build type: dynamic library
# ------
ADD_LIBRARY(${PROJECT_NAME} SHARED ${SOURCE_FILES})
 
# link libs
# -------
IF(${CURRENT_OS} STREQUAL "linux")
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
                        gpupixel
                        GL
                        glfw)
    SET(GPUPIXEL_LIBS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libgpupixel.so)    
    FIlE(GLOB VNN_LIBS 
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/third_party/vnn/libs/${CURRENT_OS}/*
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath,./")
ELSEIF(${CURRENT_OS} STREQUAL "windows")
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
                        gpupixel
                        opengl32
                        glfw3
                        )
                        
    FIlE(GLOB VNN_LIBS 
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/third_party/vnn/libs/${CURRENT_OS}/x64/*
    )

    SET(GPUPIXEL_LIBS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libgpupixel.dll)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath,./")
ENDIF()

# copy resource file
# --------
# Add resource file
FILE(GLOB RESOURCE_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/resources/*"        
)
list(APPEND RESOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../pywrapper/windows/demo.png")
list(APPEND RESOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/../src/third_party/vnn/models/vnn_face278_data/face_pc[1.0.0].vnnmodel")

# copy resource files to output/pywrapper directory
ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} PRE_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy 
                ${RESOURCE_FILES} ${PYWRAPPER_RESOURCE_DIR}
                COMMENT "Copying resource files to output/pywrapper directory.")

# copy gpupixel and vnn lib
ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${VNN_LIBS} ${COPY_DST_RUNTIME_DIR}
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${GPUPIXEL_LIBS} ${COPY_DST_RUNTIME_DIR}
                )
