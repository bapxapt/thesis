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

#ifndef NOTAGAME_ITEM_GENERIC_H
#define NOTAGAME_ITEM_GENERIC_H

#include "LyingGeneric.h"

#include <stdexcept>
#include <string>
#include <utility> //for std::pair

class ItemGeneric: public LyingGeneric {
public:
	enum enumItemType: unsigned char {TYPE_EQUIPMENT = 0, TYPE_CONSUMABLE,
									  TYPE_MISC};
    ItemGeneric(const std::string&, size_t, size_t, enumMaterial, 
				const std::string&, size_t, float, enumItemType, size_t);
	bool operator==(const LyingGeneric&) const = delete;
	bool operator==(const ItemGeneric&) const;
    enumItemType GetType() const;
    void SetType(enumItemType);
	size_t GetQuantity() const;
	void SetQuantity(size_t);
	bool IsStackable() const;
private:
    enumItemType m_eType;
	size_t m_unQuantity;
};

inline bool ItemGeneric::operator==(const ItemGeneric& oOther) const {
	return LyingGeneric::operator==(oOther) &&
		   GetType() == oOther.GetType();
}

inline ItemGeneric::enumItemType ItemGeneric::GetType() const { 
	return m_eType; 
}

inline void ItemGeneric::SetType(enumItemType eType) {
	m_eType = eType;
}

inline size_t ItemGeneric::GetQuantity() const {
	return m_unQuantity;
}

inline bool ItemGeneric::IsStackable() const {
	return GetType() != TYPE_EQUIPMENT;
}

inline void ItemGeneric::SetQuantity(const size_t unQuantity) {
	m_unQuantity = unQuantity;
}

#endif