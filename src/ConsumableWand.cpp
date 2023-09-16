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

#include "ConsumableWand.h"

ConsumableWand::ConsumableWand(const std::string& strFileName,
							   const size_t unX, const size_t unY, 
							   const enumMaterial eMaterial, 
							   const std::string& strName, 
							   const size_t unMaxHP, const float fWeight,
							   LayoutMap& oLayoutMap):
    ConsumableGeneric(strFileName, unX, unY, eMaterial, strName, unMaxHP, 
					  fWeight),
	m_oLayoutMap(oLayoutMap)
{}

short ConsumableWand::Effect(HumanoidGeneric& oUser,
							 std::default_random_engine& oRNG) {
    const enumDirection eUserDirection(oUser.GetDirection());
	const short nStepX(GetStepX(eUserDirection));
	const short nStepY(GetStepY(eUserDirection));
    LayoutMap& oLayoutMap(GetLayoutMap());
	size_t unX(oUser.GetX()); //start from the user's coordinates
	size_t unY(oUser.GetY()); //start from the user's coordinates
	sptr_LayoutGeneric poTile(oLayoutMap.Get(unX, unY));
	const size_t unSpriteOffsetX(poTile->GetPerceivedSpriteOffsetX());

	while (!oLayoutMap.IsWall(poTile)) { //search for a first encountered wall
		unX += nStepX;
		unY += nStepY;
		poTile = oLayoutMap.Get(unX, unY);
	}
	if (oLayoutMap.IsBorder(poTile)) 
		return poTile->IsInFOV() ? 2 : 3;
	oLayoutMap.Replace(FloorGeneric(unX, unY), oRNG, true);
	return poTile->IsInFOV() ? 0 : 1;
}

std::string ConsumableWand::EffectMessage(const csptr_HumanoidGeneric poUser,
										  const short nResult) const {
	switch (nResult) {
		case 0:
			return "The wall falls to pieces!";
		case 1:
			return "You hear stone shattering in the distance.";
		case 2:
			return "The wall rumbles for a moment.";
		case 3:
			return "A wall in the distance rumbles slightly.";
		default:
			throw std::runtime_error("ConsumableWand::EffectMessage(): "
									 "an unexpected value.");
	}
}