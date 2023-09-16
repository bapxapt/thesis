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

#include "TrapMap.h"

TrapMap::TrapMap():
	FieldObjectMap<TrapGeneric>::FieldObjectMap(),
	m_setTrappableTilesXY()
{}

void TrapMap::TrappableAdd(const pair_size_t& TileXY) {
	if (!GetTrappableAll().insert(TileXY).second)
		throw std::logic_error("TrapMap::TrappableAdd(): duplicate pair (" +
							   ofToString(TileXY.first) + ", " + 
							   ofToString(TileXY.second) + ") found.");
}

void TrapMap::Clear() {
	GetTrappableAll().clear();
	FieldObjectMap<TrapGeneric>::Clear();
}

void TrapMap::Generate(const uset_pair_size_t& setPassableTiles,
					   UnitMap& oUnitMap, std::default_random_engine& oRNG) {
	uset_pair_size_t& setTrappableTiles(GetTrappableAll());
		
	setTrappableTiles = setPassableTiles;

	const size_t unTiles(setTrappableTiles.size());
	const size_t unTrapsMin(unTiles < 120 ? 1 : unTiles / 120);
	const size_t unTrapsMax(unTiles < 80 ? 1 : unTiles / 80);
    std::uniform_int_distribution<size_t> oDistributionTrapNumber(unTrapsMin,
																  unTrapsMax);
	std::uniform_int_distribution<size_t> oDistributionTrapCharges(2, 6);
	const size_t unTraps(oDistributionTrapNumber(oRNG));
	std::bernoulli_distribution oDistributionBernoulli;

	for (size_t iii(0); iii < unTraps; ++iii) {
		const pair_size_t& TileXY(GetTrappableRandom(oRNG));
		const size_t unCharges(oDistributionTrapCharges(oRNG));

		if (oDistributionBernoulli(oRNG)) 
			Add(TrapDamaging("trap_damaging.png", TileXY.first, TileXY.second, 
							 MAT_METAL, unCharges, 0, oUnitMap), false);
		else
			Add(TrapBlinding("trap_blinding.png", TileXY.first, TileXY.second, 
							 MAT_METAL, unCharges, 0, oUnitMap), false);
	}
}