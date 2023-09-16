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

#ifndef NOTAGAME_FIELD_OBJECT_GENERIC_H
#define NOTAGAME_FIELD_OBJECT_GENERIC_H

#include "DrawableGeneric.h"
#include "define.h"

#include "ofImage.h"

#include <memory> //for std::shared_ptr
#include <string>
#include <utility> //for std::pair

class FieldObjectGeneric: public DrawableGeneric { //including walls/floor
public: 
    FieldObjectGeneric(const std::string&, size_t, size_t, float,
					   enumMaterial);
	bool operator==(const DrawableGeneric&) const = delete;
	bool operator==(const FieldObjectGeneric&) const;
	void Draw(float = 0, float = 0) const override;
	void Draw(const pair_float&) const override;
	pair_size_t GetCoordinates() const;
	void SetCoordinates(size_t, size_t); //tile XY
	void SetCoordinates(const pair_size_t&); //tile XY
	size_t GetX() const; //tile X, not window X
	size_t GetY() const; //tile Y, not window Y
	void SetX(size_t); 
	void SetY(size_t);
	enumMaterial GetMaterial() const;
	void SetMaterial(enumMaterial);
	bool IsDiscovered() const;
	void SetDiscovered(bool);
	const pair_int& GetPerceivedWindowCoordinates() const;
	void SetPerceivedWindowCoordinates(const pair_int&);
	int GetPerceivedWindowX() const;
	int GetPerceivedWindowY() const;
	pair_size_t GetPerceivedCoordinates() const; //tile XY
	int GetPerceivedX() const; //tile X
	int GetPerceivedY() const; //tile Y
	const pair_size_t& GetPerceivedSpriteOffsets() const;
	void SetPerceivedSpriteOffsets(const pair_size_t&);
	size_t GetPerceivedSpriteOffsetX() const;
	void SetPerceivedSpriteOffsetX(size_t);
	size_t GetPerceivedSpriteOffsetY() const;
	void SetPerceivedSpriteOffsetY(size_t);
	std::shared_ptr<const ofImage> GetPerceivedSprite() const;
	void SetPerceivedSprite(std::shared_ptr<const ofImage>);
	virtual void Update();
	void UpdatePerceivedWindowCoordinates();
	void UpdateSpritePerceived();
	void UpdateSpritePerceivedOffsets();
private:
	std::shared_ptr<const ofImage> m_poPerceivedSprite;
	pair_size_t m_PerceivedSpriteOffsets; //in tile coordinates
	pair_int m_PerceivedWindowXY; //player saw last time at (drawn there under fog)
	enumMaterial m_eMaterial;
	bool m_bDiscovered; //by the player
};

inline bool FieldObjectGeneric::operator==(const FieldObjectGeneric& oOther) const {
	return DrawableGeneric::operator==(oOther) &&
		   GetMaterial() == oOther.GetMaterial();
}

inline void FieldObjectGeneric::Draw(const pair_float& CameraOffsets) const {
	Draw(CameraOffsets.first, CameraOffsets.second);
}

//returns a copy
inline pair_size_t FieldObjectGeneric::GetCoordinates() const {
	return std::make_pair(GetX(), GetY());
}

inline void FieldObjectGeneric::SetCoordinates(const size_t unX, 
											   const size_t unY) {
	SetWindowCoordinates(unX * SPRITE_W, unY * SPRITE_H);
}

inline void FieldObjectGeneric::SetCoordinates(const pair_size_t& TileXY) {
	SetCoordinates(TileXY.first, TileXY.second);
}

inline size_t FieldObjectGeneric::GetX() const {
	return GetWindowX() / SPRITE_W;
}

inline size_t FieldObjectGeneric::GetY() const {
	return GetWindowY() / SPRITE_H;
}

inline void FieldObjectGeneric::SetX(const size_t unX) {
	SetWindowX(unX * SPRITE_W);
}

inline void FieldObjectGeneric::SetY(const size_t unY) {
	SetWindowY(unY * SPRITE_H);
}

inline enumMaterial FieldObjectGeneric::GetMaterial() const {
	return m_eMaterial;
}

inline void FieldObjectGeneric::SetMaterial(const enumMaterial eNew) {
	m_eMaterial = eNew;
}

inline bool FieldObjectGeneric::IsDiscovered() const {
	return m_bDiscovered;
}

inline void FieldObjectGeneric::SetDiscovered(const bool bDiscovered) {
	m_bDiscovered = bDiscovered;
}

inline const pair_int& FieldObjectGeneric::GetPerceivedWindowCoordinates() const {
	return m_PerceivedWindowXY;
}

inline void FieldObjectGeneric::SetPerceivedWindowCoordinates(const pair_int& WindowXY) {
	m_PerceivedWindowXY = WindowXY;
}

inline int FieldObjectGeneric::GetPerceivedWindowX() const {
	return m_PerceivedWindowXY.first;
}

inline int FieldObjectGeneric::GetPerceivedWindowY() const {
	return m_PerceivedWindowXY.second;
}

inline pair_size_t FieldObjectGeneric::GetPerceivedCoordinates() const {
	return std::make_pair(GetPerceivedX(), GetPerceivedY());
}

inline int FieldObjectGeneric::GetPerceivedX() const {
	return GetPerceivedWindowX() / SPRITE_W;
}

inline int FieldObjectGeneric::GetPerceivedY() const{
	return GetPerceivedWindowY() / SPRITE_H;
}

inline std::shared_ptr<const ofImage> FieldObjectGeneric::GetPerceivedSprite() const {
	return m_poPerceivedSprite;
}

inline void FieldObjectGeneric::SetPerceivedSprite(const std::shared_ptr<const ofImage> poPerceived) {
	m_poPerceivedSprite = poPerceived;
}

inline const pair_size_t& FieldObjectGeneric::GetPerceivedSpriteOffsets() const {
	return m_PerceivedSpriteOffsets;
}

inline void FieldObjectGeneric::SetPerceivedSpriteOffsets(const pair_size_t& OffsetsNew) {
	m_PerceivedSpriteOffsets = OffsetsNew;
}

inline size_t FieldObjectGeneric::GetPerceivedSpriteOffsetX() const {
	return m_PerceivedSpriteOffsets.first;
}

inline void FieldObjectGeneric::SetPerceivedSpriteOffsetX(const size_t unOffsetX) {
	m_PerceivedSpriteOffsets.first = unOffsetX;
}

inline size_t FieldObjectGeneric::GetPerceivedSpriteOffsetY() const {
	return m_PerceivedSpriteOffsets.second;
}

inline void FieldObjectGeneric::SetPerceivedSpriteOffsetY(const size_t unOffsetY) {
	m_PerceivedSpriteOffsets.second = unOffsetY;
}

inline void FieldObjectGeneric::UpdatePerceivedWindowCoordinates() {
	SetPerceivedWindowCoordinates(GetWindowCoordinates());
}

inline void FieldObjectGeneric::UpdateSpritePerceived() {
	SetPerceivedSprite(GetSprite());
}

inline void FieldObjectGeneric::UpdateSpritePerceivedOffsets() {
	SetPerceivedSpriteOffsets(GetSpriteOffsets());
}

#endif