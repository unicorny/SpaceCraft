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

#ifndef __DR_ENGINE2_SAVE_TEXTURE__
#define __DR_ENGINE2_SAVE_TEXTURE__

/**!
 * class for saving texture to filesystem
 */

class ENGINE2_API DRSaveTexture
{
public:
    DRSaveTexture(const char* savingPath, DRTextureBufferPtr textureBuffer, GLuint stepSize = 16384);
    // no openGL funtion called
    ~DRSaveTexture();
    // create a pbo buffer with currently bind texture
    DRReturn getPixelsToSave();
    //! \param stepSize how many bytes pro call will be readed
    DRReturn putPixelsToImage();
    //! cann be called in other thread, no openGL functions called
    virtual DRReturn saveImage();
    
    __inline__ bool isTextureReadyToSave() {return mSavingState == 2;}
    __inline__ bool isTextureSaved() {return mSavingState == 3;}
    
protected:
    GLuint      mPboSaveID;// OpenGL Pixel Buffer Object to save texture
    DRTextureBufferPtr mTextureBuffer;
    short	mSavingState; //
    GLuint	mSavingCursor;
    u8* 	mSavingBuffer;
    DRString    mFilename;
    DRIImage*   mImage;
    DRVector2i  mSize;
    GLuint      mStepSize;
};

#endif //__DR_ENGINE2_SAVE_TEXTURE__