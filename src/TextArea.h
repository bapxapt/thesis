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

#ifndef NOTAGAME_TEXT_AREA_H
#define NOTAGAME_TEXT_AREA_H

#include "Alignable.h"
#include "TextLabel.h"

#include "ofImage.h"
#include "ofGraphics.h" //for ofPushStyle()
#include "ofAppRunner.h" //for ofGetWindowWidth()

#include <string>
#include <utility> //for std::pair

class TextArea: public Alignable {
public:
    TextArea(int, int, size_t, size_t, enumHorizontal, enumVertical, int, int, 
			 const std::string&, const std::string&);
    TextArea(enumHorizontal, enumVertical, int, int, const std::string&,
			 const std::string&);
    TextArea(int, int, const std::string&, const std::string&);
	void Draw() const;
	void Initialise();
	void SetColourSprite(float, float, float, float = 255.0);
	void SetText(const std::string&);
private:
	const ofColor& GetSpriteColour() const;
	std::shared_ptr<const ofImage> GetSprite() const;
	std::shared_ptr<ofImage> GetSprite();
	const TextLabel& GetTextLabel() const;
	TextLabel& GetTextLabel();

	ofColor m_oSpriteColour;
	const std::shared_ptr<ofImage> m_poSprite;
	TextLabel m_oTextlabel;
};

inline void TextArea::SetColourSprite(const float fRed, const float fGreen, 
									  const float fBlue, const float fAlpha) {
	m_oSpriteColour.set(fRed, fGreen, fBlue, fAlpha);
}

inline void TextArea::SetText(const std::string& strNew) {
	GetTextLabel().SetText(strNew);
}

inline const ofColor& TextArea::GetSpriteColour() const {
	return m_oSpriteColour;
}

inline std::shared_ptr<const ofImage> TextArea::GetSprite() const {
	return m_poSprite;
}

inline std::shared_ptr<ofImage> TextArea::GetSprite() {
	return m_poSprite;
}

inline const TextLabel& TextArea::GetTextLabel() const {
	return m_oTextlabel;
}

inline TextLabel& TextArea::GetTextLabel() {
	return m_oTextlabel;
}

#endif