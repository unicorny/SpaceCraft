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

#ifndef __DR_CORE2_MATRIX__
#define __DR_CORE2_MATRIX__


#include "stdio.h"
#include "memory.h"
#include "math.h"

#include "DRVector3.h"

/*! \brief Diese Klasse representiert eine DRMatrix

    Zum rechnen und erstellen von Matritzen
    Die Struktur der Klasse habe ich &uuml;bernommen von <a href="http://www.spieleprogrammierer.de/" target="_blank">David Scherfgens</a>
    tbMatrix Klasse aus seiner TriBase Engine
    \author Dario Rekowski
    \date 07.12.09
*/
class CORE2_API DRMatrix
{
public:
    // Konstruktoren
    //! setzt die DRMatrix auf die Identit&auml;ts DRMatrix
    //! \brief Standardkonstruktor
    DRMatrix() {*this = identity();}

    //! \brief Kopierkonstruktor
    //!     erwartet eine DRMatrix deren Werte &uuml;bernommen werden
    //! \param m andere DRMatrix von der die Werte genommen werden
    DRMatrix(const DRMatrix& m) {memcpy(n, m.n, sizeof(float)*16);}

    // Spalte ist groß bei opengl...
    //! \brief Konstruktor, der jeden einzelnen DRMatrix Wert erwartet
    //! \param _m00 = x Achse x Koordinate
    //! \param _m01 = x Achse y Koordinate
    //! \param _m02 = x Achse z Koordinate
    //! \param _m03 = 0
    //! \param _m04 = y Achse x Koordinate
    //! \param _m05 = y Achse y Koordinate
    //! \param _m06 = y Achse z Koordinate
    //! \param _m07 = 0
    //! \param _m08 = z Achse x Koordinate
    //! \param _m09 = z Achse y Koordinate
    //! \param _m10 = z Achse z Koordinate
    //! \param _m11 = 0
    //! \param _m12 x Translation
    //! \param _m13 y Translation
    //! \param _m14 z Translation
    //! \param _m15 = 1
    DRMatrix(float _m00, float _m04, float _m08, float _m12,
       float _m01, float _m05, float _m09, float _m13,
       float _m02, float _m06, float _m10, float _m14,
       float _m03, float _m07, float _m11, float _m15) : m00(_m00), m04(_m01), m08(_m02), m12(_m03),
                                                         m01(_m04), m05(_m05), m09(_m06), m13(_m07),
                                                         m02(_m08), m06(_m09), m10(_m10), m14(_m11),
                                                         m03(_m12), m07(_m13), m11(_m14), m15(_m15) {}
    //! \brief Kontruktor
    //!    erwartet einen Wert, setzt alle Felder auf diesen Wert
    //! \param f ein float Wert auf den alle Felder gesetzt werden
    DRMatrix(const float f) {memset(n, (int)f, sizeof(float)*16);}
    //! \brief Konstruktor
    //!     erwartet ein Zeiger auf ein float Array mit mind. 16 Felder
    //! \param v float Zeiger auf Array von dem die Werte kopiert werden
    DRMatrix(const float* v) {memcpy(n, v, sizeof(float)*16);}


    // Variablen
	union
	{
		struct
		{
        //! DRMatrix als benannte Elemente
			float m00, m04, m08, m12,	// Elemente der DRMatrix
				  m01, m05, m09, m13,
				  m02, m06, m10, m14,
				  m03, m07, m11, m15;
		};
        //! DRMatrix als zwei dimensionales Array/Feld
		float		m[4][4];			// Zweidimensionales Array der Elemente
		//! DRMatrix als ein dimensionales Array/Feld
		float		n[16];				// Eindimensionales Array der Elemente
	};

    // Casting-Opeatoren
    //! \brief casting operator
    //! \return float Zeiger auf DRMatrix Elemente Array
	operator float* ()					{return (float*)(n);}
	//! \brief casting operator, const
    //! \return float Zeiger auf DRMatrix Elemente Array
	operator const float* ()	const	{return (float*)(n);}
	//! \brief Klammer Operator
	//! \param iRow Zeile der DRMatrix [0,3]
	//! \param iColumn Spalte der DRMatrix [0,3]
	//! \return eine referenz auf das gew&uuml;nschte Feld, bei &auml;nderung des Wertes, &auml;nderung in der DRMatrix!
	float& operator () (int iRow, int iColumn) {return m[iRow][iColumn];}
	//! \brief Klammer Operator, const
    //! \param iRow Zeile der DRMatrix [0,3]
	//! \param iColumn Spalte der DRMatrix [0,3]
	//! \return eine Kopie des Wertes im gew&uuml;nschtem Feld, bei &auml;nderung, bleibt DRMatrix wie sie ist
	float operator () (int iRow, int iColumn) const {return m[iRow][iColumn];}

	//! \brief Klammer Operator
	//! \param iRow Zeile der DRMatrix [0,3]
	//! \param iColumn Spalte der DRMatrix [0,3]
	//! \return eine referenz auf das gew&uuml;nschte Feld, bei &auml;nderung des Wertes, &auml;nderung in der DRMatrix!
	float& operator () (int index) {return n[index];}
	//! \brief Klammer Operator, const
    //! \param iRow Zeile der DRMatrix [0,3]
	//! \param iColumn Spalte der DRMatrix [0,3]
	//! \return eine Kopie des Wertes im gew&uuml;nschtem Feld, bei &auml;nderung, bleibt DRMatrix wie sie ist
	float operator () (int index) const {return n[index];}

	//Rechen operationen
	//! \brief Multiplikations Operator, const
    //!     zum erstellen einer neuen DRMatrix als Ergebnis aus dieser DRMatrix * &uuml;bergebende DRMatrix
	//! \param DRMatrix mit welcher Multipliziert werden soll
	//! \return eine neue DRMatrix als Ergebnis aus this * mm
	DRMatrix operator * (const DRMatrix& mm) const
	{
	    DRMatrix e(0.0f);

	    for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                e.m[i][j] = mm.m[0][j] * m[i][0] + 
                	    mm.m[1][j] * m[i][1] + 
                	    mm.m[2][j] * m[i][2] + 
                            mm.m[3][j] * m[i][3];
        return e;
	}
	//! \brief Multiplikations Operator
    //!     zum auf Auf-Multiplizieren einer DRMatrix auf diese DRMatrix
	//! \param DRMatrix mit welcher Multipliziert werden soll
	//! \return diese DRMatrix, da diese die neuen Werte enth&auml;lt
	DRMatrix& operator *= (const DRMatrix& mm) {return *this = *this*mm;}

	// Vergleichsoperatoren
	//! \brief vegleicht 2 Matritzen miteinander, const
	//! \param mm DRMatrix mit der verglichen werden soll
	//! \return true wenn beide Matritzen, d.h. alle Felder den gleichen Wert haben
	//! \return false wenn auch nur ein Wert abweicht
    bool operator == (const DRMatrix& mm) const
    {
        for(int i = 0; i < 16; i++)
            if(n[i] != mm.n[i]) return false;

        return true;
    }

    //! \brief vergleicht 2 Matritzen auf ungleichheit miteinander, const
    //! \param mm DRMatrix mit der verglichen werden soll
    //! \return true wenn eines der Felder ungleich dem Feld der anderen DRMatrix ist
    //! \return false wenn alle Felder gleich sind
    bool operator != (const DRMatrix& mm) const
    {
        for(int i = 0; i < 16; i++)
            if(n[i] != mm.n[i]) return true;

        return false;
    }

    // Zuweisungsoperator
    //! \brief damit werden alle Werte der rechtsstehenden DRMatrix dieser DRMatrix hinzugef&uuml;gt
    //! \param mm DRMatrix deren Werte kopiert werden
    //! \return eine Referenz auf diese DRMatrix, die jetzt die Werte der anderen DRMatrix enth&auml;lt
    DRMatrix& operator = (const DRMatrix& mm) {memcpy(n, mm.n, sizeof(float)*16); return *this;}

    // tools
    //! \brief zum ausgeben der aktuellen DRMatrix auf die Konsole
    //! mit 3 Nachkommastellen, in tabellenforn
    //! \param name name wird, falls &uuml;bergeben, &uuml;ber der DRMatrix angezeigt
    void print(const char* name = NULL) const
	{
	    if(name) printf("DRMatrix %s: \n", name);
        else printf("DRMatrix: \n");
	    for(int i = 0; i < 4; i++)
	    {
            for(int j = 0; j < 4; j++)
                printf("%.3f ", m[i][j]);
            printf("\n");
	    }
	    printf("..end..\n");
	}

    // Funktionen deklarieren
    //! \brief liefert die Identit&auml;tsmatrix, static
    //!     (Multiplaikation mit ihr ergibt die DRMatrix die mit ihr Multipliziert wurde)
    //! \return eine neue DRMatrix, gef&uuml;llt mit nullen und 1<br>
    //! <p align="center"> 1 0 0 0 <br>
    //!                    0 1 0 0 <br>
    //!                    0 0 1 0 <br>
    //!                    0 0 0 1 </p>
    inline static DRMatrix	identity() {return DRMatrix(1.0f, 0.0f, 0.0f, 0.0f,
    												  0.0f, 1.0f, 0.0f, 0.0f,
    												  0.0f, 0.0f, 1.0f, 0.0f, 
    												  0.0f, 0.0f, 0.0f, 1.0f);}	// Identit&auml;tsmatrix liefern
    //! \brief liefert die Translationsmatrix zu einem Vektor, static
    //! \param v translations Vektor
    //! \return eine neue Translationsmatrix <br>
    //! <p align="center"> 1   0   0   0 <br>
    //!                    0   1   0   0 <br>
    //!                    0   0   1   0 <br>
    //!                    v.x v.y v.z 1 </p>
    static DRMatrix	translation(const DRVector3& v); 	// Translationsmatrix (Verschiebungsmatrix) berechnen
    //! \brief liefert die Achsen DRMatrix aus drei Achsen, static
    //! \param x_axis x Achse
    //! \param y_axis y Achse
    //! \param z_axis z Achse
    //! \return eine neue DRMatrix <br>
    //! <p align="center"> x_axis.x y_axis.x z_axis.x 0 <br>
    //!                    x_axis.y y_axis.y z_axis.y 0 <br>
    //!                    x_axis.z y_axis.z z_axis.z 0 <br>
    //!                    0        0        0        1 </p>
    static DRMatrix   axis(const DRVector3& x_axis, const DRVector3& y_axis, const DRVector3& z_axis);
    //! \brief liefert die Rotationsmatrix um die x Achse, static
    //! \param f rotation in Grad um die x-Achse
    //! \return eine neue DRMatrix <br>
    //! <p align="center"> 1  0      0      0 <br>
    //!                    0  cos(f) sin(f) 0 <br>
    //!                    0 -sin(f) cos(f) 0 <br>
    //!                    0  0      0      1 </p>
    static DRMatrix	rotationX(const float f);       // Rotationsmatrix um die X-Achse berechnen
    //! \brief liefert die Rotationsmatrix um die y Achse, static
    //! \param f rotation in Grad um die y-Achse
    //! \return eine neue DRMatrix <br>
    //! <p align="center"> cos(f) 0 -sin(f) 0 <br>
    //!                    0      1  0      0 <br>
    //!                    sin(f) 0  cos(f) 0 <br>
    //!                    0      0  0      1 </p>
    static DRMatrix	rotationY(const float f);       // Rotationsmatrix um die Y-Achse berechnen
    //! \brief liefert die Rotationsmatrix um die z Achse, static
    //! \param f rotation in Grad um die z-Achse
    //! \return eine neue DRMatrix <br>
    //! <p align="center"> cos(f) sin(f) 0 0 <br>
    //!                    0      1      0 0 <br>
    //!                    sin(f) cos(f) 1 0 <br>
    //!                    0      0      0 1 </p>
    static DRMatrix	rotationZ(const float f);       // Rotationsmatrix um die Z-Achse berechnen
    //! \brief liefert die Rotationsmatrix um alle 3 Achsen, static
    //! \param v rotationen in Grad um die jeweilge Achse
    //! \return eine neue DRMatrix
    //!  rotationZ(v.z) * rotationY(v.y) * rotationX(v.x)
    static DRMatrix	rotation(const DRVector3& v);   // Rotiert um alle drei Achsen
    //! \brief liefert die Rotationsmatrix um eine beliebige Achse, static
    //! \param v die beliebige Achse
    //! \param f rotation in Grad um diese Achse
    //! \return eine neue DRMatrix Rotationsmatrix
    static DRMatrix	rotationAxis(const DRVector3& v, const float f); // Rotationsmatrix um eine beliebige Achse berechnen
    //! \brief liefert die Skalierungs DRMatrix, static
    //! \param v die Skalierungsfaktoren auf den 3 Achsen
    //! \return eine neue Skalierungsmatrix <br>
    //! <p align="center"> v.x 0   0   0 <br>
    //!                    0   v.y 0   0 <br>
    //!                    0   0   v.z 0 <br>
    //!                    0   0   0   1 </p>
    static DRMatrix	scaling(const DRVector3& v);		// Skalierungsmatrix berechnen

    static DRMatrix perspective_projection(const float angle_of_view, const float aspect_ratio, const float z_near, const float z_far);
    static DRMatrix ortho_projection(const float left, const float right, const float bottom, const float top,
                                     const float zNear, const float zFar);

    //! \brief liefert eine transponierte DRMatrix zur&uuml;ck, const
    //! \return eine neue DRMatrix, diese transponiert
    DRMatrix transpose() const;
    //! \brief liefert eine invertierte DRMatrix, const
    //!
    //! didn't work by projection matrices!
    //! \return eine neue DRMatrix, diese invertiert
    DRMatrix invert() const;
    //! \brief liefert die determinante dieser DRMatrix, const
    //! \return die determinante dieser DRMatrix als float (z.B. zum berechnen der invertierten DRMatrix)
    float det() const;
};

#endif //__DR_CORE2_MATRIX__
