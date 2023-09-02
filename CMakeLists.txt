cmake_minimum_required(VERSION 3.25)
project(RayTracer)

set(CMAKE_CXX_STANDARD 17)

STRING (REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
STRING (REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
STRING (REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")

add_executable(RayTracer main.cpp
        CImg.h
        Image.cpp
        Image.h
        Vector3.cpp
        Vector3.h
        Ray.h
        Hit.cpp
        Hit.h
        Camera.cpp
        Camera.h
        utils.h
        Material.cpp
        Material.h
        Objects/SceneObject.cpp
        Objects/SceneObject.h
        Transform.cpp
        Transform.h
        Objects/Sphere.cpp
        Objects/Sphere.h
        Scene.cpp
        Scene.h
        Interval.cpp
        Interval.h
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
        Window.cpp
        Window.h
        Bounds.cpp
        Bounds.h
        Bounds.h
        BVHNode.cpp
        BVHNode.h
)


target_link_libraries(RayTracer PUBLIC -static)
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    set(OMP_NUM_THREADS 10)
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
