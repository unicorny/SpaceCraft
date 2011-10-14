PROJECT(SpaceCraft CXX)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(spaceCraftSrc main Player Sektor Server Camera RenderBlockLoader RenderBlock BlockLoader Block
				  SektorChunk StellarBody Unit Planet GlobalRenderer)
#IF(WIN32)
SET(spaceCraftSrc ${spaceCraftSrc} ../include/main.h ../include/Player.h ../include/Sektor.h ../include/Server.h
								   ../include/Camera.h ../include/RenderBlockLoader.h ../include/RenderBlock.h
								   ../include/BlockLoader.h ../include/Block.h ../include/SektorChunk.h
                                                                   ../include/StellarBody.h ../include/Unit.h ../include/Planet.h
                                                                   ../include/Vector3Unit.h ../include/GlobalRenderer.h)
#ENDIF(WIN32)

SET(PA ../import/bin ../import/lib)
find_library(SDL_LIB SDL PATHS ${PA})
find_library(DR_ENGINE2_LIB Engine2 PATHS ${PA} REQUIRED)
find_library(DR_CORE2_LIB Core2 PATHS ${PA} REQUIRED)
if(WIN32)
SET(SpaceCraftLibs ${SDL_LIB} ${DR_ENGINE2_LIB} ${DR_CORE2_LIB} glu32 opengl32)
else()
SET(SpaceCraftLibs ${SDL_LIB} Engine2 Core2 GLEW glut)
endif()

ADD_EXECUTABLE(SpaceCraft ${spaceCraftSrc})

TARGET_LINK_LIBRARIES(SpaceCraft ${SpaceCraftLibs})

SET_TARGET_PROPERTIES(SpaceCraft PROPERTIES EXECUTABLE_OUTPUT_PATH ../)
