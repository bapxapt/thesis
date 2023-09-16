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

#ifndef NOTAGAME_FURNITURE_GENERIC_H
#define NOTAGAME_FURNITURE_GENERIC_H

#include "StandingGeneric.h"

#include <string>
#include <utility> //for std::pair

class FurnitureGeneric: public StandingGeneric {
public:
    FurnitureGeneric(const std::string&, size_t, size_t, enumMaterial, 
					 const std::string&, size_t, float, enumDirection, bool, 
					 bool, bool);
	bool IsMovable() const;
	void SetMovable(bool);
    bool IsContainer() const;
private:
	bool m_bMovable;
    const bool m_bContainer;
};

inline bool FurnitureGeneric::IsMovable() const {
	return m_bMovable;
}

inline void FurnitureGeneric::SetMovable(const bool bMovable) {
	m_bMovable = bMovable;
}

inline bool FurnitureGeneric::IsContainer() const { 
	return m_bContainer;
}

#endif