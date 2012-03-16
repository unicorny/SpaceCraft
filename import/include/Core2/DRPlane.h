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


/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`�__\ /\ \ \ \  __\ /\ __\_ /\  __\   /�__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	DRPlane.h
	=========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Deklaration einer Ebenenklasse und von Ebenenfunktionen

	Autor:
	David Scherfgen
/*
Ebenen Klasse, �bernommen aus der TriBase-Engine von David Scherfgen
*/

/********************************************************************/

#ifndef __DR_CORE2_PLANE__
#define __DR_CORE2_PLANE__

// Die Ebenenklasse
class CORE2_API DRPlane
{
public:
	// Variablen
	union
	{
		struct
		{
			DRReal a;		// Variablen der Ebenengleichung
			DRReal b;
			DRReal c;
			DRReal d;
		};

/*  	struct
		{
			DRVector3	n;	// Normalenvektor
			DRReal		d;	// Distanz
		};
    */

		DRReal v[4];			// Array der Variablen
	};

	// Konstruktoren
	DRPlane() : a(0.0f), b(0.0f), c(0.0f), d(0.0f)							      							{}
	DRPlane(const DRPlane& p) : a(p.a), b(p.b), c(p.c), d(p.d)												{}
	DRPlane(const DRReal _a, const DRReal _b, const DRReal _c, const DRReal _d) : a(_a), b(_b), c(_c), d(_d){}
	DRPlane(const DRReal* pfValue) : a(pfValue[0]), b(pfValue[1]), c(pfValue[2]), d(pfValue[3])				{}
	DRPlane(const DRVector3& _n, DRReal _d): a(_n.x), b(_n.y), c(_n.z), d(_d)                               {}

	// Casting-Operatoren
	operator DRReal* () {return (DRReal*)(v);}

	// Zuweisungsoperatoren
	DRPlane& operator = (const DRPlane& p) {a = p.a; b = p.b; c = p.c; d = p.d; return *this;}

	// Vergleichsoperatoren
    inline bool operator == (const DRPlane& b) const {if(this->a != b.a) return false; if(this->b != b.b) return false; if(this->c != b.c) return false; return this->d == b.d;}
    inline bool operator != (const DRPlane& b) const {if(this->a != b.a) return true;  if(this->b != b.b) return true;  if(this->c != b.c) return true;  return this->d != b.d;}

    //**************************************************************************
    // Rechenoperationen
    inline DRPlane operator / (const DRReal f) const {return DRPlane(a/f, b/f, c/f, d/f);}


    // ******************************************************************
    // Funktionen deklarieren
    inline DRPlane	normalize()                     const  	{return *this / DRVector3(a,b,c).length();}
    inline DRReal	dotNormal(const DRVector3& v)	const	{return a * v.x + b * v.y + c * v.z;}
    //! \brief check if point is on plane
    //! 
    //! \return 0 if point is on plane, > 1 is point over the plane and < 1 is pointer underneat the plane
    inline DRReal	dotCoords(const DRVector3& v)	const	{return a * v.x + b * v.y + c * v.z + d;}

    static inline DRPlane	fromPointNormal(const DRVector3& p, const DRVector3& n)	{return DRPlane(n, -n.x * p.x - n.y * p.y - n.z * p.z);}
    static inline DRPlane	fromPoints(const DRVector3& v1, const DRVector3& v2, const DRVector3& v3)	{return fromPointNormal(v1, DRVector3(v3 - v2).cross(v1 - v2));}
    
    DRPlane transform(const DRMatrix& m) const;

    // ******************************************************************

};


#endif //__DR_CORE2_PLANE__
