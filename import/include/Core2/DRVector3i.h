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

#ifndef __DR_CORE2_VECTOR3_I__
#define __DR_CORE2_VECTOR3_I__

class DRMatrix;

/*!  \brief repr&auml;sentiert einen 3D Vektor, bzw. Punkt

    eine Klasse zum einfachen Rechnen mit 3D Vektoren.
    Die Struktur der Klasse habe ich &uuml;bernommen von <a href="http://www.spieleprogrammierer.de/" target="_blank">David Scherfgens</a>
    tbVector3 Klasse aus seiner TriBase Engine
    \author Dario Rekowski
    \date 07.12.09
*/
class CORE2_API DRVector3i
{
public:
    //! setzt alle werte auf null
    //! \brief Standardkonstruktor
    DRVector3i() : x(0), y(0), z(0) {}
    //!  setzt die werte auf die &uuml;bergebenden
    //! \brief Konstruktor
    //! \param _x x Koordinate
    //! \param _y y Koordinate
    //! \param _z z Koordinate
    DRVector3i(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}
    //!  setzt alle Werte auf einen Wert
    //! \brief Konstruktor
    //! \param c setzt alle 3 Koordinaten auf den Wert von c
    DRVector3i(int c): x(c), y(c), z(c) {}
    //!  kopiert die Koordinaten eines anderen Vektors
    //! \brief Kopierkonstruktor
    //! \param v Vektor von dem die Koordinaten kopiert werden
    DRVector3i(const DRVector3i& v): x(v.x), y(v.y), z(v.z) {}
    //!  setzt alle werte auf die Werte des &uuml;bergebenden Arrays
    //! \brief Konstruktor
    //! \param ar ein Zeiger auf ein int array, mind. L&auml;nge 3
    DRVector3i(const int* ar) : x(ar[0]), y(ar[1]), z(ar[2]) {}

    union
    {
    //! zugriff via x, y z
       struct
       {
           int x;
           int y;
           int z;
       };
    //! zugriff via c array
       int c[3];
    };

    //! \brief () zugriffsoperator
    //! \param index [0,2] f&uuml;r die gesuchte koordinate (x = 0, y = 1, z = 2)
    //! \return eine Referenz, wenn ver&auml;ndert wird, wird der wert im Vektor ver&auml;ndert
    int& operator [] (int index) {return c[index];}
    //! \brief () zugriffsoperator, const
    //! \param index [0,2] f&uuml;r die gesuchte koordinate (x = 0, y = 1, z = 2)
    //! \return eine Kopie des Wertes, kann gefahrlos ver&auml;ndert werden
    int operator [] (int index) const {return c[index];}

    //! \brief Casting operator
    //! \return einen Zeiger auf das Werte Array
    operator int* () {return c;}
    
    operator const int* () const {return c;}

    //!  addiert auf den aktuellen Vektor, die Koordinaten des &uuml;bergebenden Vektors hinzu
    //! \brief additions operator
    //! \param v der Vektor der aufaddiert wird
    //! \return referenz auf diesen Vektor
    DRVector3i operator += (const DRVector3i& v) {x += v.x; y += v.y; z += v.z; return *this;}
    //! addiert den Vektor mit dem &uuml;bergebenden und liefert den neuen Vektor zur&uuml;ck
    //! \brief additions operator
    //! \param v der Vektor mit dem addiert wird
    //! \return einen neuen Vektor
    DRVector3i operator +  (const DRVector3i& v) const {return DRVector3i(x+v.x, y+v.y, z+v.z);}
    //!  subtrahiert vom aktuellen Vektor, die Koordinaten des &uuml;bergebenden Vektors
    //! \brief subtraktions operator
    //! \param v der Vektor der subtrahiert wird
    //! \return referenz auf diesen Vektor
    DRVector3i operator -= (const DRVector3i& v) {x -= v.x; y -= v.y; z -= v.z; return *this;}
    //! subtrahiert den Vektor vom &uuml;bergebenden und liefert den neuen Vektor zur&uuml;ck
    //! \brief subtraktions operator
    //! \param v der Vektor der subtrahiert wird
    //! \return einen neuen Vektor
    DRVector3i operator - (const DRVector3i& v) const {return DRVector3i(x-v.x, y-v.y, z-v.z);}
    //!  weist dem Vektor den Wert des &uuml;bergebenden zu
    //! \brief Zuweisungsoperator
    //! \param v &uuml;bergebender Vector
    //! \return Referenz auf diesen Vektor
    DRVector3i operator =  (const DRVector3i& v) {x = v.x; y = v.y; z = v.z; return *this;}
    //! dreht den Vektor um
    //! \brief Negationsoperator
    //! \return einen neuen Vektor, als Negation dieses Vektors
    DRVector3i operator -  ()		  const    {return DRVector3i(-x, -y, -z);}

    //!  teilt diesen Vektor durch einen Skalar, const
    //! \brief / operator
    //! \param f Skalar durch den geteilt wird
    //! \return einen neuen Vektor
    DRVector3i operator / (const int f)       const {return DRVector3i(x/f, y/f, z/f);}
    DRVector3i operator /= (const int f)            {*this = *this / f; return *this;}
    //!  multiplieziert diesen Vektor mit einem Skalar, const
    //! \brief operator: Vector * Scalar
    //! \param f Skalar mit dem multipliziert wird
    //! \return einen neuen Vektor
    DRVector3i operator *  (const int f)       const {return DRVector3i(x*f, y*f, z*f);}
    DRVector3i operator *= (const int f)             {*this = *this * f; return *this;}

    //! \brief transformiert diesen Vektor mit der Matrix m, const
    //! \param die &uuml;bergebende Mtrix m
    //! \return einen neuen, transformierten Vektor
    DRVector3i transformNormal(const DRMatrix& m) const;
    
    
    DRVector3i transformCoords(const DRMatrix& m, int* const pfOutW = NULL) const;
    
    
    //! \brief berechnet das Kreuzprodukt mit dem &uuml;bergebendem Vektor, const
    //! \param v2 der Vector mit dem das Kreuzprodukt errechnet werden soll
    //! \return einen neuen Vektor, das Kreuzprodukt aus diesem und dem &uuml;bergebendem Vektor
    DRVector3i cross(const DRVector3i& v2)         const {return DRVector3i(y * v2.z - z * v2.y, z * v2.x - x * v2.z, x * v2.y - y * v2.x);}
    //! \brief berechnet das Punktprodukt mit dem &uuml;bergebendem Vektor, const
    //! \param v2 der Vectro mit dem das Punktprodukt errechnet werden soll
    //! \return ein int Wert mit dem Punktprodukt
    int 	dot(const DRVector3i& v2)		 const {return x * v2.x + y * v2.y + z * v2.z;}

    //! \brief berechnet die L&auml;nge des Vektors, const
    //! \return die l&auml;nge des Vektors
//    int   length()                         const {return sqrtf(x*x + y*y + z*z);}
    //! \brief berechnet die quadratische L&auml;nge des Vektors, const
    //! \return die l&auml;nge des Vektors
    int   lengthSq()                       const {return (x*x + y*y + z*z);}

    //! \brief gibt den Vektor auf die Konsole aus, const
    //! \param name wenn angegeben, wird name mit auf der Konsole ausgegeben
    void print(const char* name = NULL) const
    {
        if(!name)
            printf("x: %d, y: %d, z: %d\n", x, y, z);
        else
            printf("%s: x: %d, y: %d, z: %d\n",name, x, y, z);
    }

    //! \brief Vergelichsoperator f&uuml;r ungleich, const
    //! \param a Vektor mit dem verglichen wird
    //! \return true bei ungleichheit, sonst false
    bool operator != (const DRVector3i& a) const
    {
        if(x != a.x || y != a.y || z != a.z) return true;
        return false;
    }
   /* //! \brief standard value x Achse (1,0,0)
    static const DRVector3i DRVector3i::X_AXIS;
    //! \brief standard value y Achse (0,1,0)
    static const DRVector3i DRVector3i::Y_AXIS;
    //! \brief standard value y Achse (0,0,1)
    static const DRVector3i DRVector3i::Z_AXIS;
    * */

private:

};

#endif //__DR_CORE2_VECTOR3__
