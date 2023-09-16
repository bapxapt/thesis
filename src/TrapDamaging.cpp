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

#include "TrapDamaging.h"

TrapDamaging::TrapDamaging(const std::string& strFileName, const size_t unX, 
						   const size_t unY, const enumMaterial eMaterial, 
						   const size_t unMaxHP, const float fWeight,
						   const UnitMap& oUnitMap):
    TrapGeneric(strFileName, unX, unY, eMaterial, "Damaging trap", unMaxHP, 
				fWeight),
	m_oUnitMap(oUnitMap)
{}

short TrapDamaging::Effect(const sptr_StandingGeneric poTrapped, 
						   std::default_random_engine& oRNG) {
	float fMultiplier(1.0);
	std::uniform_int_distribution<size_t> oDistributionDamage(3, 5);

	if (poTrapped->IsLiving()) {
		const auto poUnit(std::static_pointer_cast<UnitGeneric>(poTrapped));

		if (poUnit->IsCapableOfAdvancedManipulations()) {
			const auto poHumanoid(std::static_pointer_cast<HumanoidGeneric>(poTrapped));

			if (poHumanoid->GetEquipped(SLOT_LBOOT))
				fMultiplier -= 0.2;
			if (poHumanoid->GetEquipped(SLOT_RBOOT))
				fMultiplier -= 0.2;
		}
	}
	poTrapped->ReceiveDamage(oDistributionDamage(oRNG) * fMultiplier);
	SetHPCurrent(GetHPCurrent() - 1); //minus one charge for the trap
	return fMultiplier == 1.0 ? 1 : 2;
}

std::string TrapDamaging::EffectMessage(const csptr_StandingGeneric poTrapped, 
										const short nEffectResult) const {
	if (!GetUnitMap().IsPlayer(poTrapped)) {
		if (poTrapped->GetHPCurrent())
			return "The " + poTrapped->GetName() + " got damaged by spikes.";
		else
			return "The " + poTrapped->GetName() + " got de" + 
				   (poTrapped->IsLiving() ? "feat" : "stroy") + 
				   "ed by spikes!";
	}
	if (nEffectResult == 1) //unmitigated
		return "You got damaged by spikes.";
	if (nEffectResult == 2)
		return "You received some damage (reduced by boots).";
	throw std::runtime_error("TrapDamaging::EffectMessage(): an unexpected "
							 "value.");
}