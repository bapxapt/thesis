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

#ifndef NOTAGAME_CONSUMABLE_KEY_H
#define NOTAGAME_CONSUMABLE_KEY_H

#include "ConsumableGeneric.h"
#include "UnitMap.h"

#include <string>
#include <utility> //for std::pair
#include <random> //for std::default_random_engine
#include <stdexcept>

class ConsumableKey: public ConsumableGeneric {
public:
    ConsumableKey(const std::string&, size_t, size_t, enumMaterial, 
				  const std::string&, size_t, float, UnitMap&, bool = true,
				  bool = false);
    short Effect(HumanoidGeneric&, std::default_random_engine&) override;
	std::string EffectMessage(csptr_HumanoidGeneric, short) const override;
	bool IsAbleToUnlockNormal() const;
	void SetAbleToUnlockNormal(bool);
	bool IsAbleToUnlockSpecial() const;
	void SetAbleToUnlockSpecial(bool);
private:
	const UnitMap& GetUnitMap() const;
	UnitMap& GetUnitMap();

    UnitMap& m_oUnitMap;
	bool m_bAbleToUnlockNormal;
	bool m_bAbleToUnlockSpecial;
};

inline bool ConsumableKey::IsAbleToUnlockNormal() const {
	return m_bAbleToUnlockNormal;
}

inline void ConsumableKey::SetAbleToUnlockNormal(const bool bAble) {
	m_bAbleToUnlockNormal = bAble;
}

inline bool ConsumableKey::IsAbleToUnlockSpecial() const {
	return m_bAbleToUnlockSpecial;
}

inline void ConsumableKey::SetAbleToUnlockSpecial(const bool bAble) {
	m_bAbleToUnlockSpecial = bAble;
}


inline const UnitMap& ConsumableKey::GetUnitMap() const {
	return m_oUnitMap;
}

inline UnitMap& ConsumableKey::GetUnitMap() {
	return m_oUnitMap;
}

#endif