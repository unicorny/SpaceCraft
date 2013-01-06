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
#ifndef __DR_CORE2_RESOURCE_PTR__
#define __DR_CORE2_RESOURCE_PTR__

#include <cstddef>

template <class ResourceType>
class DRSimpleResourcePtr
{
public:
    DRSimpleResourcePtr(const DRSimpleResourcePtr<ResourceType> & resPtr);
    explicit DRSimpleResourcePtr(ResourceType * pRes);
    ~DRSimpleResourcePtr();

    DRSimpleResourcePtr<ResourceType> & operator= (ResourceType * resPtr);

    ResourceType * operator->() const { return mResource; }
    ResourceType & operator*() const { return *mResource; }
    operator ResourceType*() {return mResource;} 

    int getRef() {return mRefCounter;}
    void addRef() {mRefCounter++;}
    void removeRef() {mRefCounter--;}

private:
    ResourceType*   mResource;
    int             mRefCounter;
};
template <class ResourceType>
DRSimpleResourcePtr<ResourceType>::DRSimpleResourcePtr(const DRSimpleResourcePtr<ResourceType> & resPtr)
: mRefCounter(1)
{
    mResource = resPtr.mResource;
}


template <class ResourceType>
DRSimpleResourcePtr<ResourceType>::DRSimpleResourcePtr(ResourceType * pRes)
: mRefCounter(1)
{
    mResource = pRes;
}

template <class ResourceType>
DRSimpleResourcePtr<ResourceType>::~DRSimpleResourcePtr()
{
    mResource = NULL;
}

template <class ResourceType>
DRSimpleResourcePtr<ResourceType> & DRSimpleResourcePtr<ResourceType>::operator= (ResourceType * resPtr)
{
    mResource = resPtr;
    return *this;
}


template <class ResourceType>
class DRResourcePtr
{
public:
    DRResourcePtr(const DRResourcePtr<ResourceType> & resPtr);
    explicit DRResourcePtr(ResourceType * pRes);
    DRResourcePtr(DRResourcePtrHolder * pHolder = NULL);
    ~DRResourcePtr();

    DRResourcePtr<ResourceType> & operator= (const DRResourcePtr<ResourceType> & resPtr);

    ResourceType * operator->() const { return static_cast<ResourceType *>(mResourceHolder->mResource); }
    ResourceType & operator*() const { return *(static_cast<ResourceType *>(mResourceHolder->mResource)); }
    operator ResourceType*() {return mResourceHolder ? static_cast<ResourceType *>(mResourceHolder->mResource) : NULL;} 

    bool operator==(DRResourcePtr<ResourceType> res) const;
    bool operator!=(DRResourcePtr<ResourceType> res) const { return !operator==(res); }
    bool operator< (DRResourcePtr<ResourceType> res) const;
    
    void release();

    DRResourcePtrHolder * getResourcePtrHolder() const { return mResourceHolder; }
    typedef ResourceType * PointerType;

private:
    DRResourcePtrHolder * mResourceHolder;
};


template <class ResourceType>
DRResourcePtr<ResourceType>::DRResourcePtr(const DRResourcePtr<ResourceType> & resPtr)
{
    mResourceHolder = resPtr.getResourcePtrHolder();
    if (mResourceHolder != NULL)
        mResourceHolder->addRef();    
}


template <class ResourceType>
DRResourcePtr<ResourceType>::DRResourcePtr(ResourceType * pRes)
{
    mResourceHolder = new DRResourcePtrHolder(pRes); 
    mResourceHolder->addRef();    
}

template <class ResourceType>
DRResourcePtr<ResourceType>::DRResourcePtr(DRResourcePtrHolder * pHolder) : 
    mResourceHolder (pHolder)
{
    if (mResourceHolder != NULL)
        mResourceHolder->addRef();    
}


template <class ResourceType>
DRResourcePtr<ResourceType>::~DRResourcePtr()
{
    release();
}

template <class ResourceType>
void DRResourcePtr<ResourceType>::release()
{
    if (mResourceHolder != NULL)
        mResourceHolder->release(); 
    mResourceHolder = NULL;
}

template <class ResourceType>
DRResourcePtr<ResourceType> & DRResourcePtr<ResourceType>::operator= (const DRResourcePtr<ResourceType> & resPtr)
{
    if (mResourceHolder != resPtr.getResourcePtrHolder())
    {
        if (mResourceHolder != NULL)
            mResourceHolder->release(); 
        mResourceHolder = resPtr.getResourcePtrHolder();
        if (mResourceHolder != NULL)
            mResourceHolder->addRef();    
    }
    return *this;
}

template <class ResourceType>
bool DRResourcePtr<ResourceType>::operator==(DRResourcePtr<ResourceType> res) const
{
    if (mResourceHolder == res.mResourceHolder)
        return true;
    // At this point, both holders cannot be NULL
    if (res.mResourceHolder == NULL && mResourceHolder->mResource == NULL)
        return true;
    if (mResourceHolder == NULL && res.mResourceHolder->mResource == NULL)
        return true;
    if (mResourceHolder != NULL && res.mResourceHolder != NULL &&
        mResourceHolder->mResource == res.mResourceHolder->mResource)
        return true;
    return false;
}

template <class ResourceType>
bool DRResourcePtr<ResourceType>::operator< (DRResourcePtr<ResourceType> res) const
{
    if (res.mResourceHolder == NULL)
        return false;
    if (mResourceHolder == NULL)
        return true;
    return (mResourceHolder->mResource->less_than(*res.mResourceHolder->mResource));
}


#endif //__DR_CORE2_RESOURCE_PTR__