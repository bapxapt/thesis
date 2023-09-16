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

#ifndef NOTAGAME_LYING_GENERIC_H
#define NOTAGAME_LYING_GENERIC_H

#include "NameableGeneric.h"

#include <string>

class LyingGeneric: public NameableGeneric { //passable objects on the floor
public:
    LyingGeneric(const std::string&, size_t, size_t, enumMaterial, 
				 const std::string&, size_t, float, bool);
	bool operator==(const NameableGeneric&) const = delete;
	bool operator==(const LyingGeneric&) const;
    bool IsPickable() const;
    void SetPickable(bool);
private:
    bool m_bPickable;
};

inline bool LyingGeneric::operator==(const LyingGeneric& oOther) const {
	return NameableGeneric::operator==(oOther) &&
		   IsPickable() == oOther.IsPickable();
}

inline bool LyingGeneric::IsPickable() const {
	return m_bPickable;
}

inline void LyingGeneric::SetPickable(bool bPickable) {
	m_bPickable = bPickable;
}

#endif