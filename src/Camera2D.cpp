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

#include "Camera2D.h"

Camera2D::Camera2D(const long long nFieldWidth, const long long nFieldHeight):
	m_bFreeRoamMode(false),
	m_OffsetXY(std::make_pair(0.0, 0.0)),
	m_TopLeftTileXY(std::make_pair(0, 0)),
	m_BottomRightTileXY(std::make_pair(0, 0)),
	m_nFieldDimensions(std::make_pair(nFieldWidth, nFieldHeight))
{
	if (!nFieldWidth || !nFieldHeight)
		throw std::logic_error("Camera2D::Camera2D(): an invalid width or "
							   "height.");
}

bool Camera2D::MoveToMousePointer(const int nMouseX, const int nMouseY) {
	const int nRelativeToCenterMouseX(nMouseX - ofGetWindowWidth() * 0.5);
	const int nRelativeToCenterMouseY(nMouseY - ofGetWindowHeight() * 0.5);
	const float fMultiplier(0.015); //to slow the camera down
	bool bChanged(false);
	const size_t unViewPortWidth(GetBottomRightX() - GetTopLeftX());
	const size_t unViewPortHeight(GetBottomRightY() - GetTopLeftY());
	const float fViewPortWidthHalf(0.5 * unViewPortWidth);
	const float fViewPortHeightHalf(0.5 * unViewPortHeight);

	if (GetBottomRightX() > fViewPortWidthHalf && 
		nRelativeToCenterMouseX < 0 ||
		GetTopLeftX() < GetFieldWidth() - 1 - fViewPortWidthHalf && 
		nRelativeToCenterMouseX > 0) {
		SetOffsetX(GetOffsetX() - fMultiplier * nRelativeToCenterMouseX);
		bChanged = true;
	}
	if (GetBottomRightY() > fViewPortHeightHalf && 
		nRelativeToCenterMouseY < 0 ||
		GetTopLeftY() < GetFieldHeight() - 1 - fViewPortHeightHalf && 
		nRelativeToCenterMouseY > 0) {
		SetOffsetY(GetOffsetY() - fMultiplier * nRelativeToCenterMouseY);
		bChanged = true;
	}
	if (bChanged)
		UpdateViewPortBoundaries();
	return bChanged;
}

void Camera2D::UpdateViewPortBoundaries() {
	const int nWindowWidth(ofGetWindowWidth());
	const int nWindowHeight(ofGetWindowHeight());
	const float fOffsetX(GetOffsetX());
	const float fOffsetY(GetOffsetY());
	const long long nTopLeftX(-fOffsetX / SPRITE_W);
	const long long nTopLeftY(-fOffsetY / SPRITE_H);
	const long long nBottomRightX((-fOffsetX + nWindowWidth) / SPRITE_W);
	const long long nBottomRightY((-fOffsetY + nWindowHeight) / SPRITE_H);

	SetTopLeftCoordinates(nTopLeftX, nTopLeftY); //can be out of map bounds
	SetBottomRightCoordinates(nBottomRightX, nBottomRightY); //can be out
}

void Camera2D::Center(const long long nWindowX, const long long nWindowY) {
	SetOffsetX(ofGetWindowWidth() / 2 - nWindowX - SPRITE_W / 2);
	SetOffsetY(ofGetWindowHeight() / 2 - nWindowY - SPRITE_H / 2);
	UpdateViewPortBoundaries();
}

void Camera2D::Reset() {
	SetFreeRoamMode(false);
	SetOffsetX(0.0);
	SetOffsetY(0.0);
	SetTopLeftCoordinates(0, 0);
	SetBottomRightCoordinates(0, 0);
}