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
 * File:   DRIndexReferenzHolder.h
 * Author: Dario
 *
 * Created on 8. November 2011, 11:56
 */

#ifndef __DR_CORE2_INDEX_REFERENZ_HOLDER__
#define	__DR_CORE2_INDEX_REFERENZ_HOLDER__


class CORE2_API DRIndexReferenzHolder
{
public:
    DRIndexReferenzHolder(uint maxIndexCount);
    ~DRIndexReferenzHolder();
    
    void add(uint index);
    
    void remove(uint index);
    
    uint getFree();
    
private:
    
    uint*       mReferenzCounter;
    uint*       mFreePlaces;  
    uint        mFreePlaceCursor;
    uint        mMaxIndexCount;
    
};


#endif	/* __DR_CORE2_INDEX_REFERENZ_HOLDER__ */

