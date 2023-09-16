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

#include "ButtonCluster.h"

ButtonCluster::ButtonCluster(const int nShiftX, const int nShiftY,
							 const std::string& strFileName, 
							 const size_t unButtons, 
							 const size_t unRows, const size_t unGap):
	Alignable(0, 0, ofGetWindowWidth(), ofGetWindowHeight(), H_CENTER, 
			  V_CENTER, nShiftX, nShiftY),
	m_poSpriteButton(std::make_shared<ofImage>(GetSpriteFullPath(strFileName))),
	m_poColourNormal(std::make_shared<const ofColor>(235, 210, 160)),
    m_poColourDisabled(std::make_shared<const ofColor>(220, 190, 150)), 
	m_poColourHovered(std::make_shared<const ofColor>(245, 225, 175)),
	m_voButtons(unButtons, DrawableGeneric(strFileName, GetSprite())),
	m_nHoveredIndex(-1),
	m_unGap(unGap),
	m_unRows(unRows)
{
	if (!unButtons)
		throw std::logic_error("ButtonCluster::ButtonCluster(): zero buttons");
	if (!unRows)
		throw std::logic_error("ButtonCluster::ButtonCluster(): zero rows "
							   "per column.");
	if (unButtons < unRows)
		throw std::logic_error("ButtonCluster::ButtonCluster(): not enough "
							   "buttons to fill a column.");
}

void ButtonCluster::Draw() const {
	for (const DrawableGeneric& oButton : GetButtons())
        oButton.Draw();
}

//returns the ID of a slot under the mouse pointer
short ButtonCluster::Poll(const int nX, const int nY) const {
	const std::vector<DrawableGeneric>& voButtons(GetButtons());
	const DrawableGeneric& oLast(voButtons.back()); //the bottom right button
	const std::shared_ptr<const ofImage> poSprite(GetSprite());
	const size_t unSpriteWidth(poSprite->getWidth());
	const size_t unSpriteHeight(poSprite->getHeight());
	const int nClusterTopLeftX(GetWindowX());
	const int nClusterTopLeftY(GetWindowY());

	if (nX < nClusterTopLeftX || 
		nX > nClusterTopLeftX + GetWidth() ||
		nY < nClusterTopLeftY || 
		nY > nClusterTopLeftY + GetHeight()) 
		return -1; //outside of any slot

	const int nInSquareX(nX - nClusterTopLeftX); //X inside of the cluster
	const int nInSquareY(nY - nClusterTopLeftY); //Y inside of the cluster
	const size_t unGap(GetGap());
	const int nColumnIndex(nInSquareX / (unSpriteWidth + unGap));
	const int nRowIndex(nInSquareY / (unSpriteHeight + unGap));
	const int nRemainderX(nInSquareX % (unSpriteWidth + unGap));
	const int nRemainderY(nInSquareY % (unSpriteHeight + unGap));

	if (nRemainderX >= unSpriteWidth || nRemainderY >= unSpriteHeight)
		return -1; //in a gap between rows/columns
	return nColumnIndex * CountRows() + nRowIndex; //1D button vector index
}

bool ButtonCluster::Hover(const int nX, const int nY) {
	const int nHoveredIndexCurrent(GetHoveredIndex());
	const int nHoveredIndexNew(Poll(nX, nY));
	std::vector<DrawableGeneric>& voButtons(GetButtons());
	const std::shared_ptr<const ofColor> poColourDisabled(GetColourDisabled());

	if (nHoveredIndexNew == nHoveredIndexCurrent)
		return false;
	SetHoveredIndex(nHoveredIndexNew);
	if (nHoveredIndexCurrent != -1 && IsButtonEnabled(nHoveredIndexCurrent))
		voButtons.at(nHoveredIndexCurrent).SetColour(GetColourNormal());
	if (nHoveredIndexNew != -1 && IsButtonEnabled(nHoveredIndexNew)) //hover
		voButtons.at(nHoveredIndexNew).SetColour(GetColourHovered());
	return true; //a change happened, need to redraw
}

void ButtonCluster::SetButtonEnabled(const size_t unIndex, const bool bEnabled) {
	DrawableGeneric& oButton(GetButtons().at(unIndex));

	if (bEnabled)
		oButton.SetColour(unIndex == GetHoveredIndex() ? GetColourHovered() : 
						  GetColourNormal());
	else
		oButton.SetColour(GetColourDisabled());
}