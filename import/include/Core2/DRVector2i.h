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

#ifndef __DR_CORE2_VECTOR2_I__
#define __DR_CORE2_VECTOR2_I__

/*
Vector2D Klasse, �bernommen aus der TriBase-Engine von David Scherfgen
*/
class CORE2_API DRVector2i
{
public:
	//Variablen
	union
	{
		struct
		{
			int x;
			int y;
		};

		struct
		{
			int u;
			int v;
		};
		int c[2];
	};

	//Konstruktoren
	// Konstruktoren
	DRVector2i(): x(0), y(0)																	{}
	DRVector2i(const DRVector2i& v) : x(v.x), y(v.y)								{}
	DRVector2i(const int f) : x(f), y(f)										{}
	DRVector2i(const int _x, const int _y) : x(_x), y(_y)					{}
	DRVector2i(const int* pComponent) : x(pComponent[0]), y(pComponent[1])	{}
        
	//Operatoren
	//Casting
	operator int* () {return (int*)(c);}

		// Zuweisungsoperatoren
	inline DRVector2i& operator = (const DRVector2i& v)	{x = v.x; y = v.y; return *this;}
	inline DRVector2i& operator += (const DRVector2i& v)	{x += v.x; y += v.y; return *this;}
	inline DRVector2i& operator -= (const DRVector2i& v)	{x -= v.x; y -= v.y; return *this;}
	inline DRVector2i& operator *= (const DRVector2i& v)	{x *= v.x; y *= v.y; return *this;}
	inline DRVector2i& operator *= (const int f)		{x *= f; y *= f; return *this;}
	inline DRVector2i& operator /= (const DRVector2i& v)	{x /= v.x; y /= v.y; return *this;}
	inline DRVector2i& operator /= (const int f)		{x /= f; y /= f; return *this;}

	//----------------------------------------------------------------------------------------------------------------------

	// Arithmetische Operatoren
    inline DRVector2i operator + (const DRVector2i& b) const	{return DRVector2i(this->x + b.x, this->y + b.y);}
    inline DRVector2i operator - (const DRVector2i& b) const	{return DRVector2i(this->x - b.x, this->y - b.y);}
    inline DRVector2i operator - ()					 const 	{return DRVector2i(-this->x, -this->y);}
    inline DRVector2i operator * (const DRVector2i& b) const	{return DRVector2i(this->x * b.x, this->y * b.y);}
    inline DRVector2i operator * (const int f)	 const  {return DRVector2i(this->x * f, this->y * f);}
    inline DRVector2i operator / (const DRVector2i& b) const	{return DRVector2i(this->x / b.x, this->y / b.y);}
    inline DRVector2i operator / (const int f)     const	{return DRVector2i(this->x / f, this->y / f);}

    //----------------------------------------------------------------------------------------------------------------------

	// rechenfunktionen:
	// Funktionen deklarieren
    inline int		lengthSq()	                                           const {return x*x + y*y;}
    inline int		dot(const DRVector2i& v2)                               const {return x * v2.x + y * v2.y;}
    inline DRVector2i	interpolateCoords(const DRVector2i& v2, const int p) const {return *this + DRVector2i((v2 - (*this) * p));}
//    inline DRVector2	min(const DRVector2& v2)							   const {return DRVector2(DR_MIN(x, v2.x), DR_MIN(y, v2.y));}
 //   inline DRVector2	max(const DRVector2& v2)							   const {return DRVector2(DR_MAX(x, v2.x), DR_MAX(y, v2.y));}
    //DRVector2	transformNormal_TranspInv(const DRMatrix& m)					const; // 2D-Normalenvektor mit Matrix multiplizieren
    
    

    //----------------------------------------------------------------------------------------------------------------------

    // Vergleichsoperatoren
    //! Mit Vorsicht zu genie�en!
    inline bool operator == (const DRVector2i& b) const {if(this->x != b.x) return false; return this->y == b.y;}
    inline bool operator != (const DRVector2i& b) const {if(this->x != b.x) return true;  return this->y != b.y;}
    inline bool	operator <  (const DRVector2i& b) const {if(this->x < b.x)  return true;  if(this->x == b.x && this->y < b.y) return true; return false;}
    inline bool	operator >  (const DRVector2i& b) const {if(this->x > b.x && this->y > b.y) return true; else return false;}
    inline bool operator <= (const DRVector2i& b) const {if(this->x <= b.x && this->y <= b.y) return true; else return false;}
    inline bool operator >= (const DRVector2i& b) const {if(this->x >= b.x && this->y >= b.y) return true; else return false;}

};

//********************************************************************************************************************++

#endif //__DR_CORE2_VECTOR2_I__
