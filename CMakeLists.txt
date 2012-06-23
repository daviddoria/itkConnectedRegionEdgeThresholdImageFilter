cmake_minimum_required(VERSION 2.6)

INCLUDE(${CMAKE_SOURCE_DIR}/IJMacros.txt)

PROJECT(itkConnectedRegionEdgeThresholdImageFilter)

#The following lines are required to use Dart
ENABLE_TESTING()
INCLUDE(Dart)


SET(Required_Packages
ITK
)

#this foreach loads all of the packages that you specified as required.
#It shouldn't need to be modified.
FOREACH(Package ${Required_Packages})
  LOADPACKAGE(${Package})
ENDFOREACH(Package)

#######################
# 1 dimensional example
# Executable
SET(itkConnectedRegionEdgeThresholdImageFilter_Test1D)
ADD_EXECUTABLE(itkConnectedRegionEdgeThresholdImageFilter_Test1D itkConnectedRegionEdgeThresholdImageFilter_Test1D.cxx)
TARGET_LINK_LIBRARIES(itkConnectedRegionEdgeThresholdImageFilter_Test1D ${ITK_LIBRARIES})

#######################
# 2 dimensional example
ADD_EXECUTABLE(Demo2D Demo2D.cxx)
TARGET_LINK_LIBRARIES(Demo2D ${ITK_LIBRARIES})



########## Testing ############

SET(BuildTests OFF CACHE BOOL "BuildTests")
if(BuildTests)

  SET(CurrentExe "ImageCompare")
  ADD_EXECUTABLE(${CurrentExe} ImageCompare.cxx)
  TARGET_LINK_LIBRARIES(${CurrentExe} ${ITK_LIBRARIES})

  ADD_TEST(itkConnectedRegionEdgeThresholdImageFilter_Test1D itkConnectedRegionEdgeThresholdImageFilter_Test1D ${CMAKE_SOURCE_DIR}/Data/1D.png 120 5 5 ${CMAKE_SOURCE_DIR}/Testing/1D.png)
  ADD_TEST(CompareImage1D ImageCompare ${CMAKE_SOURCE_DIR}/Baselines/1D.png ${CMAKE_SOURCE_DIR}/Testing/1D.png)

  ADD_TEST(itkConnectedRegionEdgeThresholdImageFilter_Test2D itkConnectedRegionEdgeThresholdImageFilter_Test2D ${CMAKE_SOURCE_DIR}/Data/2D.png 135 95 5 5 ${CMAKE_SOURCE_DIR}/Testing/2D.png)
  ADD_TEST(CompareImage2D ImageCompare ${CMAKE_SOURCE_DIR}/Baselines/2D.png ${CMAKE_SOURCE_DIR}/Testing/2D.png)
endif()
