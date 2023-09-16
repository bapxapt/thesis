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

#include "MainMenuUI.h"

MainMenuUI::MainMenuUI():
	m_oColourBackgroundTop(51, 102, 153), 
	m_oColourBackgroundBottom(122, 186, 122), 
	m_oButtonCluster(0, 0, "ui_slot_backpack.png", 3, 3),
	m_voButtonLabels({TextLabel(0, 0, "Start", ofColor(0, 0, 0)),
					  TextLabel(0, 0, "Help", ofColor(0, 0, 0)),
					  TextLabel(0, 0, "Exit", ofColor(0, 0, 0))}),
	m_oBitmapFont(),
	m_oTextMain(0, -150, "A sprawling labyrinth awaits."),
	m_bInHelpMode(false),
	m_oHelpWindow(0, 0, GetSpriteFullPath("ui_log_window.png"), 
				  "G - pick up an item;\nF - force attack;\n"
				  "P - push furniture;\nI - inventory screen;\n"
				  "M - camera free roam mode;\n"
				  "Shift+click - drop an item (on the inventory screen).")
{} //gets initialised in ofApp::windowResized()

void MainMenuUI::Initialise() {
	if (!IsInitialisationNeeded())
		return;

	std::vector<TextLabel>& voButtonLabels(GetButtonLabels());
	const size_t unLabels(voButtonLabels.size());
	TextLabel& oTextMain(GetTextMain());
	ButtonCluster& oButtonCluster(GetButtonCluster());

	oButtonCluster.SetContainerDimensions(ofGetWindowWidth(), 
										  ofGetWindowHeight());
    oButtonCluster.Initialise([] (const size_t unIndex) -> bool {
								  return true;
							  }); //all buttons are always enabled
	oTextMain.SetContainerDimensions(ofGetWindowWidth(), ofGetWindowHeight());
	oTextMain.Initialise();
	for (size_t iii(0); iii < unLabels; ++iii) {
		TextLabel& oButtonLabel(voButtonLabels.at(iii));
		const DrawableGeneric& oButton(oButtonCluster.Get(iii));
		const std::shared_ptr<const ofImage> poSprite(oButton.GetSprite());

		oButtonLabel.SetContainerTopLeftCoordinates(oButton.GetWindowX(), 
													oButton.GetWindowY());
		oButtonLabel.SetContainerDimensions(poSprite->getWidth(), 
											poSprite->getHeight());
		oButtonLabel.Initialise();
	}
	GetHelpWindow().Initialise();
	SetInitialisationNeeded(false);
}

void MainMenuUI::Draw() const {
	const std::vector<TextLabel>& voButtonLabels(GetButtonLabels());

	ofBackgroundGradient(GetColourBackgroundTop(), GetColourBackgroundBottom(), 
						 OF_GRADIENT_LINEAR);
	GetTextMain().Draw();
	GetButtonCluster().Draw();
	for (const TextLabel& oLabel : voButtonLabels)
		oLabel.Draw();
	if (IsInHelpMode())
		GetHelpWindow().Draw();
}

void MainMenuUI::SetButtonLabel(const size_t unIndex, 
								const std::string& strLabel) {
	TextLabel& oButtonLabel(GetButtonLabels().at(unIndex));

	oButtonLabel.SetText(strLabel);
	oButtonLabel.Initialise();
}

void MainMenuUI::SetTextMain(const std::string& strText) {
	TextLabel& oTextMain(GetTextMain());

	oTextMain.SetText(strText);
	oTextMain.Initialise();
}

void MainMenuUI::SetInHelpMode(const bool bInHelpMode) {
	m_bInHelpMode = bInHelpMode;
	GetButtonCluster().ResetHoveredIndex();
}