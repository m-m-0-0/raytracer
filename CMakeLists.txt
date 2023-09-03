cmake_minimum_required(VERSION 3.25)
project(RayTracer)

set(CMAKE_CXX_STANDARD 17)

STRING (REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
STRING (REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
STRING (REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")

add_executable(RayTracer main.cpp
        Image.cpp
        Image.h
        Types/Vector3.cpp
        Types/Vector3.h
        Types/Ray.h
        Types/Hit.cpp
        Types/Hit.h
        Camera.cpp
        Camera.h
        utils.h
        materials/Material.cpp
        materials/Material.h
        Objects/SceneObject.cpp
        Objects/SceneObject.h
        Types/Transform.cpp
        Types/Transform.h
        Objects/Sphere.cpp
        Objects/Sphere.h
        Scene.cpp
        Scene.h
        Types/Interval.cpp
        Types/Interval.h
        Objects/AAB.cpp
        Objects/AAB.h
        materials/Lambertian.cpp
        materials/Lambertian.h
        materials/Emissive.cpp
        materials/Emissive.h
        materials/Metallic.cpp
        materials/Metallic.h
        materials/Dielectric.cpp
        materials/Dielectric.h
        Texture.cpp
        Texture.h
        Objects/Model.cpp
        Objects/Model.h
        materials/VolumeTest.cpp
        materials/VolumeTest.h
        Types/Window.cpp
        Types/Window.h
        Types/Bounds.cpp
        Types/Bounds.h
        Types/Bounds.h
        BVHNode.cpp
        BVHNode.h
        Lights/Light.cpp
        Lights/Light.h
        Lights/PointLight.cpp
        Lights/PointLight.h
        Types/simd_utils.cpp
        Types/simd_utils.h
        Scenes/ManyBallsScene.cpp
        Scenes/ManyBallsScene.h
        Scenes/VolumeTestScene.cpp
        Scenes/VolumeTestScene.h
)


target_link_libraries(RayTracer PUBLIC -static)
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    #set(OMP_NUM_THREADS 16)
    #-fopenmp if gcc
    #/openmp if msvc
    if(MSVC)
        target_link_libraries(RayTracer PUBLIC OpenMP::OpenMP_CXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /openmp")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /openmp")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /openmp")
    else()
        target_link_libraries(RayTracer PUBLIC OpenMP::OpenMP_CXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fopenmp")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fopenmp")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fopenmp")
    endif()
endif()

#libjpeg-turbo
find_package(JPEG REQUIRED)
include_directories(${JPEG_INCLUDE_DIR})
target_link_libraries(RayTracer PUBLIC ${JPEG_LIBRARIES})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(RayTracer PUBLIC ${SDL2_LIBRARIES})

#oidn
set(OpenImageDenoise_DIR "${CMAKE_SOURCE_DIR}/oidn/lib/cmake/OpenImageDenoise-2.0.1")
find_package(OpenImageDenoise 2.0.1 REQUIRED)
target_link_libraries(RayTracer PUBLIC OpenImageDenoise)

#copy dlls
file(GLOB OIDN_DLLS "${CMAKE_SOURCE_DIR}/oidn/bin/*.dll")
add_custom_command(TARGET RayTracer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${OIDN_DLLS}
        $<TARGET_FILE_DIR:RayTracer>)


#fast-math
if(MSVC)
    target_compile_options(RayTracer PUBLIC /fp:fast)
else()
    target_compile_options(RayTracer PUBLIC -ffast-math)
endif()
