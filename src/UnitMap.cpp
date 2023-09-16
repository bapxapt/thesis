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

#include "UnitMap.h"

UnitMap::UnitMap(LayoutMap& oLayoutMap):
	FieldObjectMap<StandingGeneric>::FieldObjectMap(),
    m_poPlayer(nullptr),
	m_oCalculatorFOV(oLayoutMap),
	m_setpoContainers()
{}

template<typename T> 
std::shared_ptr<T> UnitMap::Add(const T& oNew, LayoutMap& oLayoutMap, 
								const bool bUseTexture) {
	const std::shared_ptr<T> poNew(std::make_shared<T>(oNew));

	if (poNew->IsLiving()) {
		const auto poUnit(std::dynamic_pointer_cast<UnitGeneric>(poNew));

		GetCalculatorFOV().Calculate(poUnit);
	}
	else {
		const auto poContainer(std::dynamic_pointer_cast<ContainerGeneric>(poNew));
		
		if (poContainer && !poContainer->IsInvulnerable())
			GetContainers().insert(poContainer);
	}
	FieldObjectMap<StandingGeneric>::Add(poNew, bUseTexture);
	oLayoutMap.PassableRemove(poNew->GetCoordinates());
	return poNew;
}

void UnitMap::Clear() {
	SetPlayer(nullptr);
	GetContainers().clear();
	FieldObjectMap<StandingGeneric>::Clear();
}

void UnitMap::Remove(const pair_size_t& TileXY) {
	const sptr_StandingGeneric poRemoved(Get(TileXY));

	if (!poRemoved->IsLiving()) {
		const auto poFurniture(std::static_pointer_cast<FurnitureGeneric>(poRemoved));

		if (poFurniture->IsContainer())
			GetContainers().erase(std::static_pointer_cast<ContainerGeneric>(poFurniture));
	}
	else
		if (IsPlayer(poRemoved))
			SetPlayer(nullptr);
	
	FieldObjectMap<StandingGeneric>::Remove(TileXY);
}

const pair_size_t UnitMap::Generate(uset_pair_size_t& setContainerXY,
									LayoutMap& oLayoutMap,
									std::default_random_engine& oRNG) {
	const pair_size_t& PlayerXY(oLayoutMap.GetPassableRandom(oRNG));
	const HumanoidGeneric oPlayer("player.png", PlayerXY.first, 
								  PlayerXY.second, MAT_FLESH, "Player", 
								  10, 70.0, GetDirectionRandom(oRNG), false, 
								  10, 1, 1);

	SetPlayer(Add(oPlayer, oLayoutMap, false));
	GenerateFurniture(setContainerXY, oLayoutMap, oRNG);

	const pair_size_t OrnateXY(oLayoutMap.GetPassableRandom(oRNG)); //a copy
	const ContainerGeneric oOrnate("chest_ornate.png", OrnateXY.first, 
								   OrnateXY.second, MAT_WOOD, "Ornate chest", 
								   1, 1000.0, GetDirectionRandom(oRNG), true, 
								   false, true);
	
	Add(oOrnate, oLayoutMap, false);
	GenerateOpponents(oLayoutMap, oRNG);
	return OrnateXY;
}

void UnitMap::GenerateFurniture(uset_pair_size_t& setContainerXY,
								LayoutMap& oLayoutMap,
								std::default_random_engine& oRNG) {
	const size_t unPassableTiles(oLayoutMap.PassableCount());
    std::bernoulli_distribution oDistributionBernoulli;
	std::uniform_int_distribution<size_t> oDistributionFurnitureNumber(unPassableTiles / 25,
																	   unPassableTiles / 20);
	std::uniform_int_distribution<size_t> oDistributionHP(900, 1500);
	std::uniform_int_distribution<size_t> oDistributionUnitWeight(500, 800);
	std::uniform_int_distribution<size_t> oDistributionContainerFile(1, 3);
	std::uniform_int_distribution<size_t> oDistributionFurnitureFile(1, 2);
	std::uniform_int_distribution<size_t> oDistributionFurnitureWeight(100, 
																	   500);
	std::uniform_int_distribution<size_t> oDistributionCoins(1, 4);
	const size_t unFurnitureNumber(oDistributionFurnitureNumber(oRNG));
	
    for (size_t iii(0); iii != unFurnitureNumber; ++iii) {
        size_t unHP(oDistributionHP(oRNG));
		const float fWeight(oDistributionFurnitureWeight(oRNG) * 0.1);
		const pair_size_t& TileXY(oLayoutMap.GetPassableRandom(oRNG));
		const enumDirection eDirection(GetDirectionRandom(oRNG));

        if (oDistributionBernoulli(oRNG)) {
			const size_t unFileIndex(oDistributionContainerFile(oRNG));
            const std::string strFileName("container" + 
										  ofToString(unFileIndex) + ".png");
			const bool bLocked(TrueWithProbability(oRNG, 0.3));
			
			setContainerXY.emplace(TileXY);
			Add(ContainerGeneric(strFileName, TileXY.first, TileXY.second, 
								 MAT_WOOD, "Container", unHP, fWeight, 
								 eDirection, false, true, bLocked), 
				oLayoutMap, false);
        }
        else { //non-containers
			const size_t unFileIndex(oDistributionFurnitureFile(oRNG));
			const std::string strFileName("furniture" + 
										  ofToString(unFileIndex) + ".png");

			Add(FurnitureGeneric(strFileName, TileXY.first, TileXY.second, 
								 MAT_WOOD, "Furniture", unHP, fWeight, 
								 eDirection, false, true, false), 
				oLayoutMap, false);
        }
    }
}

void UnitMap::GenerateOpponents(LayoutMap& oLayoutMap,
								std::default_random_engine& oRNG) {
	const size_t unPassableTiles(oLayoutMap.PassableCount());
	std::uniform_int_distribution<size_t> oDistributionUnitNumber(unPassableTiles / 55,
																  unPassableTiles / 40);
	std::uniform_int_distribution<size_t> oDistributionHP(30, 50);
	std::uniform_int_distribution<size_t> oDistributionUnitWeight(40, 100);
	std::uniform_int_distribution<size_t> oDistributionColour(0, 255);
	const size_t unUnitNumber(oDistributionUnitNumber(oRNG));

	for (size_t iii(0); iii != unUnitNumber; ++iii) {
        size_t unHP(oDistributionHP(oRNG));
		const float fWeight(oDistributionUnitWeight(oRNG) * 0.1);
		const pair_size_t& UnitXY(oLayoutMap.GetPassableRandom(oRNG));
		const enumDirection eDirection(GetDirectionRandom(oRNG));
		UnitGeneric oBubble("unit_bubble.png", UnitXY.first, UnitXY.second, 
							MAT_FLESH, "Bubble", unHP, fWeight, eDirection, 
							false, 3, 1, 1, false);

		oBubble.SetColour(oDistributionColour(oRNG), oDistributionColour(oRNG), 
						  oDistributionColour(oRNG));
		Add(oBubble, oLayoutMap, false);
	}
}