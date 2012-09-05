PROJECT(SpaceCraft CXX)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(spaceCraftSrc main Player sektor/Sektor Server Camera render/RenderBlockLoader render/RenderBlock BlockLoader Block
				  Unit render/GlobalRenderer ShaderProgram render/RenderPlanet 
				  sektor/SolarSystemSektor sektor/RootSektor sektor/PlanetSektor render/RenderSektor render/RenderSolarSystem
				  sektor/SubPlanetSektor render/RenderSubPlanet DRGeometrieHeightfield 
				  DRGeometrieManager render/RenderInStepsToTexture render/RenderNoisePlanetToTexture ShaderManager
				  render/RenderSubPatchPlanet sektor/SubPatchPlanetSektor Observer HeightMapTexture 
				  )
#IF(WIN32)
SET(spaceCraftSrc ${spaceCraftSrc} ../include/main.h ../include/Player.h ../include/sektor/Sektor.h ../include/Server.h
								   ../include/Camera.h ../include/render/RenderBlockLoader.h ../include/render/RenderBlock.h
								   ../include/BlockLoader.h ../include/Block.h ../include/Unit.h 
								   ../include/Vector3Unit.h ../include/render/GlobalRenderer.h ../include/ShaderProgram
								   ../include/render/RenderPlanet.h ../include/sektor/SolarSystemSektor.h ../include/sektor/RootSektor.h
								   ../include/sektor/PlanetSektor.h ../include/render/RenderSektor.h ../include/render/RenderSolarSystem.h
								   ../include/sektor/SubPlanetSektor.h ../include/render/RenderSubPlanet.h 
								   ../include/DRGeometrieHeightfield.h 
								   ../include/DRGeometrieManager.h ../include/render/RenderInStepsToTexture.h 
								   ../include/render/RenderNoisePlanetToTexture.h ../include/ShaderManager.h 
								   ../include/render/RenderSubPatchPlanet.h ../include/sektor/SubPatchPlanetSektor.h
								   ../include/Observer.h ../include/HeightMapTexture.h
								   )
#ENDIF(WIN32)

SET(PA ../import/bin ../import/lib)
find_library(SDL_LIB SDL PATHS ${PA})
find_library(DR_ENGINE2_LIB DREngine2 PATHS ${PA} REQUIRED)
find_library(DR_CORE2_LIB DRCore2 PATHS ${PA} REQUIRED)
find_library(LIB_NOISE_DLL libnoise PATHS ${PA} REQUIRED)
if(WIN32)
SET(SpaceCraftLibs ${SDL_LIB} ${DR_ENGINE2_LIB} ${DR_CORE2_LIB} glu32 opengl32 ${LIB_NOISE_DLL})
else(UNIX)
SET(SpaceCraftLibs ${SDL_LIB} DREngine2 DRCore2 GLU ${LIB_NOISE_DLL})
endif(WIN32)

if (MSVC)
ADD_DEFINITIONS(-DNODEFAULTLIB:library)
endif(MSVC)


ADD_EXECUTABLE(SpaceCraft ${spaceCraftSrc})

TARGET_LINK_LIBRARIES(SpaceCraft ${SpaceCraftLibs})

SET_TARGET_PROPERTIES(SpaceCraft PROPERTIES EXECUTABLE_OUTPUT_PATH ../)
