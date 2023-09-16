/*  Copyright (C) 2023  Dmitrii Chernikov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef NOTAGAME_GAME_FIELD_UI_H
#define NOTAGAME_GAME_FIELD_UI_H

#include "GenericUI.h"
#include "Camera2D.h"
#include "UnitMap.h"
#include "FurnitureGeneric.h"
#include "define.h"
#include "utility.h"

#include "ofAppRunner.h" //for ofGetWindowWidth()
#include "ofGraphics.h" //for ofBackgroundGradient()

#include <string>
#include <deque>

class GameFieldUI: public GenericUI {
public:
    GameFieldUI();
	void ClearLog();
	template<typename F> void Draw(const UnitMap&, const F&, 
								   const Camera2D&) const;
    void DrawLog() const;
	void Initialise();
    void AddToLog(const std::string&);
private:
	int CalculateArrowSpriteOffsetY(const UnitMap&, 
									csptr_StandingGeneric) const;
	pair_size_t CalculateArrowDrawOffsets(enumDirection, bool) const;
	void DrawArrow(csptr_StandingGeneric, float, float, int) const;
	void DrawHP(csptr_StandingGeneric, float, float) const;
	const DrawableGeneric& GetLogWindow() const;
	DrawableGeneric& GetLogWindow();
	const DrawableGeneric& GetFreeRoamIndicator() const;
	DrawableGeneric& GetFreeRoamIndicator();
	const ofColor& GetColourBackgroundTop() const;
	const ofColor& GetColourBackgroundBottom() const;
	const ofColor& GetColourLogWindow() const;
	std::shared_ptr<const ofImage> GetSpriteArrows() const;
	std::shared_ptr<const ofImage> GetSpriteHP() const;
	const std::deque<std::string>& GetLog() const;
	std::deque<std::string>& GetLog();

    DrawableGeneric m_oLogWindow;
    DrawableGeneric m_oFreeRoamIndicator;
	const std::shared_ptr<const ofImage> m_poSpriteArrows;
    const std::shared_ptr<const ofImage> m_poSpriteHP;
    std::deque<std::string> m_qLog;
	const ofColor m_oColourBackgroundTop;
    const ofColor m_oColourBackgroundBottom;
	const ofColor m_oColourLogWindow;
};

template<typename F>
void GameFieldUI::Draw(const UnitMap& oUnitMap, const F& LambdaDraw, 
					   const Camera2D& oCamera) const {
	const std::unordered_map<pair_size_t, sptr_StandingGeneric>& mpoUnits(oUnitMap.GetAll());
	const float fCameraOffsetX(oCamera.GetOffsetX());
	const float fCameraOffsetY(oCamera.GetOffsetY());
	const size_t unVisibleTiles(oCamera.CountVisibleExistingTiles());

	ofPushStyle();
	ofBackgroundGradient(GetColourBackgroundTop(), GetColourBackgroundBottom(), 
						 OF_GRADIENT_LINEAR);
	ofPopStyle();
	LambdaDraw(oCamera);
	if (unVisibleTiles < mpoUnits.size()) { //iterate over coordinates
		for (size_t unY(oCamera.GetTopLeftExistingY()); 
			 unY <= oCamera.GetBottomRightExistingY(); ++unY)
			for (size_t unX(oCamera.GetTopLeftExistingX()); 
				 unX <= oCamera.GetBottomRightExistingX(); ++unX) {
				const csptr_StandingGeneric poStanding(oUnitMap.Get(unX, unY));

				if (!poStanding || !poStanding->IsDiscovered())
					continue;
				DrawArrow(poStanding, fCameraOffsetX, fCameraOffsetY, 
						  CalculateArrowSpriteOffsetY(oUnitMap, poStanding));
				DrawHP(poStanding, fCameraOffsetX, fCameraOffsetY);
			}
	}
	else //easier to iterate over all units
		for (const std::pair<pair_size_t, csptr_StandingGeneric>& KeyValue : mpoUnits) {
			const csptr_StandingGeneric poStanding(KeyValue.second);

			if (poStanding->IsDiscovered() && 
				oCamera.IsInView(poStanding->GetCoordinates())) {
				DrawArrow(poStanding, fCameraOffsetX, fCameraOffsetY, 
						  CalculateArrowSpriteOffsetY(oUnitMap, poStanding));
				DrawHP(poStanding, fCameraOffsetX, fCameraOffsetY);
			}
		}
    DrawLog();
    if (oCamera.IsFreeRoamMode())
        GetFreeRoamIndicator().Draw();
}

inline void GameFieldUI::ClearLog() {
	GetLog().clear();
}

inline const std::deque<std::string>& GameFieldUI::GetLog() const {
	return m_qLog;
}

inline std::deque<std::string>& GameFieldUI::GetLog() {
	return m_qLog;
}

inline const DrawableGeneric& GameFieldUI::GetLogWindow() const {
	return m_oLogWindow;
}

inline DrawableGeneric& GameFieldUI::GetLogWindow() {
	return m_oLogWindow;
}

inline const DrawableGeneric& GameFieldUI::GetFreeRoamIndicator() const {
	return m_oFreeRoamIndicator;
}

inline DrawableGeneric& GameFieldUI::GetFreeRoamIndicator() {
	return m_oFreeRoamIndicator;
}

inline const ofColor& GameFieldUI::GetColourBackgroundTop() const {
	return m_oColourBackgroundTop;
}

inline const ofColor& GameFieldUI::GetColourBackgroundBottom() const {
	return m_oColourBackgroundBottom;
}

inline const ofColor& GameFieldUI::GetColourLogWindow() const {
	return m_oColourLogWindow;
}

inline std::shared_ptr<const ofImage> GameFieldUI::GetSpriteArrows() const {
	return m_poSpriteArrows;
}

inline std::shared_ptr<const ofImage> GameFieldUI::GetSpriteHP() const {
	return m_poSpriteHP;
}

#endif