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

#include "UnitGeneric.h"

UnitGeneric::UnitGeneric(const std::string& strFileName, const size_t unX, 
						 const size_t unY, const enumMaterial eMaterial, 
						 const std::string& strName, const size_t unMaxHP, 
						 const float fWeight, const enumDirection eDirection,
						 const bool bInvulnerable, 
						 const size_t unBaseDistanceFOV, 
						 const size_t unStrength, const size_t unIntelligence, 
						 const bool bCapableOfAdvanced):
    StandingGeneric(strFileName, unX, unY, eMaterial, strName, unMaxHP, 
					fWeight, eDirection, true, bInvulnerable),
	m_unBaseDistanceFOV(unBaseDistanceFOV),
	m_unStrength(unStrength), 
	m_unIntelligence(unIntelligence),
	m_bCapableOfAdvancedManipulations(bCapableOfAdvanced),
	m_msetTilesXYInView()
{
	InitialiseFOV();
}

void UnitGeneric::InitialiseFOV() {
	std::unordered_map<short, uset_pair_size_t>& mFOV(GetFOV());

	for (short iii(-2); iii <= 2; ++iii) //\, sector, |, sector, /
		mFOV.emplace(iii, uset_pair_size_t());
}

short UnitGeneric::Hit(const sptr_StandingGeneric poTarget, 
					   std::default_random_engine& oRNG) const {
	const size_t unStrength(GetStrength());
	std::uniform_int_distribution<size_t> oDistributionDamage(unStrength * 7, 
															  unStrength * 13);
	const size_t unFinalDamage(oDistributionDamage(oRNG));
	const size_t unTargetHP(poTarget->GetHPCurrent());

    if (poTarget->IsLiving()) { //can't miss inanimate objects
		if (TrueWithProbability(oRNG, 0.05)) //base miss chance
			return -1; //miss, no change to the target's HP
	}
	poTarget->ReceiveDamage(unFinalDamage);
	return poTarget->GetHPCurrent() ? 1 : 0; //1 if target's HP is above zero
}

void UnitGeneric::AddBuff(const enumStatusEffect eEffect, 
						  const size_t unTurns) {
	if (!unTurns)
		throw std::logic_error("UnitGeneric::AddBuff(): zero turns.");
	GetBuffs().insert_or_assign(eEffect, unTurns);
}

void UnitGeneric::TickBuffs() {
	std::unordered_map<enumStatusEffect, size_t>& munBuffs(GetBuffs());

	for (auto Itr(munBuffs.begin()); Itr != munBuffs.end(); ) {
		std::pair<const enumStatusEffect, size_t>& KeyValue(*Itr);

		KeyValue.second--;
		Itr = KeyValue.second ? std::next(Itr) : munBuffs.erase(Itr);
	}
}