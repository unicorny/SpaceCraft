PROJECT(SpaceCraft CXX)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(spaceCraftSrc main Player Sektor Server Camera render/RenderBlockLoader render/RenderBlock BlockLoader Block
				  SektorChunk StellarBody Unit Planet render/GlobalRenderer ShaderProgram
                                  render/RenderStellarBody render/RenderPlanet GenerateNoisePlanet)
#IF(WIN32)
SET(spaceCraftSrc ${spaceCraftSrc} ../include/main.h ../include/Player.h ../include/Sektor.h ../include/Server.h
								   ../include/Camera.h ../include/render/RenderBlockLoader.h ../include/render/RenderBlock.h
								   ../include/BlockLoader.h ../include/Block.h ../include/SektorChunk.h
                                                                   ../include/StellarBody.h ../include/Unit.h ../include/Planet.h
                                                                   ../include/Vector3Unit.h ../include/render/GlobalRenderer.h ../include/ShaderProgram
                                                                   ../include/render/RenderStellarBody.h ../include/render/RenderPlanet.h ../include/GenerateNoisePlanet.h)
#ENDIF(WIN32)

SET(PA ../import/bin ../import/lib)
find_library(SDL_LIB SDL PATHS ${PA})
find_library(DR_ENGINE2_LIB Engine2 PATHS ${PA} REQUIRED)
find_library(DR_CORE2_LIB Core2 PATHS ${PA} REQUIRED)
find_library(LIB_NOISE_DLL libnoise PATHS ${PA} REQUIRED)
if(WIN32)
SET(SpaceCraftLibs ${SDL_LIB} ${DR_ENGINE2_LIB} ${DR_CORE2_LIB} glu32 opengl32 ${LIB_NOISE_DLL})
else()
SET(SpaceCraftLibs ${SDL_LIB} Engine2 Core2 glut ${LIB_NOISE_DLL})
endif()

if (MSVC)
ADD_DEFINITIONS(-DNODEFAULTLIB:library)
endif(MSVC)

ADD_EXECUTABLE(SpaceCraft ${spaceCraftSrc})

TARGET_LINK_LIBRARIES(SpaceCraft ${SpaceCraftLibs})

SET_TARGET_PROPERTIES(SpaceCraft PROPERTIES EXECUTABLE_OUTPUT_PATH ../)
