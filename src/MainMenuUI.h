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

#ifndef NOTAGAME_MAIN_MENU_UI_H
#define NOTAGAME_MAIN_MENU_UI_H

#include "GenericUI.h"
#include "TextArea.h"
#include "TextLabel.h"
#include "ButtonCluster.h"
#include "utility.h" //for GetSpriteFullPath()

#include "ofBitmapFont.h"
#include "ofAppRunner.h" //for ofGetWindowWidth()

#include <vector>
#include <string>

class MainMenuUI: public GenericUI {
public:
    MainMenuUI();
	void Draw() const;
	void Initialise();
	bool HoverButton(int, int);
	short PollButton(int, int) const;
	void SetButtonLabel(size_t, const std::string&);
	void SetTextMain(const std::string&);
	bool IsInHelpMode() const;
	void SetInHelpMode(bool);
private:
	const TextLabel& GetButtonLabel(size_t) const;
	const ButtonCluster& GetButtonCluster() const;
	ButtonCluster& GetButtonCluster();
	const ofColor& GetColourBackgroundTop() const;
	const ofColor& GetColourBackgroundBottom() const;
	const std::vector<TextLabel>& GetButtonLabels() const;
	std::vector<TextLabel>& GetButtonLabels();
	const ofBitmapFont& GetBitmapFont() const;
	const TextLabel& GetTextMain() const;
	TextLabel& GetTextMain();
	const TextArea& GetHelpWindow() const;
	TextArea& GetHelpWindow();

	const ofColor m_oColourBackgroundTop;
	const ofColor m_oColourBackgroundBottom;
    ButtonCluster m_oButtonCluster;
	std::vector<TextLabel> m_voButtonLabels;
	const ofBitmapFont m_oBitmapFont; //for label bounding boxes
	TextLabel m_oTextMain;
	bool m_bInHelpMode;
	TextArea m_oHelpWindow;
};

inline bool MainMenuUI::HoverButton(const int nX, const int nY) { 
	return GetButtonCluster().Hover(nX, nY);
}

inline short MainMenuUI::PollButton(const int nX, const int nY) const { 
	return GetButtonCluster().Poll(nX, nY);
}

inline bool MainMenuUI::IsInHelpMode() const {
	return m_bInHelpMode;
}

inline const ButtonCluster& MainMenuUI::GetButtonCluster() const {
	return m_oButtonCluster;
}

inline ButtonCluster& MainMenuUI::GetButtonCluster() {
	return m_oButtonCluster;
}

inline const ofColor& MainMenuUI::GetColourBackgroundTop() const {
	return m_oColourBackgroundTop;
}

inline const ofColor& MainMenuUI::GetColourBackgroundBottom() const {
	return m_oColourBackgroundBottom;
}

inline const TextLabel& MainMenuUI::GetButtonLabel(const size_t unIndex) const {
	return GetButtonLabels().at(unIndex);
}

inline const std::vector<TextLabel>& MainMenuUI::GetButtonLabels() const {
	return m_voButtonLabels;
}

inline std::vector<TextLabel>& MainMenuUI::GetButtonLabels() {
	return m_voButtonLabels;
}

inline const ofBitmapFont& MainMenuUI::GetBitmapFont() const {
	return m_oBitmapFont;
}

inline const TextLabel& MainMenuUI::GetTextMain() const {
	return m_oTextMain;
}

inline TextLabel& MainMenuUI::GetTextMain() {
	return m_oTextMain;
}

inline const TextArea& MainMenuUI::GetHelpWindow() const {
	return m_oHelpWindow;
}

inline TextArea& MainMenuUI::GetHelpWindow() {
	return m_oHelpWindow;
}

#endif