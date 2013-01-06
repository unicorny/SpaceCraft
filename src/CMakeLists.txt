PROJECT(SpaceCraft CXX)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(spaceCraftSrc main
				  Player
				  Server
				  Camera
				  BlockLoader
				  Block
				  Unit
				  ShaderManager
				  ShaderProgram
				  DRGeometrieManager
				  DRGeometrieHeightfield
				  SektorObject
				  Observer
				  HeightMapTexture
				  
				  sektor/Sektor
				  sektor/RootSektor
				  sektor/SolarSystemSektor
				  sektor/PlanetSektor
				  sektor/SubPlanetSektor
				  sektor/SubPatchPlanetSektor
				  
				  render/RenderBlockLoader
				  render/RenderBlock
				  render/RenderSektor
				  render/RenderSolarSystem
				  render/RenderPlanet 
				  render/RenderSubPlanet 
				  render/RenderSubPatchPlanet
				  
				  render/renderToTexture/GlobalRenderer 
				  render/renderToTexture/RenderToTexture 
				  render/renderToTexture/RenderTextureToTexture
				  render/renderToTexture/RenderInStepsToTexture
				  render/renderToTexture/RenderNoisePlanetToTexture		
				  )
#IF(WIN32)
SET(spaceCraftSrc ${spaceCraftSrc} ../include/main.h
								   ../include/Player.h  
								   ../include/Server.h
								   ../include/Camera.h 
								   ../include/BlockLoader.h 
								   ../include/Block.h 
								   ../include/Unit.h 
								   ../include/Vector3Unit.h 
								   ../include/ShaderProgram
								   ../include/DRGeometrieHeightfield.h 
								   ../include/DRGeometrieManager.h 
								   ../include/ShaderManager.h 
								   ../include/SektorObject
								   ../include/Observer.h 
								   ../include/HeightMapTexture.h 
								   
								   ../include/sektor/Sektor.h
								   ../include/sektor/RootSektor.h
								   ../include/sektor/SolarSystemSektor.h 
								   ../include/sektor/PlanetSektor.h 
								   ../include/sektor/SubPlanetSektor.h 
								   ../include/sektor/SubPatchPlanetSektor.h
								   
								   ../include/render/RenderBlockLoader.h 
								   ../include/render/RenderBlock.h
								   ../include/render/RenderSektor.h 
								   ../include/render/RenderSolarSystem.h
								   ../include/render/RenderPlanet.h 
								   ../include/render/RenderSubPlanet.h 
								   ../include/render/RenderSubPatchPlanet.h 
								   
								   ../include/render/renderToTexture/GlobalRenderer.h 
								   ../include/render/renderToTexture/RenderToTexture.h
								   ../include/render/renderToTexture/RenderTextureToTexture.h
								   ../include/render/renderToTexture/RenderInStepsToTexture.h 
								   ../include/render/renderToTexture/RenderNoisePlanetToTexture.h 
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
