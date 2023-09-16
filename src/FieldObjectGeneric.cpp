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

#include "FieldObjectGeneric.h"

FieldObjectGeneric::FieldObjectGeneric(const std::string& strFileName,
									   const size_t unX, const size_t unY,
									   const float fSourceOffsetX,
									   //const float fSourceOffsetY,
									   const enumMaterial eMaterial):
	DrawableGeneric(strFileName, unX * SPRITE_W, unY * SPRITE_H, 
					fSourceOffsetX/*, fSourceOffsetY*/), 
	m_poPerceivedSprite(nullptr),
	m_PerceivedSpriteOffsets(std::make_pair(0, 0)),
	m_PerceivedWindowXY(std::make_pair(0, 0)),
	m_eMaterial(eMaterial), 
	m_bDiscovered(false)
{}

void FieldObjectGeneric::Draw(const float fCameraOffsetX, 
							  const float fCameraOffsetY) const {
	const std::shared_ptr<const ofColor> poColour(GetColour());
	const float fWindowX(GetPerceivedWindowX() + fCameraOffsetX);
	const float fWindowY(GetPerceivedWindowY() + fCameraOffsetY);
	const float fPixelOffsetX(GetPerceivedSpriteOffsetX() * SPRITE_W);
	const float fPixelOffsetY(GetPerceivedSpriteOffsetY() * SPRITE_H);

	ofPushStyle();
	if (poColour)
		ofSetColor(*poColour);
	GetPerceivedSprite()->drawSubsection(fWindowX, fWindowY, SPRITE_W, 
										 SPRITE_H, fPixelOffsetX, 
										 fPixelOffsetY);
	ofPopStyle();
}

//should be called when this object gets into the player's FOV
void FieldObjectGeneric::Update() {
	UpdateSpritePerceived();
	UpdateSpritePerceivedOffsets();
	UpdatePerceivedWindowCoordinates();
}