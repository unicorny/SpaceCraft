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
 * File:   DRGameStateManager.h
 * Author: Dario
 *
 * Created on 09. Juni 2012, 08:26
 */

#ifndef __DR_CORE2_GAME_STATE_MANAGER__
#define	__DR_CORE2_GAME_STATE_MANAGER__

class CORE2_API DRGameState: public DRIResource
{
public:
    //! \param transparency 1 is not transparent, < 1 is transparent
    DRGameState(float transparency = 1.0f)
    : mTransparency(transparency), mFirstState(false) {};
    virtual ~DRGameState() {};

    virtual DRReturn render(float fTime) = 0;
    virtual DRReturn move(float fTime) = 0;

    inline void setTransparency(float transparency) {mTransparency = transparency;}
    inline float getTransparency() const {return mTransparency;}

    inline bool isFirstState() {return mFirstState;}
    inline void setFirstState(bool firstState) {mFirstState = firstState;}


protected:
    float       mTransparency;
    bool        mFirstState;//the state who call glClear
};
typedef DRResourcePtr<DRGameState> DRGameStatePtr;

class CORE2_API DRGameStateManager
{
public:
    DRGameStateManager();
    ~DRGameStateManager();

    void addConstantState(DRGameStatePtr newGameState, const char* name);

    //! \return DR_ERROR if state with this name didn't exist in constant states map
    DRReturn pushState(const char* name);
    void pushState(DRGameStatePtr gameState);
    void popState();

    int getStateCount() const {return mGameStates.size();}
    int getConstantStateCount() const {return mConstantGameStates.size();}
    
    DRReturn move(float fTime);
    DRReturn render(float fTime);

private:
    std::map<DHASH, DRGameStatePtr> mConstantGameStates;
    typedef std::pair<DHASH, DRGameStatePtr> CONSTANT_GAME_STATE_ENTRY;

    std::list<DRGameStatePtr> mGameStates;

};

#endif //__DR_CORE2_GAME_STATE_MANAGER__
