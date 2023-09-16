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

#ifndef NOTAGAME_CAMERA_2D_H
#define NOTAGAME_CAMERA_2D_H

#include "define.h"

#include "ofAppRunner.h" //for ofGetWindowWidth()

#include <utility> //for std::pair
#include <stdexcept>

class Camera2D {
public:
	Camera2D(long long, long long);
	void Reset();
	bool IsInView(const pair_size_t&) const;
	bool IsFreeRoamMode() const;
	void SetFreeRoamMode(bool);
	void Center(const pair_size_t&);
	void Center(long long, long long);
	size_t CountVisibleExistingTiles() const;
	const pair_float& GetOffsets() const;
	float GetOffsetX() const;
	float GetOffsetY() const;
	size_t GetTopLeftExistingX() const;
	size_t GetTopLeftExistingY() const;
	size_t GetBottomRightExistingX() const;
	size_t GetBottomRightExistingY() const;
	bool MoveToMousePointer(int, int);
private:
	size_t GetCoordinateExisting(long long, long long) const;
	long long GetTopLeftX() const;
	long long GetTopLeftY() const;
	long long GetBottomRightX() const;
	long long GetBottomRightY() const;
	void SetOffsetX(float);
	void SetOffsetY(float);
	void UpdateViewPortBoundaries();
	void SetTopLeftCoordinates(long long, long long);
	void SetBottomRightCoordinates(long long, long long);
	long long GetFieldWidth() const;
	long long GetFieldHeight() const;

	bool m_bFreeRoamMode;
	pair_float m_OffsetXY;
	pair_long_long m_TopLeftTileXY; //XY of a top left tile in the viewport
	pair_long_long m_BottomRightTileXY; //bottom right tile XY in the viewport
	pair_long_long m_nFieldDimensions;
};

inline bool Camera2D::IsInView(const pair_size_t& TileXY) const {
	return TileXY.first >= GetTopLeftExistingX() && 
		   TileXY.first <= GetBottomRightExistingX() &&
		   TileXY.second >= GetTopLeftExistingY() && 
		   TileXY.second <= GetBottomRightExistingY();
}

inline void Camera2D::Center(const pair_size_t& TileXY) {
	Center(TileXY.first * SPRITE_W, TileXY.second * SPRITE_H);
}

inline const pair_float& Camera2D::GetOffsets() const {
	return m_OffsetXY;
}

inline float Camera2D::GetOffsetX() const {
	return m_OffsetXY.first;
}

inline void Camera2D::SetOffsetX(const float fOffsetX) {
	m_OffsetXY.first = fOffsetX;
}

inline float Camera2D::GetOffsetY() const {
	return m_OffsetXY.second;
}

inline void Camera2D::SetOffsetY(const float fOffsetY) {
	m_OffsetXY.second = fOffsetY;
}

//within map bounds; includes partially visible and undiscovered
inline size_t Camera2D::CountVisibleExistingTiles() const {
	return (GetBottomRightExistingX() - GetTopLeftExistingX() + 1) * 
		   (GetBottomRightExistingY() - GetTopLeftExistingY() + 1); 
}

inline bool Camera2D::IsFreeRoamMode() const {
	return m_bFreeRoamMode;
}

inline void Camera2D::SetFreeRoamMode(const bool bFreeRoam) {
	m_bFreeRoamMode = bFreeRoam;
}

inline void Camera2D::SetTopLeftCoordinates(const long long nX, 
											const long long nY) {
	m_TopLeftTileXY = std::make_pair(nX, nY);
}

inline void Camera2D::SetBottomRightCoordinates(const long long nX, 
												const long long nY) {
	m_BottomRightTileXY = std::make_pair(nX, nY);
}

inline long long Camera2D::GetTopLeftX() const {
	return m_TopLeftTileXY.first;
}

inline long long Camera2D::GetTopLeftY() const {
	return m_TopLeftTileXY.second;
}

inline long long Camera2D::GetBottomRightX() const {
	return m_BottomRightTileXY.first;
}

inline long long Camera2D::GetBottomRightY() const {
	return m_BottomRightTileXY.second;
}

inline long long Camera2D::GetFieldWidth() const {
	return m_nFieldDimensions.first;
}

inline long long Camera2D::GetFieldHeight() const {
	return m_nFieldDimensions.second;
}

inline size_t Camera2D::GetCoordinateExisting(const long long nCoordinate, 
											  const long long nMaximum) const {
	return nCoordinate < 0 ? 0 :
		   (nCoordinate > nMaximum ? nMaximum : nCoordinate);
}

inline size_t Camera2D::GetTopLeftExistingX() const {
	return GetCoordinateExisting(GetTopLeftX(), GetFieldWidth() - 1);
}

inline size_t Camera2D::GetTopLeftExistingY() const {
	return GetCoordinateExisting(GetTopLeftY(), GetFieldHeight() - 1);
}

inline size_t Camera2D::GetBottomRightExistingX() const {
	return GetCoordinateExisting(GetBottomRightX(), GetFieldWidth() - 1);
}

inline size_t Camera2D::GetBottomRightExistingY() const {
	return GetCoordinateExisting(GetBottomRightY(), GetFieldHeight() - 1);
}

#endif