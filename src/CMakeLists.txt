PROJECT(SpaceCraft CXX)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

FILE(GLOB SRC "*.cpp")
FILE(GLOB SEKTOR_SRC "sektor/*.cpp")
FILE(GLOB RENDER_SRC "render/*.cpp")
FILE(GLOB RENDER_TO_TEXTURE_SRC "render/renderToTexture/*.cpp")
FILE(GLOB HEADER "../include/*.h")
FILE(GLOB SEKTOR_HEADER "../include/sektor/*.h")
FILE(GLOB RENDER_HEADER "../include/render/*.h")
FILE(GLOB RENDER_TO_TEXTURE_HEADER "../include/render/renderToTexture/*.h")

SET(spaceCraftSrc ${SRC} ${SEKTOR_SRC} ${RENDER_SRC} ${RENDER_TO_TEXTURE_SRC} 
				  ${HEADER} ${SEKTOR_HEADER} ${RENDER_HEADER} ${RENDER_TO_TEXTURE_HEADER})

if(MSVC)

# src
source_group("src\\sektor" FILES ${SEKTOR_SRC} )
source_group("src\\render" FILES ${RENDER_SRC} )
source_group("src\\render\\renderToTexture" FILES ${RENDER_TO_TEXTURE_SRC} )
source_group("src" FILES ${SRC})
# include
source_group("include\\sektor" FILES ${SEKTOR_HEADER} )
source_group("include\\render" FILES ${RENDER_HEADER} )
source_group("include\\render\\renderToTexture" FILES ${RENDER_TO_TEXTURE_HEADER} )
source_group("include" FILES ${HEADER})
#source_group("include" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/${REC2}" )
endif(MSVC)

SET(PA ../import/lib)
find_library(SDL2_LIB SDL2 PATHS ../import/lib)
find_library(DR_ENGINE2_LIB DREngine2 PATHS ../import/lib REQUIRED)
find_library(DR_CORE2_LIB DRCore2 PATHS ../import/lib REQUIRED)
find_library(LIB_NOISE_DLL libnoise PATHS ../import/lib REQUIRED)
if(WIN32)
SET(SpaceCraftLibs ${SDL2_LIB} ${DR_ENGINE2_LIB} ${DR_CORE2_LIB} glu32 opengl32 ${LIB_NOISE_DLL})
else(UNIX)
SET(SpaceCraftLibs SDL2 DREngine2 DRCore2 GLU ${LIB_NOISE_DLL})
endif(WIN32)

if (MSVC)
#ADD_DEFINITIONS(-DNODEFAULTLIB:library)
endif(MSVC)


ADD_EXECUTABLE(SpaceCraft ${spaceCraftSrc})

TARGET_LINK_LIBRARIES(SpaceCraft ${SpaceCraftLibs})

SET_TARGET_PROPERTIES(SpaceCraft PROPERTIES EXECUTABLE_OUTPUT_PATH ../)
