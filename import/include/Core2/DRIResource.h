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

/**!
 * @Author: Dario Rekowski
 * 
 * @Date: 16.03.2012
 * 
 * this class is based on
 * Noel Llopis,
 * Day 1 Studios,
 * llopis@convexhull.com
 * article in Gems4 1.8, about Weak references and zero-objects
 * 
 * 
 */
#ifndef __DR_CORE2_IRESOURCE__
#define __DR_CORE2_IRESOURCE__

class CORE2_API DRIResource
{
public:
    virtual ~DRIResource() {}
    virtual const char* getResourceType() const = 0;
    virtual bool less_than(DRIResource& b) const = 0;
protected:
};

#endif //__DR_CORE2_IRESOURCE__