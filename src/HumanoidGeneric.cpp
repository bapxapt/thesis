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

#include "HumanoidGeneric.h"

HumanoidGeneric::HumanoidGeneric(const std::string& strFileName,
								 const size_t unX, const size_t unY, 
								 const enumMaterial eMaterial, 
								 const std::string& strName, 
								 const size_t unMaxHP, const float fWeight, 
								 const enumDirection eDirection,
								 const bool bInvulnerable,
								 const size_t unBaseDistanceFOV,
								 const size_t unStrength, 
								 const size_t unIntelligence):
    UnitGeneric(strFileName, unX, unY, eMaterial, strName, unMaxHP, 
				fWeight, eDirection, bInvulnerable, unBaseDistanceFOV, 
				unStrength, unIntelligence, true),
	m_Encumbrance(std::make_pair(0.0, unStrength * 25)),
	m_vpoBackpack(),
	m_vpoEquipment(SLOT_N_EQUIPMENT, nullptr)
{
	GetBackpack().reserve(SLOT_N_BACKPACK);
}

short HumanoidGeneric::Consume(const size_t unBackpackIndex, 
							   std::default_random_engine& oRNG) {
	const sptr_ItemGeneric poItem(GetBackpackItem(unBackpackIndex));
	const auto poConsumable(std::static_pointer_cast<ConsumableGeneric>(poItem));
	const short nConsumptionResult(poConsumable->Effect(*this, oRNG));

	if (nConsumptionResult >= 0) { //if negative, consumption failed
		const size_t unQuantityNew(poItem->GetQuantity() - 1);

		poItem->SetQuantity(unQuantityNew);
		if (!unQuantityNew)
			RemoveBackpackItem(unBackpackIndex);
	}
	return nConsumptionResult;
}

//returns values from -2 to N-1 backpack slot index
short HumanoidGeneric::Unequip(const size_t unIndex) {
	const size_t unItemsInBackpack(CountBackpackItems());
	const sptr_EquipmentGeneric poEquipment(GetEquipped(unIndex));
	std::vector<sptr_ItemGeneric>& vpoBackpack(GetBackpack());
	const size_t unBackpackSizeBeforePush(vpoBackpack.size());

	if (!poEquipment->IsUnequppable())
		return -2;

	const auto Lambda([] (const csptr_ItemGeneric poInBackpack) -> bool {
						  return !poInBackpack; //nullptr
					  });
	const short nNullIndex(GetBackpackSlotIndex(Lambda)); 

	if (nNullIndex == -1) { //not swapping, just unequipping
		if (unItemsInBackpack == SLOT_N_BACKPACK)
			return -1; //no space in the backpack
		m_vpoBackpack.push_back(poEquipment);
	}
	else //swapping
		vpoBackpack.at(nNullIndex) = poEquipment;
	GetEquipment().at(unIndex) = nullptr;
	poEquipment->EffectOnUnequipping(*this);
	return unBackpackSizeBeforePush; //the last slot index
}

//returns values from -3 to N-1 (excluding -1) equipment slot index
short HumanoidGeneric::Equip(const size_t unBackpackIndex) {
	const sptr_ItemGeneric poNew(GetBackpackItem(unBackpackIndex));
	const auto poNewGear(std::static_pointer_cast<EquipmentGeneric>(poNew));
    const enumEquipmentSlot eEquipmentIndex(poNewGear->GetSlotIndex());
    const sptr_EquipmentGeneric poInSlot(GetEquipped(eEquipmentIndex));
	std::vector<sptr_ItemGeneric>& vpoBackpack(GetBackpack());

	if (poInSlot) {
		if (*poInSlot == *poNewGear) //an identical item
			return -3;
		if (!poInSlot->IsUnequppable()) //not unequippable
			return -2;
		vpoBackpack.at(unBackpackIndex) = nullptr;
		Unequip(eEquipmentIndex);
	}
	vpoBackpack.at(unBackpackIndex) = poInSlot; //can be a nullptr
    GetEquipment().at(eEquipmentIndex) = poNewGear;
	if (!poInSlot) //no swap happened
		vpoBackpack.erase(std::next(vpoBackpack.begin(), unBackpackIndex));
	poNewGear->EffectOnEquipping(*this);
    return eEquipmentIndex;
}

//returns -1 if failed to pick up, or from 0 to N-1 slots
short HumanoidGeneric::PickUp(const sptr_ItemGeneric poNew) {   
    std::vector<sptr_ItemGeneric>& vpoBackpack(GetBackpack());
	const auto Lambda([poNew] (const csptr_ItemGeneric poInBackpack) -> bool {
						  return *poInBackpack == *poNew; 
					  });
	const short nBackpackIndex(GetBackpackSlotIndex(Lambda));
    const sptr_ItemGeneric poSame(nBackpackIndex < 0 ? nullptr : 
								  GetBackpackItem(nBackpackIndex)); 

	if (poSame && poSame->IsStackable()) {
		poSame->SetQuantity(poSame->GetQuantity() + poNew->GetQuantity());
		return nBackpackIndex; //managed to stack the item
	} //cannot stack, try to add

	const size_t unBackpackSizeBeforePush(CountBackpackItems());

	if (unBackpackSizeBeforePush == SLOT_N_BACKPACK) 
		return -2; //cannot be stacked, and no empty slot
	vpoBackpack.push_back(poNew);
    SetEncumbranceCurrent(GetEncumbranceCurrent() + poNew->GetWeight());
    return unBackpackSizeBeforePush; //the last slot index
}

//if not the last one, shifts the backpack back
void HumanoidGeneric::RemoveBackpackItem(const size_t unIndex) {
    std::vector<sptr_ItemGeneric>& vpoBackpack(GetBackpack());

	SetEncumbranceCurrent(GetEncumbranceCurrent() - 
						  GetBackpackItem(unIndex)->GetWeight());
	vpoBackpack.erase(std::next(vpoBackpack.begin(), unIndex));
}

//attempts to get the eqiupment slot index of a backpack item
enumEquipmentSlot HumanoidGeneric::ToEquipmentIndex(const size_t unIndex) const {
	const csptr_ItemGeneric poItem(GetBackpackItem(unIndex));

	if (!poItem || poItem->GetType() != ItemGeneric::TYPE_EQUIPMENT)
		return SLOT_NONE; //not equippable

	const auto poGear(std::static_pointer_cast<const EquipmentGeneric>(poItem));

	return poGear->GetSlotIndex();
}

void HumanoidGeneric::SetStrength(const size_t unStrength) {
	UnitGeneric::SetStrength(unStrength);
	m_Encumbrance.second = GetStrength() * 25;
}