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

#ifndef __DR_CORE2_VECTOR2__
#define __DR_CORE2_VECTOR2__

/*
Vector2D Klasse, �bernommen aus der TriBase-Engine von David Scherfgen
*/
class DRMatrix;
class CORE2_API DRVector2
{
public:
	//Variablen
	union
	{
		struct
		{
			DRReal x;
			DRReal y;
		};

		struct
		{
			DRReal u;
			DRReal v;
		};
		DRReal c[2];
	};

	//Konstruktoren
	// Konstruktoren
	DRVector2(): x(0.0f), y(0.0f)																	{}
	DRVector2(const DRVector2& v) : x(v.x), y(v.y)								{}
	DRVector2(const DRReal f) : x(f), y(f)										{}
	DRVector2(const DRReal _x, const DRReal _y) : x(_x), y(_y)					{}
	DRVector2(const DRReal* pfComponent) : x(pfComponent[0]), y(pfComponent[1])	{}
        DRVector2(const int _x, const int _y): x(static_cast<float>(_x)), y(static_cast<float>(_y)) {}

	//Operatoren
	//Casting
	operator DRReal* () {return (DRReal*)(c);}

		// Zuweisungsoperatoren
	inline DRVector2& operator = (const DRVector2& v)	{x = v.x; y = v.y; return *this;}
	inline DRVector2& operator += (const DRVector2& v)	{x += v.x; y += v.y; return *this;}
	inline DRVector2& operator -= (const DRVector2& v)	{x -= v.x; y -= v.y; return *this;}
	inline DRVector2& operator *= (const DRVector2& v)	{x *= v.x; y *= v.y; return *this;}
	inline DRVector2& operator *= (const DRReal f)		{x *= f; y *= f; return *this;}
	inline DRVector2& operator /= (const DRVector2& v)	{x /= v.x; y /= v.y; return *this;}
	inline DRVector2& operator /= (const DRReal f)		{x /= f; y /= f; return *this;}

	//----------------------------------------------------------------------------------------------------------------------

	// Arithmetische Operatoren
    inline DRVector2 operator + (const DRVector2& b) const	{return DRVector2(this->x + b.x, this->y + b.y);}
    inline DRVector2 operator - (const DRVector2& b) const	{return DRVector2(this->x - b.x, this->y - b.y);}
    inline DRVector2 operator - ()					 const 	{return DRVector2(-this->x, -this->y);}
    inline DRVector2 operator * (const DRVector2& b) const	{return DRVector2(this->x * b.x, this->y * b.y);}
    inline DRVector2 operator * (const DRReal f)	 const  {return DRVector2(this->x * f, this->y * f);}
    inline DRVector2 operator / (const DRVector2& b) const	{return DRVector2(this->x / b.x, this->y / b.y);}
    inline DRVector2 operator / (const DRReal f)     const	{return DRVector2(this->x / f, this->y / f);}

    //----------------------------------------------------------------------------------------------------------------------

	// rechenfunktionen:
	// Funktionen deklarieren
    inline DRReal		length()                                               const {return sqrtf(x*x + y*y);}
    inline DRReal		lengthSq()	                                           const {return x*x + y*y;}
    inline DRVector2    normalize()                                            const {return (*this / this->length());}
    inline DRVector2    normalizeEx()                                          const {return (*this / (this->length() +  0.0001f));}
    inline DRReal		dot(const DRVector2& v2)                               const {return x * v2.x + y * v2.y;}
    inline DRReal		angle(const DRVector2& v2)                             const {return acosf((x * v2.x + y * v2.y) / (this->length() * v2.length()));}
    inline DRVector2	interpolateCoords(const DRVector2& v2, const DRReal p) const {return *this + DRVector2((v2 - (*this) * p));}
    inline DRVector2	interpolateNormal(const DRVector2& v2, const DRReal p) const {return this->interpolateCoords(v2, p).normalizeEx();}
//    inline DRVector2	min(const DRVector2& v2)							   const {return DRVector2(DR_MIN(x, v2.x), DR_MIN(y, v2.y));}
 //   inline DRVector2	max(const DRVector2& v2)							   const {return DRVector2(DR_MAX(x, v2.x), DR_MAX(y, v2.y));}
    DRVector2	transformCoords(const DRMatrix& m, DRReal* const pfOutW = NULL) const; // 2D-Vektor mit Matrix multiplizieren
    DRVector2	transformNormal(const DRMatrix& m)								const; // 2D-Normalenvektor mit Matrix multiplizieren
    //DRVector2	transformNormal_TranspInv(const DRMatrix& m)					const; // 2D-Normalenvektor mit Matrix multiplizieren
    
    

    //----------------------------------------------------------------------------------------------------------------------

    // Vergleichsoperatoren
    //! Mit Vorsicht zu genie�en!
    inline bool operator == (const DRVector2& b) const {if(this->x != b.x) return false; return this->y == b.y;}
    inline bool operator != (const DRVector2& b) const {if(this->x != b.x) return true;  return this->y != b.y;}
    inline bool	operator <  (const DRVector2& b) const {if(this->x < b.x)  return true;  if(this->x == b.x && this->y < b.y) return true; return false;}
    inline bool	operator >  (const DRVector2& b) const {if(this->x > b.x && this->y > b.y) return true; else return false;}
    inline bool operator <= (const DRVector2& b) const {if(this->x <= b.x && this->y <= b.y) return true; else return false;}
    inline bool operator >= (const DRVector2& b) const {if(this->x >= b.x && this->y >= b.y) return true; else return false;}

};

//********************************************************************************************************************++

#endif //__DR_CORE2_VECTOR2__
