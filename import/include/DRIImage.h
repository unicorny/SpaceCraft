/*/*************************************************************************
 *                                                                       *
 * Engine, Engine-Lib interface class for many things,                   *
 * implementierung in extern libs                               		 *
 * Copyright (C) 2009, 20010, 2011 Dario Rekowski.						 *
 * Email: dariofrodo@gmx.de   Web: www.einhornimmond.de                  *
 *                                                                       *
 * This program is free software: you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation, either version 3 of the License, or     *
 * any later version.													 *
 *																		 *
 * This program is distributed in the hope that it will be useful,		 *
 * but WITHOUT ANY WARRAN TY; without even the implied warranty of		 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		 *
 * GNU General Public License for more details.							 *
 *																		 *
 * You should have received a copy of the GNU General Public License	 *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                       *
 *************************************************************************/

/*!
    Eine Klasse welche ein Bild repräsentiert,
    welches zum Beispiel als Textur verwendet werden kann
    @author Dario Rekowski
    @date 05.11.10
*/

#ifndef __DR_IIMAGE__
#define __DR_IIMAGE__
#ifdef _WIN32
#define __IMAGE_DLL_NAME__ "libImage.dll"
#else
#define __IMAGE_DLL_NAME__ "./libImage.so"
#endif

class ENGINE_API DRIImage : public DRInterface
{
public:
    DRIImage();
    virtual ~DRIImage();

    static DRIImage* newImage();
    static void deleteImage(DRIImage *pData);

    virtual GLenum getImageFormat();
    virtual unsigned int getWidth();
    virtual unsigned int getHeight();
    virtual unsigned char* getPixel();

    //! Functionen
    virtual DRReturn loadFromFile(const char* filename);
protected:


private:

};

#endif //__DR_IIMAGE__
