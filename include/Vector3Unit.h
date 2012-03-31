/* 
 * File:   Vector3Unit.h
 * Author: Dario
 *
 * Created on 21. September 2011, 11:45
 */

#ifndef __SPACE_CRAFT_VECTOR3_UNIT_H
#define	__SPACE_CRAFT_VECTOR3_UNIT_H

#include "Unit.h"

class Vector3Unit
{
public:
//! setzt alle werte auf null
    //! \brief Standardkonstruktor
    Vector3Unit(UnitTypes type = M): x(0.0, type), y(0.0, type), z(0.0, type) {}
    //!  setzt die werte auf die &uuml;bergebenden
    //! \brief Konstruktor
    //! \param _x x Koordinate
    //! \param _y y Koordinate
    //! \param _z z Koordinate
    Vector3Unit(Unit _x, Unit _y, Unit _z) : x(_x), y(_y), z(_z) {if(!x.compareType(y) || !y.compareType(z)) LOG_WARNING("x, y or z haven't the same type");}
    Vector3Unit(double _x, double _y, double _z, UnitTypes type = M) : x(_x, type), y(_y, type), z(_z, type) {}
    //!  setzt alle Werte auf einen Wert
    //! \brief Konstruktor
    //! \param c setzt alle 3 Koordinaten auf den Wert von c
    Vector3Unit(Unit c): x(c), y(c), z(c) {}
    //!  kopiert die Koordinaten eines anderen Vektors
    //! \brief Kopierkonstruktor
    //! \param v Vektor von dem die Koordinaten kopiert werden
    Vector3Unit(const Vector3Unit& v): x(v.x), y(v.y), z(v.z) {}
    Vector3Unit(DRVector3 v, UnitTypes type = M): x(v.x, type), y(v.y, type), z(v.z, type) {}
    //!  setzt alle werte auf die Werte des &uuml;bergebenden Arrays
    //! \brief Konstruktor
    //! \param ar ein Zeiger auf ein Unit array, mind. L&auml;nge 3
    Vector3Unit(const Unit* ar) : x(ar[0]), y(ar[1]), z(ar[2]) {if(!x.compareType(y) || !y.compareType(z)) LOG_WARNING("x, y or z haven't the same type");}
    Vector3Unit(const double* ar, UnitTypes type = M) : x(ar[0], type), y(ar[1], type), z(ar[2], type) {}

    Unit x;
    Unit y;
    Unit z;    

    //! \brief () zugriffsoperator
    //! \param index [0,2] f&uuml;r die gesuchte koordinate (x = 0, y = 1, z = 2)
    //! \return eine Referenz, wenn ver&auml;ndert wird, wird der wert im Vektor ver&auml;ndert
    Unit& operator [] (int index) {switch(index){case 0: return x; case 1: return y; case 2: return z;}}
    //! \brief () zugriffsoperator, const
    //! \param index [0,2] f&uuml;r die gesuchte koordinate (x = 0, y = 1, z = 2)
    //! \return eine Kopie des Wertes, kann gefahrlos ver&auml;ndert werden
    Unit operator [] (int index) const {switch(index){case 0: return x; case 1: return y; case 2: return z; default: return Unit();}}

    //operator DRVector3() {return DRVector3((float)x, (float)y, (float)z);}
    DRVector3 getVector3() {return DRVector3((float)x, (float)y, (float)z);}

    //!  addiert auf den aktuellen Vektor, die Koordinaten des &uuml;bergebenden Vektors hinzu
    //! \brief additions operator
    //! \param v der Vektor der aufaddiert wird
    //! \return referenz auf diesen Vektor
    Vector3Unit operator += (const Vector3Unit& v) {x += v.x; y += v.y; z += v.z; return *this;}
    //! addiert den Vektor mit dem &uuml;bergebenden und liefert den neuen Vektor zur&uuml;ck
    //! \brief additions operator
    //! \param v der Vektor mit dem addiert wird
    //! \return einen neuen Vektor
    Vector3Unit operator +  (const Vector3Unit& v) const {return Vector3Unit(x+v.x, y+v.y, z+v.z);}
    
    Vector3Unit operator -= (const Vector3Unit& v) {x -= v.x; y -= v.y; z -= v.z; return *this;}
    //! subtrahiert den Vektor vom &uuml;bergebenden und liefert den neuen Vektor zur&uuml;ck
    //! \brief subtraktions operator
    //! \param v der Vektor der subtrahiert wird
    //! \return einen neuen Vektor
    Vector3Unit operator - (const Vector3Unit& v) const {return Vector3Unit(x-v.x, y-v.y, z-v.z);}
    //!  weist dem Vektor den Wert des &uuml;bergebenden zu
    //! \brief Zuweisungsoperator
    //! \param v &uuml;bergebender Vector
    //! \return Referenz auf diesen Vektor
    Vector3Unit operator =  (const Vector3Unit& v) {x = v.x; y = v.y; z = v.z; return *this;}
    //! dreht den Vektor um
    //! \brief Negationsoperator
    //! \return einen neuen Vektor, als Negation dieses Vektors
    Vector3Unit operator -  ()		  const    {return Vector3Unit(-x, -y, -z);}

    //!  teilt diesen Vektor durch einen Skalar, const
    //! \brief / operator
    //! \param f Skalar durch den geteilt wird
    //! \return einen neuen Vektor
    Vector3Unit operator / (const Unit f)       const {return Vector3Unit(x/f, y/f, z/f);}
    Vector3Unit operator / (const double f)      const {return Vector3Unit(x/f, y/f, z/f);}
    //!  multiplieziert diesen Vektor mit einem Unit Skalar, const
    //! \brief * operator
    //! \param f Skalar mit dem multipliziert wird
    //! \return einen neuen Vektor
    Vector3Unit operator *  (const Unit f)       const {return Vector3Unit(x*f, y*f, z*f);}
    Vector3Unit operator *= (const Unit f)             {*this = *this * f; return *this;}
    
    //!  multiplieziert diesen Vektor mit einem Skalar, const
    //! \brief * operator
    //! \param f Skalar mit dem multipliziert wird
    //! \return einen neuen Vektor
    Vector3Unit operator *  (const double f)       const {return Vector3Unit(x*f, y*f, z*f);}
    Vector3Unit operator *= (const double f)             {*this = *this * f; return *this;}
 
    //! \brief normalisiert diesen vektor (brint die L&auml;nge auf eins), const
    //! \return einen neuen normalisierten Vektor
    Vector3Unit normalize()                      const  {return *this / length();}
    //! \brief berechnet das Kreuzprodukt mit dem &uuml;bergebendem Vektor, const
    //! \param v2 der Vector mit dem das Kreuzprodukt errechnet werden soll
    //! \return einen neuen Vektor, das Kreuzprodukt aus diesem und dem &uuml;bergebendem Vektor
    Vector3Unit cross(const Vector3Unit& v2)         const {return Vector3Unit(y * v2.z - z * v2.y, z * v2.x - x * v2.z, x * v2.y - y * v2.x);}
    //! \brief berechnet das Punktprodukt mit dem &uuml;bergebendem Vektor, const
    //! \param v2 der Vectro mit dem das Punktprodukt errechnet werden soll
    //! \return ein Unit Wert mit dem Punktprodukt
    Unit 	dot(const Vector3Unit& v2)		 const {return x * v2.x + y * v2.y + z * v2.z;}

    //! \brief berechnet die L&auml;nge des Vektors, const
    //! \return die l&auml;nge des Vektors
    Unit   length()                         const {return Unit(sqrt((double)(x*x + y*y + z*z)), x.getType());}
    //! \brief berechnet die quadratische L&auml;nge des Vektors, const
    //! \return die l&auml;nge des Vektors
    Unit   lengthSq()                       const {return (x*x + y*y + z*z);}
    
    Vector3Unit convertTo(UnitTypes type) const {return Vector3Unit(x.convertTo(type), y.convertTo(type), z.convertTo(type));}

    //! \brief gibt den Vektor auf die Konsole aus, const
    //! \param name wenn angegeben, wird name mit auf der Konsole ausgegeben
    void print(const char* name = NULL) const
    {
        if(!name)
            printf("x: %s, y: %s, z: %s\n", x.print().data(), y.print().data(), z.print().data());
        else
            printf("%s: x: %s, y: %s, z: %s\n",name, x.print().data(), y.print().data(), z.print().data());
    }

    //! \brief Vergelichsoperator f&uuml;r ungleich, const
    //! \param a Vektor mit dem verglichen wird
    //! \return true bei ungleichheit, sonst false
    bool operator != (const Vector3Unit& a) const
    {
        if(x != a.x || y != a.y || z != a.z) return true;
        return false;
    }
    
   /* //! \brief standard value x Achse (1,0,0)
    static const Vector3Unit Vector3Unit::X_AXIS;
    //! \brief standard value y Achse (0,1,0)
    static const Vector3Unit Vector3Unit::Y_AXIS;
    //! \brief standard value y Achse (0,0,1)
    static const Vector3Unit Vector3Unit::Z_AXIS;
    * */

private:
};

// Unit * Vector3 = Vector3Unit
__inline__ Vector3Unit operator * (const Unit& u, const DRVector3& v) {return Vector3Unit(v, u.getType())*u;}

#endif	/* __SPACE_CRAFT_VECTOR3_UNIT_H */

