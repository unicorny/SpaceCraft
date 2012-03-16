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
 * File:   DRObject.h
 * Author: dario
 *
 * Created on 20. August 2011, 21:01
 */

#ifndef __DR_CORE2_OBJECT__
#define	__DR_CORE2_OBJECT__

/*! \brief Diese Klasse representiert ein DRObjekt

    Zum relativen Bewegen von Objekten (wie z.B. Kamera)
    Die Struktur der Klasse habe ich &uuml;bernommen von <a href="http://www.spieleprogrammierer.de/" target="_blank">David Scherfgens</a>
    tbObjekt Klasse aus seiner TriBase Engine
    \author Dario Rekowski
    \date 07.12.09
*/
class CORE2_API DRObjekt
{
public:
    //! Konstruktor welcher die Startposition und Startrotation erwartet
    //! \brief Standardkonstruktor
    //! \param position die Ausgangsposition des Objektes
    //! \param rotation die Ausgangsrotation des Objektes
    DRObjekt(const DRVector3& position);

    //! Konstruktor welcher ohne Parameter auskommt
    //! \brief Standardkonstruktor, setzte alles auf Null
    DRObjekt();

    //! in Verh&auml;ltniss zum Globalen Koordinatensystem
    //! \brief bewegt das DRObjekt absolut
    //! \param translate die Bewegung des Objektes auf allen drei Achsen
    void translateAbs(const DRVector3& translate);
    //! in Verh&auml;ltniss zum Objekteigenem Koordinatensystem
    //! \brief bewegt das DRObjekt relativ
    //! \param translate die Bewegung des Objektes auf allen drei Achsen
    void translateRel(const DRVector3& translate);
    //! in Verh&auml;ltniss zum Objekteigenem Koordinatensystem
    //! \brief rotiert das DRObjekt relativ
    //! \param rotate die Rotation in Grad um die 3 Achsen
    void rotateRel(const DRVector3& rotate);
    //! in Verh&auml;ltniss zum Zentralem Koordinatensystem
    //! \brief rotiert das DRObjekt absolut
    //! \param rotate die Rotation in Grad um die 3 Achsen
    void rotateAbs(const DRVector3& rotate);
    
    //! \brief liefert die Matrix des Objektes
    //! \return eine konstante Referenz auf die DRObjekt matrix,
    //!     es knnen nur als const deklarierte Funktionen von Matrix dran aufgerufen werden
    const DRMatrix& getMatrix() {return mMatrix;}
    //! \brief liefert die Position des Objektes
    //! \return eine konstante Referenz auf die DRObjekt position,
    //!     es knnen nur als const deklarierte Funktionen von Vector3 dran aufgerufen werden
    const DRVector3& getPosition() const {return mPosition;}
    void setPosition(DRVector3 newPosition) {mPosition = newPosition;}
    
    void lookAt(DRVector3 targetPosition, DRVector3 upVector = DRVector3(0.0f, 1.0f, 0.0f));
    
    const DRVector3& getYAxis() const {return mYAxis;}
    const DRVector3& getXAxis() const {return mXAxis;}
    const DRVector3& getZAxis() const {return mZAxis;}
    
    void setAxis(DRVector3 XAxis, DRVector3 YAxis, DRVector3 ZAxis)
    {
        mXAxis = XAxis;
        mYAxis = YAxis;
        mZAxis = ZAxis;
        update();
    }

protected:
    //! berechnet die Matrix neu
    virtual void update();

    DRVector3 mPosition;

    DRVector3 mXAxis;
    DRVector3 mYAxis;
    DRVector3 mZAxis;

    DRMatrix mMatrix;
};


#endif	/* __DR_CORE2_OBJECT__ */

