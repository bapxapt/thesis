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

#ifndef NOTAGAME_STANDING_GENERIC_H
#define NOTAGAME_STANDING_GENERIC_H

#include "NameableGeneric.h"

#include <string>
#include <utility> //for std::pair

class StandingGeneric: public NameableGeneric {
public:
    StandingGeneric(const std::string&, size_t, size_t, enumMaterial, 
					const std::string&, size_t, float, enumDirection, bool,
					bool);
    bool IsLiving() const;
	void SetLiving(bool);
    enumDirection GetDirection() const;
    void SetDirection(enumDirection);
	enumDirection GetPerceivedDirection() const;
	void Update() override;
	pair_size_t GetCoordinatesInFront(size_t = 1) const;
	bool IsInvulnerable() const;
	void SetInvulnerable(bool);
	bool ReceiveDamage(size_t);
private:
	void UpdatePerceivedDirection();

    enumDirection m_eDirection;
	enumDirection m_ePerceivedDirection;
    bool m_bLiving; //true if it can act on its own, false if not (inanimate object)
	bool m_bInvulnerable;
};

inline bool StandingGeneric::IsLiving() const { 
	return m_bLiving; 
}

inline void StandingGeneric::SetLiving(bool bLiving) {
	m_bLiving = bLiving;
}

inline enumDirection StandingGeneric::GetDirection() const { 
	return m_eDirection; 
}

inline void StandingGeneric::SetDirection(enumDirection eNew) { 
	m_eDirection = eNew; 
}

inline enumDirection StandingGeneric::GetPerceivedDirection() const {
	return m_ePerceivedDirection;
}

inline pair_size_t StandingGeneric::GetCoordinatesInFront(const size_t unTiles) const {
	return std::make_pair(GetX() + unTiles * GetStepX(GetDirection()), 
						  GetY() + unTiles * GetStepY(GetDirection())); 
}

inline void StandingGeneric::UpdatePerceivedDirection() {
	m_ePerceivedDirection = GetDirection();
}

inline bool StandingGeneric::IsInvulnerable() const {
	return m_bInvulnerable;
}

inline void StandingGeneric::SetInvulnerable(const bool bInvulnerable) {
	m_bInvulnerable = bInvulnerable;
}

#endif