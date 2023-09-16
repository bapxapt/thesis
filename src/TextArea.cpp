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

#include "TextArea.h"

TextArea::TextArea(const int nTopLeftX, const int nTopLeftY, 
				   const size_t unWidth, const size_t unHeight, 
				   const enumHorizontal eHorizontal,
				   const enumVertical eVertical, const int nShiftX, 
				   const int nShiftY, const std::string& strFileName,
				   const std::string& strText):
	Alignable(nTopLeftX, nTopLeftY, unWidth, unHeight, eHorizontal,
			  eVertical, nShiftX, nShiftY),
	m_poSprite(std::make_shared<ofImage>(strFileName)),
	m_oSpriteColour(245, 225, 175),
	m_oTextlabel(0, 0, GetSprite()->getWidth(), GetSprite()->getHeight(), 
				 H_LEFT, V_TOP, 5, 5, strText, ofColor(0, 0, 0))
{
	Initialise();
}

TextArea::TextArea(const enumHorizontal eHorizontal,
				   const enumVertical eVertical, const int nShiftX, 
				   const int nShiftY, const std::string& strFileName,
				   const std::string& strText):
	TextArea(0, 0, ofGetWindowWidth(), ofGetWindowHeight(), eHorizontal, 
			 eVertical, nShiftX, nShiftY, strFileName, strText)
{}

//center of the whole window with a shift
TextArea::TextArea(const int nShiftX, const int nShiftY, 
				   const std::string& strFileName, const std::string& strText):
	TextArea(0, 0, ofGetWindowWidth(), ofGetWindowHeight(), H_CENTER, 
			 V_CENTER, nShiftX, nShiftY, strFileName, strText)
{}

void TextArea::Initialise() {
	const std::shared_ptr<const ofImage> poSprite(GetSprite());
	TextLabel& oTextLabel(GetTextLabel());

	SetContainerDimensions(ofGetWindowWidth(), ofGetWindowHeight());
	UpdateWindowCoordinates(poSprite->getWidth(), poSprite->getHeight());
	oTextLabel.SetContainerTopLeftCoordinates(GetWindowX(), GetWindowY());
	oTextLabel.Initialise();
}

void TextArea::Draw() const {
	const pair_int& TextXY(GetWindowCoordinates());

	ofPushStyle();
	ofSetColor(GetSpriteColour());
    GetSprite()->draw(TextXY.first, TextXY.second);
	ofPopStyle();
	GetTextLabel().Draw();
}