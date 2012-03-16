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

#ifndef __DR_ENGINE2_TEXT__
#define __DR_ENGINE2_TEXT__

#define DR_FONT_TEXTBUFFERSIZE 256

// Flags zum genauen definieren wie der Text gezeichnet werden soll
enum DRTEXTFlags
{
	DR_FF_ALIGN_HLEFT = 0,	//Text linksb�ndig
	DR_FF_ALIGN_HCENTER = 1, //Text zentriert
	DR_FF_ALIGN_HRIGHT = 2,  //Text rechtsb�ndig
	DR_FF_ALIGN_VBOTTOM = 4, //Text unten ausrichten
	DR_FF_ALIGN_VCENTER = 8, //Text vertikal zentrieren
	DR_FF_ALIGN_VTOP = 16,	//Text oben ausrichten
	DR_FF_RELATIVE = 32,		//Koordinaten sind relativ
	DR_FF_RELATIVSCALING = 64//Skalierung ist relativ
};

#if (_MSC_VER >= 1200 && _MSC_VER < 1310)
enum ENGINE_API DRTEXTFlags;
#endif

/*! \brief eine Hilfs-Klasse zum text rendern mittels OpenGL

    Klasse zum laden von Schriftarten Texturen,
    bestehend aus einer Textur Datei mit Alphakanal (&uuml;blicherweise *.tga)
    und einer Info datei *.tbf die ein Objekt der Struktur SFontInfo enth&aauml;lt.
    Au�erdem wird die klasse zum konfigurieren von OpenGL verwendet um Text in
    2D zeichnen zu k&ouml;nnen.

    Programmierer: Dario Rekowski
    abgeleitet aus TriBase-Engine von <a href="http://www.spieleprogrammierer.de/" target="_blank">David Scherfgens</a> und
    aus dem Tutorial: <a href="http://www.joachimrohde.com/cms/xoops/modules/articles/article.php?id=24">"2D Texturierte Fonts"</a> im <a href="http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=17">Original</a> von NeHe & Giuseppe D'Agata

    \author Dario Rekowski
    \date 04.01.09
*/
class ENGINE2_API DRFont
{
public:
	//Konstruktoren
	//!\brief Standardkonstruktor, setzt alles auf null
	DRFont();
	//! \brief Standard Kopie Kontruktor
	//! \param font zeiger auf ein anderes font
	DRFont(const DRFont* font);
	//! \brief deconstruktor, freigeben der Textur
	~DRFont();

	//initen
    //! \brief initaliseren des font objektes
    //! \param pcTGAFilename filename der Textur
    //! \param pfTBFFilename filename des font infos files, welches die SFontInfo struct enth&auml;lt
    //! \return _OK wenn geladen werden konnte
    //! \return _ZERO_POINTER wenn einer der beiden Parameter null ist
    //! \return _ERROR wenn ein Fehler beim laden einer der Datein auftrat
	DRReturn init(const char* pcTGAFilename, const char* pcTBFFilename);

    //! \brief setzen der Matrix und OenGL States zum Text rendern
    //! \param bSmall wenn auf true, wenn wird nur die Textur gesetzt und der blendmodus festgelegt
	DRReturn begin(bool bSmall = false);
	//! \brief r&uuml;ckg&auml;ngig machen der Renderstates
	DRReturn end();

    //! \brief liefert die H&ouml;he der Buchstaben
    //! \return die H&ouml;he als float
	__inline__	float getTextHeight() {return m_FInfo.fHeight;};

	//! \brief berechnet die Breite eines spezifischen Textes
	//! \param pcText der Text
	//! \param l&auml;nge des Textes, oder -1 wenn die L&auml;nge automatisch bestimmt werden soll
	//! \param fSpace die Breite eines Leerzeichens
	//! \return die Breite als float
	float computeTextWidth(const char* pcText, int iLength = -1, const float fSpace = 0.0f);

	struct SFontInfo
	{
		float		fWidth;				//Breite der Schriftart
		float		fHeight;			//H�he "
		float		fTextureWidth;		//Breite der Textur
		float		fTextureHeight;		//H�he - " -
		DRVector2		avTopLeft[256];		//Linke obere Koordinate jedes Zeichens
		DRVector2		avBottomRight[256];	//Rechte untere		-	"	-
		float		afCharWidth[256];	//Breite jedes Zeichens in Pixeln
	};

	SFontInfo	m_FInfo;				//Info
	DRTextur*	mTexture;			//TextureID
	DRVector2	m_vScreenSize;
	DRObjekt*	m_pCamera;
};


//----------------------------------------------------------------------------------------------------------------------

/*! \brief zum text rendern mittels OpenGL

    Klasse zum konfigurieren und rendern von Text mittels OpenGL

    Programmierer: Dario Rekowski
    abgeleitet aus TriBase-Engine von <a href="http://www.spieleprogrammierer.de/" target="_blank">David Scherfgens</a> und
    aus dem Tutorial: <a href="http://www.joachimrohde.com/cms/xoops/modules/articles/article.php?id=24">"2D Texturierte Fonts"</a> im <a href="http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=17">Original</a> von NeHe & Giuseppe D'Agata

    \author Dario Rekowski
    \date 04.01.09
*/
class ENGINE2_API DRText
{
public:
    //! \brief Konstruktor welcher einen Zeiger auf ein FOnt Objekt erwartet
    //! \param pFont zeiger auf ein initalisiertes Font Objekt
	DRText(DRFont* pFont);
	//! \brief Kopierkonstruktor, kopiert die Daten des anderen Text Objektes
	DRText(DRText* pText);
	//! \brief Dekonstrukor, gibt reservierten Speicher frei
	~DRText() {resetText();};

	//! \brief zeichnet den Text
	//! \return _OK wenn alles Okay
	//! \return _ERROR wenn ein Fehler auftrat
	DRReturn drawText();

    //! \brief setze den Text
    //! \param _pcText zeiger auf ein char Array dessen inhalt kopiert wird
	void setText(char* _pcText);
	//! \brief setzt den Text, erwartet die gleichen Parameter wie printf
	//! \param _pcText char array mit Text und Platzhaltern, wie bei printf
	void setText(const char* _pcText, ...);

	//inline
    //! \brief setzt die Position des Textes, beeinflusst durch die Flags
    //! \param _vPosition die neue Position, absolut oder relativ, je nach Flag
	__inline__ void setPosition(DRVector2 _vPosition) {m_vPosition = _vPosition;};
	//! \brief setzt die 1. Farbe
	//! \param _Color die 1. Farbe
	__inline__ void setColor1(DRColor _Color) {Color1 = _Color;};
	//! \brief setzt die 2. Farbe
	//! \brief _Color die 2. Farbe
	__inline__ void setColor2(DRColor _Color) {Color2 = _Color;};
	//! \brief setzt die 1. und die 2. Farbe auf den selben Wert
	//! \param _bColor farbe 1 und 2
	__inline__ void setColor12(DRColor _bColor) {Color1 = Color2 = _bColor;};
	//! \brief setzt durchsichtigkeit der Schrift
	//! \param fAlpha alpha wert, durchsichtigkeit
	__inline__ void setAlpha(float fAlpha) {Color1.a = Color2.a = fAlpha;};
	//! \brief setzte Flags
	//! \param _uiFlags die Flags, beschrieben durch das FontFlags enum
	__inline__ void setFlags(unsigned int _uiFlags) {uiFlags = _uiFlags;};
	//! \brief setzte skalierung des Textes
	//! \param _vScaling die Skalierung in x und y
	__inline__ void setScaling(DRVector2 _vScaling) {m_vScaling = _vScaling;};
	//! \brief die breite des Space Zeichens einstellen
	//! \param _fSpace die neue breite des Space Zeichens
	__inline__ void setSpace(float _fSpace) {m_fSpace = _fSpace;};

    //! \brief liefert die aktulle Skalierung
	__inline__ DRVector2 getScaling() {return m_vScaling;};
	//! \brief berechnet die H&ouml;he des Textes
	//! \return die H&ouml;he des Textes als float
    float     getTextHeight();
    //! \brief liefert einen Zeiger auf die verwendete Schriftart (font objekt)
    //! \return zeiger auf font
	__inline__ DRFont* getFont() {return m_pFont;};

	//! \brief liefert die aktuellen Flags
	//! \return die flags als unsigned int
	__inline__ unsigned int getFlags() {return uiFlags;};

private:
    //! \brief setzt den Text auf Null
	void resetText();

	DRVector2 m_vPosition;
	unsigned int uiFlags;
	int iLength;
	DRColor Color1;
	DRColor Color2;
	DRVector2 m_vScaling;
	float m_fSpace;
	float fItalic;
	float fLineSpace;
	float m_fTabStops;
	float fXOffset;
	char* pcText;
	char acText[DR_FONT_TEXTBUFFERSIZE];

	DRFont* m_pFont;

	// Vertexformat
	struct SFontVertex
	{
		DRVector3		vPosition;	// Positionsangabe
		DRColor		Diffuse;	// Streufarbe
		DRVector2		vTexture;	// Texturkoordinaten
	};
};
#endif //__DR_ENGINE2_TEXT__