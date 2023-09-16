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

#include "LoadingScreenUI.h"

LoadingScreenUI::LoadingScreenUI():
	m_oColourBackgroundTop(51, 102, 153), 
	m_oColourBackgroundBottom(122, 186, 122),  
	m_oTextLabel(0, 0, "Loading...")
{} //get initialised in ofApp::windowResized()

void LoadingScreenUI::Initialise() {
	if (!IsInitialisationNeeded())
		return;

	TextLabel& oMainText(GetTextLabel());

	oMainText.SetContainerDimensions(ofGetWindowWidth(), ofGetWindowHeight());
	oMainText.Initialise();
	SetInitialisationNeeded(false);
}

void LoadingScreenUI::Draw() const {
	ofBackgroundGradient(GetColourBackgroundTop(), GetColourBackgroundBottom(), 
						 OF_GRADIENT_LINEAR);
	GetTextLabel().Draw();
}