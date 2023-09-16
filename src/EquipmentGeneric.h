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

#ifndef NOTAGAME_EQUIPMENT_GENERIC_H
#define NOTAGAME_EQUIPMENT_GENERIC_H

#include "HumanoidGeneric.h"
#include "ItemGeneric.h"
#include "define.h"

#include <string>
#include <utility> //for std::pair

class EquipmentGeneric: public ItemGeneric {
public:
    EquipmentGeneric(const std::string&, size_t, size_t, enumMaterial, 
					 const std::string&, size_t, float, enumEquipmentSlot,
					 bool, int = 0, int = 0, int = 0);
    enumEquipmentSlot GetSlotIndex() const;
    void SetSlotIndex(enumEquipmentSlot);
	bool IsUnequppable() const;
    void SetUnequppable(bool);
	void EffectOnEquipping(HumanoidGeneric&) const;
    void EffectOnUnequipping(HumanoidGeneric&) const;
private:
	void ApplyAllBonuses(HumanoidGeneric&, bool) const;
	size_t CalculateChangedStat(size_t, long long) const;
	int GetBonusHP() const;
	int GetBonusStrength() const;
	int GetBonusIntelligence() const;
	void SetBonusHP(int);
	void SetBonusStrength(int);
	void SetBonusIntelligence(int);

    enumEquipmentSlot m_eSlotIndex;
	bool m_bUnequppable;
	int m_nBonusHP;
	int m_nBonusStrength;
	int m_nBonusIntelligence;
};

inline enumEquipmentSlot EquipmentGeneric::GetSlotIndex() const { 
	return m_eSlotIndex; 
}

inline void EquipmentGeneric::SetSlotIndex(enumEquipmentSlot eSlotIndex) {
	m_eSlotIndex = eSlotIndex;
}

inline bool EquipmentGeneric::IsUnequppable() const {
	return m_bUnequppable;
}

inline void EquipmentGeneric::SetUnequppable(const bool bUnequppable) {
	m_bUnequppable = bUnequppable;
}

inline int EquipmentGeneric::GetBonusHP() const {
	return m_nBonusHP;
}

inline void EquipmentGeneric::SetBonusHP(const int nBonusHP) {
	m_nBonusHP = nBonusHP;
}

inline int EquipmentGeneric::GetBonusStrength() const {
	return m_nBonusStrength;
}

inline void EquipmentGeneric::SetBonusStrength(const int nBonusStrength) {
	m_nBonusStrength = nBonusStrength;
}

inline int EquipmentGeneric::GetBonusIntelligence() const {
	return m_nBonusIntelligence;
}

inline void EquipmentGeneric::SetBonusIntelligence(const int nBonusIntelligence) {
	m_nBonusIntelligence = nBonusIntelligence;
}

inline void EquipmentGeneric::EffectOnEquipping(HumanoidGeneric& oWearer) const {
	ApplyAllBonuses(oWearer, true);
}

inline void EquipmentGeneric::EffectOnUnequipping(HumanoidGeneric& oWearer) const {
	ApplyAllBonuses(oWearer, false);
}

#endif