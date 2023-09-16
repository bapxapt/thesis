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

#ifndef NOTAGAME_HUMANOID_GENERIC_H
#define NOTAGAME_HUMANOID_GENERIC_H

#include "ConsumableGeneric.h"
#include "EquipmentGeneric.h"
#include "UnitGeneric.h"

#include <vector>
#include <algorithm> //for std::find_if
#include <memory> //for std::shared_ptr
#include <string>
#include <utility> //for std::pair
#include <random> //for std::default_random_engine

class HumanoidGeneric: public UnitGeneric {
public:
    HumanoidGeneric(const std::string&, size_t, size_t, enumMaterial, 
					const std::string&, size_t, float, enumDirection, bool, 
					size_t, size_t, size_t);
	void SetStrength(size_t) override;
	sptr_EquipmentGeneric GetEquipped(size_t);
	csptr_EquipmentGeneric GetEquipped(size_t) const;
	sptr_ItemGeneric GetBackpackItem(size_t);
    csptr_ItemGeneric GetBackpackItem(size_t) const;
	size_t CountBackpackItems() const;
	short Consume(size_t, std::default_random_engine&);
    short Equip(size_t);
    short PickUp(sptr_ItemGeneric);
    short Unequip(size_t);
	const pair_float& GetEncumbrance() const;
	float GetEncumbranceCurrent() const;
	void SetEncumbranceCurrent(float);
	float GetEncumbranceMaximal() const;
	void SetEncumbranceMaximal(float);
	enumEquipmentSlot ToEquipmentIndex(size_t) const;
    void RemoveBackpackItem(size_t);
	bool IsOverEncumbered() const;
	template<typename F> short GetBackpackSlotIndex(const F&) const;
private:
	const std::vector<sptr_EquipmentGeneric>& GetEquipment() const;
    std::vector<sptr_EquipmentGeneric>& GetEquipment();
	const std::vector<sptr_ItemGeneric>& GetBackpack() const;
    std::vector<sptr_ItemGeneric>& GetBackpack();

    std::pair<float, float> m_Encumbrance; //current and max carried weight (encumbrance)
    std::vector<sptr_ItemGeneric> m_vpoBackpack;
    std::vector<sptr_EquipmentGeneric> m_vpoEquipment;
};

template<typename F>
short HumanoidGeneric::GetBackpackSlotIndex(const F& Lambda) const {
    const std::vector<sptr_ItemGeneric>& vpoBackpack(GetBackpack());
	const auto FoundItr(std::find_if(vpoBackpack.cbegin(), vpoBackpack.cend(), 
						Lambda));

    if (FoundItr != vpoBackpack.cend())
        return FoundItr - vpoBackpack.cbegin();
    return -1;
}

inline const std::vector<sptr_EquipmentGeneric>& HumanoidGeneric::GetEquipment() const { 
	return m_vpoEquipment; 
}

inline std::vector<sptr_EquipmentGeneric>& HumanoidGeneric::GetEquipment() { 
	return m_vpoEquipment; 
}

inline const std::vector<sptr_ItemGeneric>& HumanoidGeneric::GetBackpack() const { 
	return m_vpoBackpack; 
}

inline std::vector<sptr_ItemGeneric>& HumanoidGeneric::GetBackpack() { 
	return m_vpoBackpack; 
}

inline size_t HumanoidGeneric::CountBackpackItems() const {
	return GetBackpack().size();
}

inline sptr_EquipmentGeneric HumanoidGeneric::GetEquipped(const size_t unSlot) { 
	return GetEquipment().at(unSlot); 
}

inline csptr_EquipmentGeneric HumanoidGeneric::GetEquipped(const size_t unSlot) const { 
	return GetEquipment().at(unSlot);
}

inline sptr_ItemGeneric HumanoidGeneric::GetBackpackItem(const size_t unSlot) { 
	return unSlot < GetBackpack().size() ? GetBackpack().at(unSlot) : nullptr;
}

inline csptr_ItemGeneric HumanoidGeneric::GetBackpackItem(const size_t unSlot) const {
	return unSlot < GetBackpack().size() ? GetBackpack().at(unSlot) : nullptr; 
}

inline const pair_float& HumanoidGeneric::GetEncumbrance() const {
	return m_Encumbrance;
}

inline float HumanoidGeneric::GetEncumbranceCurrent() const { 
	return m_Encumbrance.first; 
}

inline void HumanoidGeneric::SetEncumbranceCurrent(const float fNew) { 
	m_Encumbrance.first = fNew; 
}

inline float HumanoidGeneric::GetEncumbranceMaximal() const { 
	return m_Encumbrance.second; 
}

inline void HumanoidGeneric::SetEncumbranceMaximal(const float fNew) { 
	m_Encumbrance.second = fNew; 
}

inline bool HumanoidGeneric::IsOverEncumbered() const {
	return GetEncumbranceCurrent() > GetEncumbranceMaximal();
}

#endif