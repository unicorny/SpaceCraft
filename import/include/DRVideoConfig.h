/*/*************************************************************************
 *                                                                         *
 * EngineDLL, Engine for my programs, using SDL and OpenGL		   *
 * Copyright (C) 2012, 2013, 2014 Dario Rekowski.			   *
 * Email: dario.rekowski@gmx.de   Web: www.einhornimmond.de                *
 *                                                                         *
 * This program is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * any later version.							   *
 *									   *
 * This program is distributed in the hope that it will be useful,	   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of	   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	   *
 * GNU General Public License for more details.				   *
 *									   *
 * You should have received a copy of the GNU General Public License	   *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                         *
 ***************************************************************************/

/*! @author Dario Rekowski
 *  @date   04.11.10
 *
 * \brief Die Klasse repräsentiert eine Video Configuration
 * Sie wird verwendet um OpenGL/SDL zu initalisieren
 */

#ifndef __DR_ENGINE2_VIDEO_CONFIG__
#define __DR_ENGINE2_VIDEO_CONFIG__

#define VIDEOCONFIGVERSION 1.2f

class ENGINE2_API DRVideoConfig
{
public:
    //! \brief Standard Konstruktor
    //! \param width Fensterbreite, default ist 640
    //! \param height Fensterhöhe, default is 480
    //! \param bpp Bits per pixel, Pixeltiefe, default is 32
    //! \param fullscreen 1 für Vollbild, sonst 0
    //! \param zbuffer Z-Buffer Tiefe, default is 8 Bit
    //! \param stencilBuffer Stencil-Buffer Tiefe, default is 0 (disabled)
    //! \param multiSampling default is 0 (disabled)
    DRVideoConfig(int width = 640, int height = 480, int bpp = 32, int fullscreen = 0, int zbuffer = 8, int stencilBuffer = 0, int multiSampling = 0);
    
    //Kopierkonstruktor
    DRVideoConfig(const DRVideoConfig& cfg);

    //! \brief Standard Dekonstruktor
    ~DRVideoConfig();

    //! \brief aktuelle Configuration auf die Konsole ausgeben
    void print();

    //! \brief set pixeldeep, zbuffer, stencilbuffer and multisampling
    //! !call bevor SDL_SetVideoMode!
    void setGLParams();

    __inline__ int isFullscreen() {return mFullscreen;}
    __inline__ int getMultiSampling() {return mMultiSampling;}

    // get and setter
    __inline__ DRVector2 getResolution() {return DRVector2((DRReal)mWidth, (DRReal)mHeight);}
    
    __inline__ void setWidth(int width) {mWidth = width;}
    __inline__ void setHeight(int height) {mHeight = height;}
    __inline__ void setPixelDeep(int pixelDeep) {mPixelDeep = pixelDeep;}
    __inline__ void setFullscreen(int fullscreen) {mFullscreen = fullscreen;}
    __inline__ void setZBuffer(int ZBuffer) {mZBuffer = ZBuffer;}
    __inline__ void setStencilBuffer(int stencilBuffer) {mStencilBuffer = stencilBuffer;}
    __inline__ void setMultiSampling(int multiSampling) {mMultiSampling = multiSampling;}

private:
    DRReal mVersion;

    int mWidth;
    int mHeight;
    int mPixelDeep;

    int mFullscreen;

    int mZBuffer;
    int mStencilBuffer;
    int mMultiSampling;

};

#endif //__DR_ENGINE2_VIDEO_CONFIG__
