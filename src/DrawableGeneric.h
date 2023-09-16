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

#ifndef NOTAGAME_DRAWABLE_GENERIC_H
#define NOTAGAME_DRAWABLE_GENERIC_H

#include "define.h"
#include "utility.h"

#include "ofImage.h"
#include "ofGraphics.h" //for ofPushStyle()

#include <memory> //for std::shared_ptr
#include <string>
#include <utility> //for std::pair
#include <stdexcept>

class DrawableGeneric {
public:
	DrawableGeneric(const std::string&, int = 0, int = 0, int = 0, 
					int = 0, std::shared_ptr<const ofColor> = nullptr,
					std::shared_ptr<ofImage> = nullptr);
	DrawableGeneric(const std::string&, std::shared_ptr<ofImage>);
	bool operator==(const DrawableGeneric&) const;
	virtual void Draw(float = 0, float = 0) const;
	virtual void Draw(const pair_float&) const;
	std::shared_ptr<const ofColor> GetColour() const;
	void SetColour(std::shared_ptr<const ofColor>);
	void SetColour(float, float, float, float = 255);
	std::shared_ptr<const ofImage> GetSprite() const;
	std::shared_ptr<ofImage> GetSprite();
	virtual void SetSprite(std::shared_ptr<ofImage>);
	virtual void LoadSprite(bool = true);
	const pair_int& GetWindowCoordinates() const;
	void SetWindowCoordinates(float, float);
	int GetWindowX() const;
	void SetWindowX(int);
	int GetWindowY() const;
	void SetWindowY(int);
	const pair_int& GetSpriteOffsets() const; 
	float GetSpriteOffsetX() const;
	float GetSpriteOffsetY() const;
	void SetSpriteOffsetX(float);
	void SetSpriteOffsetY(float);
	const std::string& GetSpritePath() const;
	void SetSpritePath(const std::string&);
private:
	std::string m_strSpritePath;
	pair_int m_WindowXY;
	pair_int m_SpriteOffsets;
	std::shared_ptr<ofImage> m_poSprite;
	std::shared_ptr<const ofColor> m_poColour;
};

//does not check the sprite pointer, an equal path should be enough
inline bool DrawableGeneric::operator==(const DrawableGeneric& oOther) const {
	return GetSpritePath() == oOther.GetSpritePath() &&
		   GetSpriteOffsets() == oOther.GetSpriteOffsets();
}

inline void DrawableGeneric::Draw(const pair_float& CameraOffsets) const {
	Draw(CameraOffsets.first, CameraOffsets.second);
}

inline std::shared_ptr<const ofColor> DrawableGeneric::GetColour() const {
	return m_poColour;
}

inline void DrawableGeneric::SetColour(const std::shared_ptr<const ofColor> poColour) {
	m_poColour = poColour;
}

inline void DrawableGeneric::SetColour(const float fRed, const float fGreen, 
									   const float fBlue, const float fAlpha) {
	m_poColour = std::make_shared<const ofColor>(fRed, fBlue, fGreen, fAlpha);
}

inline const std::pair<int, int>& DrawableGeneric::GetWindowCoordinates() const {
	return m_WindowXY;
}

inline void DrawableGeneric::SetWindowCoordinates(const float fWindowX, 
												  const float fWindowY) {
	m_WindowXY = std::make_pair(fWindowX, fWindowY);
}

inline int DrawableGeneric::GetWindowX() const {
	return GetWindowCoordinates().first;
}

inline void DrawableGeneric::SetWindowX(const int nWindowX) {
	m_WindowXY.first = nWindowX;
}

inline int DrawableGeneric::GetWindowY() const {
	return GetWindowCoordinates().second;
}

inline void DrawableGeneric::SetWindowY(const int nWindowY) {
	m_WindowXY.second = nWindowY;
}

inline std::shared_ptr<const ofImage> DrawableGeneric::GetSprite() const { 
	return m_poSprite; 
}

inline std::shared_ptr<ofImage> DrawableGeneric::GetSprite() { 
	return m_poSprite; 
}

inline void DrawableGeneric::SetSprite(const std::shared_ptr<ofImage> poSprite) { 
	m_poSprite = poSprite; 
}

inline const std::pair<int, int>& DrawableGeneric::GetSpriteOffsets() const {
	return m_SpriteOffsets;
}

inline float DrawableGeneric::GetSpriteOffsetX() const {
	return m_SpriteOffsets.first;
}

inline float DrawableGeneric::GetSpriteOffsetY() const {
	return m_SpriteOffsets.second;
}

inline void DrawableGeneric::SetSpriteOffsetX(const float fOffset) {
	m_SpriteOffsets.first = fOffset;
}

inline void DrawableGeneric::SetSpriteOffsetY(const float fOffset) {
	m_SpriteOffsets.second = fOffset;
}

inline const std::string& DrawableGeneric::GetSpritePath() const {
	return m_strSpritePath;
}

inline void DrawableGeneric::SetSpritePath(const std::string& strPath) {
	m_strSpritePath = strPath;
}

#endif