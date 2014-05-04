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

#ifndef __DR_ENGINE2_ENGINE_LOGGER__
#define __DR_ENGINE2_ENGINE_LOGGER__

/**!
 * class for saving texture to filesystem
 */

class ENGINE2_API DREngineLogger : public DRLogger
{
public:
    DREngineLogger();
    virtual ~DREngineLogger();
    
    virtual DRReturn init(const char* pcFilename, bool printToConsole);
    virtual void exit();

    //in die Log-Datei schreiben
    virtual DRReturn writeToLogDirect(DRString text);
    virtual DRReturn writeToLogDirect(const char* pcText, ...);
    virtual DRReturn writeToLog(const char* pcText, ...);
    virtual DRReturn writeToLog(DRString text);
protected:
    SDL_mutex*  mMutex;
        
private:
    
};

#endif //__DR_ENGINE2_ENGINE_LOGGER__