/*/*************************************************************************
 *                                                                       *
 * Core, Core-Lib for my programs, Core doesn't need any libraries	 *
 * Copyright (C) 2007, 2008, 2009, 2010, 2011 Dario Rekowski.		 *
 * Email: dario.rekowski@gmx.de   Web: www.einhornimmond.de              *
 *                                                                       *
 * This program is free software: you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation, either version 3 of the License, or     *
 * any later version.													 *
 *																		 *
 * This program is distributed in the hope that it will be useful,		 *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of		 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		 *
 * GNU General Public License for more details.							 *
 *																		 *
 * You should have received a copy of the GNU General Public License	 *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                       *
 *************************************************************************/


#ifndef __DR_DUMPING_SPRING__
#define __DR_DUMPING_SPRING__

/*! \brief class for a dumping spring system
 *
 *  Ursprungsidee kommt aus dem Buch Gems4.
 *  Eine Variable läuft auf ihren Ziel Wert zu,
 *  durch eine Feder geführt
 * 
 *  @author Dario Rekowski
 *  @date 22.11.10
 */

class CORE2_API DRDampingSpring
{
public:
        //! \brief constructor with parameters
        //! \param damping damping constant
        //!        for short moving time: damping = 2*WURZEL(spring)
        //! \param spring spring constant
        //!		    for short moving time: spring = (dumping/2)²
        DRDampingSpring(DRReal damping, DRReal spring);

        //! constructor without parameters (both set to 1)
        DRDampingSpring();

        //! \brief to set both constants
        //! \param damping damping constant
        //!        for short moving time: damping = 2*WURZEL(spring)
        //! \param spring spring constant
        //!		    for short moving time: spring = (dumping/2)²
        void setParameter(DRReal damping, DRReal spring);

        //! \brief to set manually the current value
        inline void setCurrent(DRReal current) {mCurrent = current;}

        //! \brief update the current value (returned by operator DRReal())
        //! \param timeSinceLastFrame times in second, since last frame
        void update(DRReal timeSinceLastFrame);

        //! \brief operator DRReal()
        //! \return current value
        inline operator DRReal() const	{return mCurrent;}

        //! \brief assign operator
        //! \param target intern mTarget will be set to target
        inline void operator = (DRReal target) {mTarget = target;}

        inline void operator += (DRReal additional) {mCurrent += additional;}

        //! \return intern mTarget
        inline const DRReal& getTarget() const {return mTarget;}

        inline const DRReal& getVelocity() const {return mVelocity;}

private:
        // damping constant
        DRReal  mDamping;
        // spring constant
        DRReal  mSpring;
        // target value (to reach)
        DRReal			mTarget;
        // current value (return for operator DRReal())
        DRReal			mCurrent;
        // from struct use to calculate
        DRReal			mVelocity;

};

/*   -- S A M P L E --  *\

  DampingSpring value(damping, spring);
  or
  DampingSpring value;
  value.setParameter(damping, spring)

  value = target;
  value.update(timeSinceLastFrame)
  DRReal current = value;
  */

/*
class CORE_API DRSpring
{
public:
	DRSpring();
        
	void setVar(DRReal value, DRReal time);
	void update(DRReal deltaTime);
        
	operator DRReal() const {return mVar;}
	DRReal operator = (DRReal value);

	bool hasReachedTarget() const;
private:
	DRReal mVar;		// current value
	DRReal mTarget;		// target value
	DRReal mDelta;		// difference between begin and end value
	DRReal mTime;		// current Time
	DRReal mTimeTarget;	// target time
};
//*/

#endif //__DR_DUMPING_SPRING__