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

#ifndef NOTAGAME_LOADING_SCREEN_UI_H
#define NOTAGAME_LOADING_SCREEN_UI_H

#include "GenericUI.h"
#include "TextLabel.h"
#include "define.h"

#include "ofBitmapFont.h"
#include "ofAppRunner.h" //for ofGetWindowWidth()

#include <vector>
#include <string>
#include <utility> //for std::pair

class LoadingScreenUI: public GenericUI {
public:
    LoadingScreenUI();
	void Draw() const;
	void Initialise();
private:
	const ofColor& GetColourBackgroundTop() const;
	const ofColor& GetColourBackgroundBottom() const;
	const TextLabel& GetTextLabel() const;
	TextLabel& GetTextLabel();

	const ofColor m_oColourBackgroundTop;
	const ofColor m_oColourBackgroundBottom;
	TextLabel m_oTextLabel;
};

inline const ofColor& LoadingScreenUI::GetColourBackgroundTop() const {
	return m_oColourBackgroundTop;
}

inline const ofColor& LoadingScreenUI::GetColourBackgroundBottom() const {
	return m_oColourBackgroundBottom;
}

inline const TextLabel& LoadingScreenUI::GetTextLabel() const {
	return m_oTextLabel;
}

inline TextLabel& LoadingScreenUI::GetTextLabel() {
	return m_oTextLabel;
}

#endif