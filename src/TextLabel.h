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

#ifndef NOTAGAME_TEXT_LABEL_H
#define NOTAGAME_TEXT_LABEL_H

#include "Alignable.h"

#include "ofBitmapFont.h"
#include "ofAppRunner.h" //for ofGetWindowWidth()

#include <string>
#include <utility> //for std::pair

class TextLabel: public Alignable {
public:
    TextLabel(int, int, size_t, size_t, enumHorizontal, 
			  enumVertical, int, int, const std::string&, 
			  const ofColor& = ofColor());
    TextLabel(enumHorizontal, enumVertical, int, int, 
			  const std::string&, const ofColor& = ofColor());
    TextLabel(int, int, const std::string&, const ofColor& = ofColor());
	void Draw() const;
	void Initialise();
	const std::string& GetText() const;
	void SetText(const std::string&);
	void SetColour(float, float, float, float = 255.0);
private:
	const ofColor& GetColour() const;

	ofColor m_oColour;
	std::string m_strText;
};

inline void TextLabel::SetColour(const float fRed, const float fGreen, 
								 const float fBlue, const float fAlpha) {
	m_oColour.set(fRed, fGreen, fBlue, fAlpha);
}

inline const ofColor& TextLabel::GetColour() const {
	return m_oColour;
}

inline const std::string& TextLabel::GetText() const {
	return m_strText;
}

inline void TextLabel::SetText(const std::string& strText) {
	m_strText = strText;
}

#endif