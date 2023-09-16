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

#include "EquipmentGeneric.h"

EquipmentGeneric::EquipmentGeneric(const std::string& strFileName, 
								   const size_t unX, const size_t unY, 
								   const enumMaterial eMaterial, 
								   const std::string& strName, 
								   const size_t unMaxHP, const float fWeight,
								   const enumEquipmentSlot eSlotIndex,
								   const bool bUnequppable, 
								   const int nBonusHP, 
								   const int nBonusStrength, 
								   const int nBonusIntelligence):
    ItemGeneric(strFileName, unX, unY, eMaterial, strName, unMaxHP, fWeight,
				TYPE_EQUIPMENT, 1),
	m_eSlotIndex(eSlotIndex),
	m_bUnequppable(bUnequppable),
	m_nBonusHP(nBonusHP),
	m_nBonusStrength(nBonusStrength),
	m_nBonusIntelligence(nBonusIntelligence)
{}

size_t EquipmentGeneric::CalculateChangedStat(const size_t unCurrentvalue, 
											  const long long nDelta) const {
	const size_t nDeltaAbsolute(abs(nDelta));

	if (nDelta < 0)
		return unCurrentvalue <= nDeltaAbsolute ? 1 : 
			   unCurrentvalue - nDeltaAbsolute;
	return unCurrentvalue + nDeltaAbsolute;
}

void EquipmentGeneric::ApplyAllBonuses(HumanoidGeneric& oWearer, 
									   const bool bEquipping) const {
	const short nMultiplier(bEquipping ? 1 : -1);
	const int nDeltaHP(GetBonusHP() * nMultiplier);
	const int nDeltaStrength(GetBonusStrength() * nMultiplier);
	const int nDeltaIntelligence(GetBonusIntelligence() * nMultiplier);
	const size_t unMaximalHPNew(CalculateChangedStat(oWearer.GetHPMaximal(), 
													 nDeltaHP));
	const size_t unStrengthNew(CalculateChangedStat(oWearer.GetStrength(), 
													nDeltaStrength));
	const size_t unIntelligenceNew(CalculateChangedStat(oWearer.GetStrength(), 
														nDeltaIntelligence));

	oWearer.SetStrength(unStrengthNew);
	oWearer.SetIntelligence(unIntelligenceNew);
	oWearer.SetHPMaximal(unMaximalHPNew);
	if (oWearer.GetHPCurrent() > unMaximalHPNew)
		oWearer.SetHPCurrent(unMaximalHPNew);
}