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

#include "TextLabel.h"

TextLabel::TextLabel(const int nTopLeftX, const int nTopLeftY, 
					 const size_t unWidth, const size_t unHeight, 
					 const enumHorizontal eHorizontal,
					 const enumVertical eVertical, const int nShiftX, 
					 const int nShiftY, const std::string& strText, 
					 const ofColor& oColour):
	Alignable(nTopLeftX, nTopLeftY, unWidth, unHeight, eHorizontal,
			  eVertical, nShiftX, nShiftY),
	m_oColour(oColour),
	m_strText(strText)
{
	Initialise();
}

TextLabel::TextLabel(const enumHorizontal eHorizontal,
					 const enumVertical eVertical, const int nShiftX, 
					 const int nShiftY, const std::string& strText, 
					 const ofColor& oColour):
	TextLabel(0, 0, ofGetWindowWidth(), ofGetWindowHeight(), eHorizontal, 
			  eVertical, nShiftX, nShiftY, strText, oColour)
{}

//center of the whole window with a shift
TextLabel::TextLabel(const int nShiftX, const int nShiftY, 
					 const std::string& strText, const ofColor& oColour):
	TextLabel(0, 0, ofGetWindowWidth(), ofGetWindowHeight(), H_CENTER, 
			  V_CENTER, nShiftX, nShiftY, strText, oColour)
{}

void TextLabel::Initialise() {
	const ofRectangle oBoundingBox(ofBitmapFont().getBoundingBox(GetText(), 0, 
																 0));

	UpdateWindowCoordinates(oBoundingBox.getWidth(), oBoundingBox.getHeight());
}

void TextLabel::Draw() const {
	const pair_int& TextXY(GetWindowCoordinates());

	ofPushStyle();
	ofSetColor(GetColour());
    ofDrawBitmapString(GetText(), TextXY.first, TextXY.second);
	ofPopStyle();
}