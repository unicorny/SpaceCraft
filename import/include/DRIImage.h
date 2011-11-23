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
    Eine Klasse welche ein Bild repr�sentiert,
    welches zum Beispiel als Textur verwendet werden kann
    @author Dario Rekowski
    @date 05.11.10
*/

#ifndef __DR_IIMAGE__
#define __DR_IIMAGE__
#ifdef _WIN32
	#ifdef _MSC_VER
		#define __IMAGE_DLL_NAME__ "Image.dll"
	#else
		#define __IMAGE_DLL_NAME__ "libImage.dll"
	#endif
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

    virtual GLenum getImageFormat() const;
    virtual void setImageFormat(GLenum format);
    virtual u32 getWidth() const;
    virtual void setWidth(u32 width);
    virtual u32 getHeight() const;
    virtual void setHeight(u32 height);
    virtual void setSize(DRVector2 size);
    virtual DRVector2 getSize() const;
    virtual u8* getPixel();
    virtual void getPixel(DRColor* buffer);
    //! \brief setzt neue Pixel f&uuml;r das Bild
    //! 
    //! Dabei wird ein Pixelarray erwartet der gr&ouml;&szlig;e width*height*u32
    //! width und height müssen vorher gesetzt werden!
    //! \param pixel zeigt auf einen Puffer gefüllt mit Pixeln, Daten werden kopiert,
    //! Puffer kann danach freigegeben werden
    virtual void setPixel(u8* pixel);
    virtual void setPixel(DRColor* pixel);
    

    //! Functionen
    virtual DRReturn loadFromFile(const char* filename);
    virtual DRReturn saveIntoFile(const char* filename);
protected:


private:

};

#endif //__DR_IIMAGE__
