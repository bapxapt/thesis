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

#include "TrapBlinding.h"

TrapBlinding::TrapBlinding(const std::string& strFileName, 
						   const size_t unX, const size_t unY, 
						   const enumMaterial eMaterial, const size_t unMaxHP, 
						   const float fWeight, const UnitMap& oUnitMap):
    TrapGeneric(strFileName, unX, unY, eMaterial, "Blinding trap", unMaxHP, 
				fWeight),
	m_oUnitMap(oUnitMap)
{}

short TrapBlinding::Effect(const sptr_StandingGeneric poTrapped,
						   std::default_random_engine& oRNG) {
	if (!poTrapped->IsLiving())
		return 2; //cannot be blinded

	const auto poUnit(std::static_pointer_cast<UnitGeneric>(poTrapped));
	std::uniform_int_distribution<size_t> oDistributionTurns(15, 25);

	poUnit->AddBuff(UnitGeneric::STATUS_BLIND, oDistributionTurns(oRNG));
	return 1; //got blinded
}

std::string TrapBlinding::EffectMessage(const csptr_StandingGeneric poTrapped, 
										const short nEffectResult) const {
	if (nEffectResult == 2)
		return poTrapped->GetName() + " cannot be blinded.";
	if (nEffectResult == 1) {
		const bool bIsPlayer(GetUnitMap().IsPlayer(poTrapped));

		return (bIsPlayer ? "You" : poTrapped->GetName()) + " got blinded.";
	}
	throw std::runtime_error("TrapBlinding::EffectMessage(): an unexpected "
							 "value.");
}