/*/*************************************************************************
 *                                                                         *
 * Core, Core-Lib for my programs, Core doesn't need any libraries	   *
 * Copyright (C) 2012, 2013, 2014 Dario Rekowski                           *
 * Email: dario.rekowski@gmx.de   Web: www.einhornimmond.de                *
 *                                                                         *
 * This program is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * any later version.                                                      *
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
/*
Color Klasse, �bernommen aus der TriBase-Engine von David Scherfgen
*/

#ifndef __DR_CORE2_COLOR__
#define __DR_CORE2_COLOR__

// Makros und Definitionen
#define DR_COLOR_CONV (0.003921568627450980392156862745098f) // = 1/255


class CORE2_API DRColor
{
public:

	// Variablen
	union
	{
		struct
		{
			DRReal r;	// Rot-, Gruen-, Blau- und Alphakomponente
			DRReal g;
			DRReal b;
			DRReal a;
		};

		struct
		{
			DRReal fRed;		// Rot-, Gruen-, Blau- und Alphakomponente
			DRReal fGreen;
			DRReal fBlue;
			DRReal fAlpha;
		};

		DRReal			c[4];			// Array der Komponenten
	};

		// Konstruktoren
	DRColor(): r(0.0f), g(0.0f), b(0.0f), a(0.0f)																																																		{}
	DRColor(const DRColor& c) : r(c.r), g(c.g), b(c.b), a(c.a)	{}
	DRColor(const DRVector2& a, const DRVector2& b) : r(a.x), g(a.y), b(b.x), a(b.y) {}
	DRColor(const DRReal f) : r(f), g(f), b(f), a(1.0f)																																								{}
	DRColor(const DRReal _r, const DRReal _g, const DRReal _b) : r(_r), g(_g), b(_b), a(1.0f)																															{}
	DRColor(const DRReal _r, const DRReal _g, const DRReal _b, const DRReal _a) : r(_r), g(_g), b(_b), a(_a)																											{}
	DRColor(const DRReal* pfComponent) : r(pfComponent[0]), g(pfComponent[1]), b(pfComponent[2]), a(pfComponent[3])																									{}
	DRColor(const u8 _r, const u8 _g, const u8 _b) : r((DRReal)(_r) * DR_COLOR_CONV), g((DRReal)(_g) * DR_COLOR_CONV), b((DRReal)(_b) * DR_COLOR_CONV), a(1.0f)													{}
	DRColor(const u8 _r, const u8 _g, const u8 _b, const u8 _a) : r((DRReal)(_r) * DR_COLOR_CONV), g((DRReal)(_g) * DR_COLOR_CONV), b((DRReal)(_b) * DR_COLOR_CONV), a((DRReal)(_a) * DR_COLOR_CONV)			{}
	DRColor(const u8* pComponent) : r((DRReal)(pComponent[0]) * DR_COLOR_CONV), g((DRReal)(pComponent[1]) * DR_COLOR_CONV), b((DRReal)(pComponent[2]) * DR_COLOR_CONV), a((DRReal)(pComponent[3]) * DR_COLOR_CONV)	{}
	DRColor(const u32 c) : r(DR_COLOR_CONV * (DRReal)(u8)(c >> 16)), g(DR_COLOR_CONV * (DRReal)(u8)(c >> 8)), b(DR_COLOR_CONV * (DRReal)(u8)(c)), a(DR_COLOR_CONV * (DRReal)(u8)(c >> 24))					{}


	// Casting-Operatoren
	operator u32 () const
	{
		return ((a >= 1.0f ? 255 : a <= 0.0f ? 0 : (u32)(a * 255.0f)) << 24) |
			   ((r >= 1.0f ? 255 : r <= 0.0f ? 0 : (u32)(r * 255.0f)) << 16) |
			   ((g >= 1.0f ? 255 : g <= 0.0f ? 0 : (u32)(g * 255.0f)) << 8)  |
			   (b >= 1.0f ? 255 : b <= 0.0f ? 0 : (u32)(b * 255.0f));
	}

	// Casting-Operatoren
	operator DRReal* ()			{return (DRReal*)(c);}

	// Zuweisungsoperatoren
	inline DRColor& operator += (const DRColor& c)	{r += c.r; g += c.g; b += c.b; a += c.a; return *this;}
	inline DRColor& operator -= (const DRColor& c)	{r -= c.r; g -= c.g; b -= c.b; a -= c.a; return *this;}
	inline DRColor& operator *= (const DRColor& c)	{r *= c.r; g *= c.g; b *= c.b; a *= c.a; return *this;}
	inline DRColor& operator *= (const DRReal f)	{r *= f; g *= f; b *= f; a *= f; return *this;}
	inline DRColor& operator /= (const DRColor& c)	{r /= c.r; g /= c.g; b /= c.b; a /= c.a; return *this;}
	inline DRColor& operator /= (const DRReal f)	{r /= f; g /= f; b /= f; a /= f; return *this;}

	// Arithmetische Operatoren
    inline DRColor operator + (const DRColor& b) const {return DRColor(this->r + b.r, this->g + b.g, this->b + b.b, this->a + b.a);}
    inline DRColor operator - (const DRColor& b) const {return DRColor(this->r - b.r, this->g - b.g, this->b - b.b, this->a - b.a);}
    inline DRColor operator - ()				 const {return DRColor(-r, -g, -b, a);}
    inline DRColor operator * (const DRColor& b) const {return DRColor(this->r * b.r, this->g * b.g, this->b * b.b, this->a * b.a);}
    inline DRColor operator * (const DRReal f)	 const {return DRColor(this->r * f, this->g * f, this->b * f, this->a * f);}
    inline DRColor operator / (const DRColor& b) const {return DRColor(this->r / b.r, this->g / b.g, this->b / b.b, this->a / b.a);}
    inline DRColor operator / (const DRReal f)	 const {return DRColor(this->r / f, this->g / f, this->b / f, this->a / f);}

    // Vergleichsoperatoren
    inline bool operator == (const DRColor& b) const {if(this->r != b.r) return false; if(this->g != b.g) return false; if(this->b != b.b) return false; return this->a == b.a;}
    inline bool operator != (const DRColor& b) const {if(this->r != b.r) return true;  if(this->g != b.g) return true;  if(this->b != b.b) return true;  return this->a != b.a;}

    //Weitere Funktionen
    inline DRColor interpolate(const DRColor& c2, const DRReal p) const {return *this + DRColor(p * (c2 - *this));}


};

inline DRColor operator * (const DRReal f, const DRColor& c)		{return DRColor(c.r * f, c.g * f, c.b * f, c.a * f);}

#endif //__DR_CORE2_COLOR__
