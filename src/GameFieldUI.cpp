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

#include "GameFieldUI.h"

GameFieldUI::GameFieldUI():
	m_oLogWindow("ui_log_window.png"),
	m_oFreeRoamIndicator("ui_freeroam_indicator.png"),
	m_poSpriteArrows(std::make_shared<const ofImage>(GetSpriteFullPath("ui_arrows.png"))),
	m_poSpriteHP(std::make_shared<const ofImage>(GetSpriteFullPath("ui_hp.png"))),
	m_qLog(),
    m_oColourBackgroundTop(51, 102, 153), 
	m_oColourBackgroundBottom(122, 186, 122),
	m_oColourLogWindow(245, 225, 175)
{
	GetFreeRoamIndicator().LoadSprite();
	GetLogWindow().LoadSprite();
	Initialise();
}

void GameFieldUI::Initialise() {
	if (!IsInitialisationNeeded())
		return;

	DrawableGeneric& oFreeRoam(GetFreeRoamIndicator());
	DrawableGeneric& oLogWindow(GetLogWindow());
	const std::shared_ptr<const ofImage> poSpriteRoam(oFreeRoam.GetSprite());
	const int nWindowWidth(ofGetWindowWidth());
	const int nWindowHeight(ofGetWindowHeight());
	const float fFreeRoamX(nWindowWidth * 0.5 - 
						   poSpriteRoam->getWidth() * 0.5);
	const float fFreeRoamY(nWindowHeight * 0.5 - 
						   poSpriteRoam->getHeight() * 0.5);
	const float fLogWindowHeight(oLogWindow.GetSprite()->getHeight());
	float fTopLeftY(nWindowHeight - fLogWindowHeight - 10);

	oFreeRoam.SetWindowCoordinates(fFreeRoamX, fFreeRoamY);
	oLogWindow.SetWindowCoordinates(10, fTopLeftY);
	SetInitialisationNeeded(false);
}

void GameFieldUI::DrawLog() const {
	const DrawableGeneric& oLogWindow(GetLogWindow());
    const float fTopLeftY(oLogWindow.GetWindowY());
	const std::deque<std::string>& qLog(GetLog());

	ofPushStyle();
    ofSetColor(GetColourLogWindow());
    oLogWindow.Draw();
	ofSetColor(0, 0, 0);
    for (size_t iii(0); iii < qLog.size(); ++iii) //text lines
        ofDrawBitmapString(qLog.at(iii), 20, fTopLeftY + 14 + iii * 13);
	ofPopStyle();
}

void GameFieldUI::AddToLog(const std::string& strLogLine) {
	std::deque<std::string>& qLog(GetLog());
	size_t unStartIndex(0);
	
    while (unStartIndex != std::string::npos) {
		const size_t unNewlineIndex(strLogLine.find('\n', unStartIndex));
		const size_t unSubstringLength(unNewlineIndex - unStartIndex);
		const std::string strSub(strLogLine.substr(unStartIndex,
												   unSubstringLength));
		
		if (strSub.size()) {
			if (qLog.size() == LOG_ENTRIES) //the log is full
				qLog.pop_front(); //removes the oldest entry
			qLog.push_back(strSub);
		}
		unStartIndex = unNewlineIndex;
		if (unStartIndex != std::string::npos)
		    ++unStartIndex;
    }
}

//calculates offsets inside of a SPRITE_W by SPRITE_H tile
pair_size_t GameFieldUI::CalculateArrowDrawOffsets(const enumDirection eMain,
												   const bool bDamaged) const {
	const size_t unArrowWidth(8); //in pixels; equals its height

    switch (eMain) {
        case DIR_NORTH: //unTileOffsetY stays 0
			return std::make_pair(SPRITE_W / 2 - unArrowWidth / 2, 0);
		case DIR_NE: //offset Y stays 0
			return std::make_pair(SPRITE_W - unArrowWidth, 0);
        case DIR_EAST: 
			return std::make_pair(SPRITE_W - unArrowWidth, 
								  SPRITE_H / 2 - unArrowWidth / 2);
        case DIR_SE:  
			return std::make_pair(SPRITE_W - unArrowWidth, 
								  SPRITE_H - unArrowWidth);
        case DIR_SOUTH: {
			const size_t unOffsetY(!bDamaged ? SPRITE_H - unArrowWidth :
								   SPRITE_H - unArrowWidth - 4); //above HP

			return std::make_pair(SPRITE_W / 2 - unArrowWidth / 2, unOffsetY); 
		}
        case DIR_SW: //offset X stays 0
			return std::make_pair(0, SPRITE_H - unArrowWidth);
        case DIR_WEST: //offset X stays 0
			return std::make_pair(0, SPRITE_H / 2 - unArrowWidth / 2); 
		case DIR_NW:
			return std::make_pair(0, 0);
		default:
			throw std::runtime_error("GameFieldUI::CalculateArrowDrawOffsets()"
									 ": an invalid direction.");
	}
}

//returns Y in pixels (int)
int GameFieldUI::CalculateArrowSpriteOffsetY(const UnitMap& oUnitMap,
											 const csptr_StandingGeneric poStanding) const {
	if (!oUnitMap.IsPlayer(poStanding))
		return 0; //the 1st row of arrow sprites

	const pair_size_t InFrontXY(poStanding->GetCoordinatesInFront());
	const csptr_StandingGeneric poInFront(oUnitMap.Get(InFrontXY));

    if (!poInFront) //there is no unit in front of the player
		return 0; //the 1st row of arrow sprites

	const int nArrowWidth(8); //in pixels; equals its height

	if (poInFront->IsLiving())
		return nArrowWidth; //the 2nd row of arrow sprites
	if (std::static_pointer_cast<const FurnitureGeneric>(poInFront)->IsContainer())
		return 2 * nArrowWidth; //the 3rd row of arrow sprites
	return 0;
}

void GameFieldUI::DrawArrow(const csptr_StandingGeneric poStanding, 
							const float fCameraOffsetX, 
							const float fCameraOffsetY,
							const int nSpriteOffsetY) const {
	const enumDirection eDirection(poStanding->GetPerceivedDirection());

	if (eDirection == DIR_NONE)
		return;

	const size_t unArrowWidth(8); //in pixels; equals its height
	const bool bPerceivedDamaged(poStanding->IsPerceivedAsDamaged());
	const pair_size_t DrawOffsets(CalculateArrowDrawOffsets(eDirection, 
															bPerceivedDamaged));
	const float fX(fCameraOffsetX + poStanding->GetWindowX() + 
				   DrawOffsets.first);
	const float fY(fCameraOffsetY + poStanding->GetWindowY() + 
				   DrawOffsets.second);
	const size_t unSpriteOffsetX(eDirection * unArrowWidth);

    GetSpriteArrows()->drawSubsection(fX, fY, unArrowWidth, unArrowWidth, 
									  unSpriteOffsetX, nSpriteOffsetY);
}

void GameFieldUI::DrawHP(const csptr_StandingGeneric poStanding, 
						 const float fCameraOffsetX, 
						 const float fCameraOffsetY) const {
	size_t unPerceivedHP(poStanding->GetPerceivedHPCurrent());
	size_t unPerceivedHPMax(poStanding->GetPerceivedHPMaximal());

	if (unPerceivedHP == unPerceivedHPMax)
		return;

    const float fHPFraction(static_cast<float>(unPerceivedHP) / 
							unPerceivedHPMax);
    const size_t unX(poStanding->GetX() * SPRITE_W + 7); 
	const size_t unY(poStanding->GetY() * SPRITE_H + 44);
	const std::shared_ptr<const ofImage> poSpriteHP(GetSpriteHP());
	const float fInnerBarWidth(poSpriteHP->getWidth() - 2); //border is 1px
	const float fInnerBarHeight(poSpriteHP->getHeight() - 2); //border is 1px
    const float fGreenHPWidth(fInnerBarWidth * fHPFraction);

	ofPushStyle();
    poSpriteHP->draw(unX + fCameraOffsetX, unY + fCameraOffsetY);
    ofSetColor(206, 0, 0); //red
    ofDrawRectangle(unX + 1 + fCameraOffsetX, unY + 1 + fCameraOffsetY, 
					fInnerBarWidth, fInnerBarHeight); //fills the bar with red
    ofSetColor(0, 196, 0); //green
	ofDrawRectangle(unX + 1 + fCameraOffsetX, unY + 1 + fCameraOffsetY,
					fGreenHPWidth, fInnerBarHeight); //the green part on top
    ofPopStyle();
}