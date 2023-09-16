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

#include "DrawableGeneric.h"

DrawableGeneric::DrawableGeneric(const std::string& strFileName,
								 const int nWindowX, const int nWindowY, 
								 const int nSourceOffsetX, 
								 const int nSourceOffsetY, 
								 const std::shared_ptr<const ofColor> poColour,
								 const std::shared_ptr<ofImage> poSprite):
	m_strSpritePath(GetSpriteFullPath(strFileName)),
    m_poSprite(poSprite), //can be nullptr; load with LoadSprite()
	m_SpriteOffsets(std::make_pair(nSourceOffsetX, nSourceOffsetY)),
	m_WindowXY(std::make_pair(nWindowX, nWindowY)),
	m_poColour(poColour)
{}

DrawableGeneric::DrawableGeneric(const std::string& strFileName,
								 const std::shared_ptr<ofImage> poSprite):
	DrawableGeneric(strFileName, 0, 0, 0, 0, nullptr, poSprite)
{}

void DrawableGeneric::Draw(const float fCameraOffsetX, 
						   const float fCameraOffsetY) const {
	const std::shared_ptr<const ofImage> poSprite(GetSprite()); 
	const std::shared_ptr<const ofColor> poColour(GetColour());

	ofPushStyle();
	if (poColour)
		ofSetColor(*poColour);
	GetSprite()->drawSubsection(GetWindowX() + fCameraOffsetX, 
								GetWindowY() + fCameraOffsetY, 
								poSprite->getWidth(), poSprite->getHeight(), 
								GetSpriteOffsetX(), GetSpriteOffsetY());
	ofPopStyle();
}

void DrawableGeneric::LoadSprite(const bool bUseTexture) {
	const std::shared_ptr<ofImage> poSprite(std::make_shared<ofImage>());

	poSprite->setUseTexture(bUseTexture);
	poSprite->load(GetSpritePath());
	SetSprite(poSprite);
}