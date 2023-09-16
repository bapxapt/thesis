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

#ifndef NOTAGAME_LAYOUT_GENERIC_H
#define NOTAGAME_LAYOUT_GENERIC_H

#include "FieldObjectGeneric.h"
#include "define.h"

#include "ofImage.h"

#include <memory> //for std::shared_ptr
#include <string>

class LayoutGeneric: public FieldObjectGeneric { //walls, floor
public:
    LayoutGeneric(const std::string&, size_t, size_t, float, enumMaterial,
				  bool, bool);
	bool operator==(const LayoutGeneric&) const;
    bool IsPassable() const;
    void SetPassable(bool);
    bool IsOpaque() const;
    void SetOpaque(bool);
	bool IsInFOV() const;
    void SetInFOV(bool);
private:
	bool m_bPassable;
	bool m_bOpaque; //cannot be seen through
	bool m_bInFOV; //instead of UnitGeneric::IsInFOV() - slow in DrawFog()
};

inline bool LayoutGeneric::operator==(const LayoutGeneric& oOther) const {
	return GetSprite() == oOther.GetSprite() &&
		   IsOpaque() == oOther.IsOpaque() &&
		   IsPassable() == oOther.IsPassable();
}

inline bool LayoutGeneric::IsPassable() const { 
	return m_bPassable;
}

inline void LayoutGeneric::SetPassable(const bool bPass) { 
	m_bPassable = bPass; 
}

inline bool LayoutGeneric::IsOpaque() const { 
	return m_bOpaque; 
}

inline void LayoutGeneric::SetOpaque(const bool bOpaque) { 
	m_bOpaque = bOpaque; 
}

inline bool LayoutGeneric::IsInFOV() const {
	return m_bInFOV;
}

inline void LayoutGeneric::SetInFOV(const bool bInFOV) {
	m_bInFOV = bInFOV;
}

#endif