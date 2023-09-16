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

#include "ConsumableKey.h"

ConsumableKey::ConsumableKey(const std::string& strFileName,
							 const size_t unX, const size_t unY, 
							 const enumMaterial eMaterial, 
							 const std::string& strName, 
							 const size_t unMaxHP, const float fWeight,
							 UnitMap& oUnitMap,
							 const bool bAbleToUnlockNormal,
							 const bool bAbleToUnlockSpecial):
	ConsumableGeneric(strFileName, unX, unY, eMaterial, strName, unMaxHP, 
					  fWeight),
	m_oUnitMap(oUnitMap),
	m_bAbleToUnlockNormal(bAbleToUnlockNormal),
	m_bAbleToUnlockSpecial(bAbleToUnlockSpecial)
{
	if (!bAbleToUnlockNormal && !bAbleToUnlockSpecial)
		throw std::logic_error("ConsumableKey::ConsumableKey(): not able to "
							   "unlock anything.");
}

short ConsumableKey::Effect(HumanoidGeneric& oUser,
							std::default_random_engine& oRNG) {
	UnitMap& oUnitMap(GetUnitMap());
	const pair_size_t InFrontXY(oUser.GetCoordinatesInFront()); //a copy
    const sptr_StandingGeneric poInFront(oUnitMap.Get(InFrontXY));

	if (!poInFront || poInFront->IsLiving())
		return -5;

    const auto poFurniture(std::static_pointer_cast<FurnitureGeneric>(poInFront));

	if (!poFurniture->IsContainer())
		return -4;

    const auto poContainer(std::static_pointer_cast<ContainerGeneric>(poFurniture));
	const enumDirection eUserDirection(oUser.GetDirection());
	const enumDirection eOpposite(GetDirectionOpposite(poContainer->GetDirection()));

	if (eUserDirection != eOpposite &&
		eUserDirection != GetDirectionTurned(eOpposite, -1) &&
		eUserDirection != GetDirectionTurned(eOpposite, 1)) 
		return -3;
	if (!poContainer->IsLocked())
		return -2;
	if (poContainer->IsInvulnerable() && !IsAbleToUnlockSpecial() ||
		!poContainer->IsInvulnerable() && !IsAbleToUnlockNormal())
		return -1;
    poContainer->SetLocked(false);
	return 0;
}

std::string ConsumableKey::EffectMessage(const csptr_HumanoidGeneric poUser,
										 const short nResult) const {
	const std::string& strItemName(GetName());

	if (nResult == -5)
		return "There is nothing to unlock with " + 
			   GetIndefiniteArticle(strItemName) + " " + strItemName + ".";

	const UnitMap& oUnitMap(GetUnitMap());
	const pair_size_t InFrontXY(poUser->GetCoordinatesInFront()); //a copy
    const std::string& strInFrontName(oUnitMap.Get(InFrontXY)->GetName());

	switch (nResult) {
		case -4:
			return "The " + strInFrontName + " is not a container.";
		case -3:
			return "You cannot unlock the " + strInFrontName + 
				   " from this side.";
		case -2:
			return "The " + strInFrontName + " in front of you is not locked.";
		case -1:
			return "The " + strInFrontName + " cannot be unlocked with this.";
		case 0:
			return "You successfully unlocked the " + strInFrontName + ".";
		default:
			throw std::runtime_error("ConsumableKey::EffectMessage(): "
									 "an unexpected value.");
	}
}