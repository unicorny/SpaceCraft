/*/*************************************************************************
 *                                                                         *
 * EngineDLL, Engine for my programs, using SDL and OpenGL		   *
 * Copyright (C) 2012, 2013, 2014 Dario Rekowski.			   *
 * Email: dario.rekowski@gmx.de   Web: www.einhornimmond.de                *
 *                                                                         *
 * This program is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * any later version.							   *
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

/**
 * @Author Dario Rekowski
 * 
 * @Date 13.08.12
 * 
 * @Desc Class for easy handling threading
 */
 
#ifndef __DR_ENGINE2_THREAD__
#define __DR_ENGINE2_THREAD__

class ENGINE2_API DRThread
{
public:
    //! \param threadName used up SDL 1.3, for BeOS max. 32, for Linux max 16, for Visual Studio 6.0 max 9 char
    DRThread(const char* threadName = NULL);
    virtual ~DRThread();
        
    __inline__ DRReturn lock() {if(SDL_LockMutex(mutex)) LOG_ERROR_SDL(DR_ERROR); return DR_OK;}
    __inline__ DRReturn unlock() {if(SDL_UnlockMutex(mutex)) LOG_ERROR_SDL(DR_ERROR); return DR_OK;} 
    // signal data chance, will continue thread, if he is paused
    DRReturn condSignal();
    
    //! \brief will be called every time from thread, when condSignal was called
    //! will be called from thread with locked working mutex,<br>
    //! mutex will be unlock after calling this function
    //! \return if return isn't 0, thread will exit
    virtual int ThreadFunction() = 0;
        
protected:
    
    static int Thread(void* data);
    
    SDL_mutex*		   mutex;
    SDL_Thread*		   thread;
    SDL_cond*		   condition;
    SDL_sem*		   semaphore;
    bool               exitCalled;
};

#endif //__DR_ENGINE2_THREAD__
