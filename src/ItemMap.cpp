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

#include "ItemMap.h"

ItemMap::ItemMap():
	FieldObjectMultimap<ItemGeneric>::FieldObjectMultimap()
{}

void ItemMap::DivideItemsByRarity(std::vector<size_t>& vunLootTable,
								  const size_t unItems,
								  std::default_random_engine& oRNG) const {
    const size_t unRolls(100);
    std::lognormal_distribution<float> oDistributionLogNormal(-0.25, 0.45);
    const size_t unSize(vunLootTable.size());

    for (size_t iii(0); iii < unRolls; ++iii) {
        const float fCategoryIndex(oDistributionLogNormal(oRNG));

        if (fCategoryIndex >= 0 && fCategoryIndex < unSize)
            vunLootTable.at(static_cast<size_t>(fCategoryIndex))++;
    }
    for (size_t iii(0); iii != unSize; ++iii)
        vunLootTable.at(iii) *= unItems / unRolls;
}

void ItemMap::GenerateOnFloor(LayoutMap& oLayoutMap, UnitMap& oUnitMap, 
							  std::default_random_engine& oRNG) {
	std::vector<size_t> vunLootTable(3, 0);
	const size_t unCategories(vunLootTable.size());

	DivideItemsByRarity(vunLootTable, oLayoutMap.PassableCount() * 0.025, oRNG);
	for (size_t iii(0); iii < vunLootTable.at(0); ++iii) 
		Add(GenerateVeryCommon(oLayoutMap, oUnitMap, oRNG), false);
	for (size_t iii(0); iii < vunLootTable.at(1); ++iii) 
		Add(GenerateCommon(oLayoutMap, oUnitMap, oRNG), false);
	for (size_t iii(0); iii < vunLootTable.at(2); ++iii) 
		Add(GenerateNormal(oLayoutMap, oUnitMap, oRNG), false);
}

void ItemMap::GenerateInUnits(LayoutMap& oLayoutMap, UnitMap& oUnitMap, 
							  std::default_random_engine& oRNG) const {
	std::vector<size_t> vunLootTable(3, 0);
	const size_t unCategories(vunLootTable.size());

	DivideItemsByRarity(vunLootTable, oLayoutMap.PassableCount() * 0.06, oRNG);
	for (size_t iii(0); iii < vunLootTable.at(1); ++iii)
		AddToRandomContainer(oUnitMap, 
							 GenerateCommon(oLayoutMap, oUnitMap, oRNG), oRNG);
	for (size_t iii(0); iii < vunLootTable.at(2); ++iii)
		AddToRandomContainer(oUnitMap, 
							 GenerateNormal(oLayoutMap, oUnitMap, oRNG), oRNG);
}

void ItemMap::Generate(const uset_pair_size_t& setContainerXY,
					   const pair_size_t& OrnateXY,
					   LayoutMap& oLayoutMap, UnitMap& oUnitMap,
					   std::default_random_engine& oRNG) {
	GenerateOnFloor(oLayoutMap, oUnitMap, oRNG);
	GenerateInUnits(oLayoutMap, oUnitMap, oRNG);
	AddGoalKey(oLayoutMap, oUnitMap, oRNG);

	const sptr_StandingGeneric poStanding(oUnitMap.Get(OrnateXY));
	const auto poOrnate(std::static_pointer_cast<ContainerGeneric>(poStanding));
	const ItemGeneric oOrb("orb.png", poStanding->GetX(), poStanding->GetY(),
						   MAT_STONE, "Final orb", 1, 1.0, 
						   ItemGeneric::TYPE_MISC, 1);

	poOrnate->Add(std::make_shared<ItemGeneric>(oOrb));
}

sptr_ItemGeneric ItemMap::GenerateVeryCommon(LayoutMap& oLayoutMap, 
											 UnitMap& oUnitMap,
											 std::default_random_engine& oRNG) const {
	std::uniform_int_distribution<size_t> oDistributionIndex(0, 2);
	const pair_size_t& TileXY(oLayoutMap.GetPassableRandom(oRNG));

	switch (oDistributionIndex(oRNG)) {
		case 0:
			return std::make_shared<ItemGeneric>("stick.png", TileXY.first, 
												 TileXY.second, MAT_WOOD, 
												 "Stick", 1, 2.0, 
												 ItemGeneric::TYPE_MISC, 1);
		case 1: 
			return std::make_shared<ItemGeneric>("plank.png", TileXY.first, 
												 TileXY.second, MAT_WOOD, 
												 "Plank", 1, 5.0, 
												 ItemGeneric::TYPE_MISC, 1);
		case 2:
			return std::make_shared<ItemGeneric>("stone.png", TileXY.first, 
												 TileXY.second, MAT_STONE, 
												 "Stone", 1, 0.2, 
												 ItemGeneric::TYPE_MISC, 1);
	}
	return nullptr;
}

sptr_ItemGeneric ItemMap::GenerateCommon(LayoutMap& oLayoutMap, 
										 UnitMap& oUnitMap, 
										 std::default_random_engine& oRNG) const {
	std::uniform_int_distribution<size_t> oDistributionIndex(0, 3);
	const pair_size_t& TileXY(oLayoutMap.GetPassableRandom(oRNG));

	switch (oDistributionIndex(oRNG)) {
		case 0: 
			return std::make_shared<ItemGeneric>("coin.png", TileXY.first, 
												 TileXY.second, MAT_METAL, 
												 "Coin", 1, 0.01, 
												 ItemGeneric::TYPE_MISC, 1);
		case 1:
			return std::make_shared<ConsumableApple>(TileXY.first, 
													 TileXY.second);
		case 2:
			return std::make_shared<ConsumableKey>("key.png", TileXY.first, 
												   TileXY.second, MAT_METAL,
												   "Key", 1, 0.1, oUnitMap, 
												   true, false);
		case 3:
			return std::make_shared<ConsumableWand>("wand.png", TileXY.first, 
													TileXY.second, MAT_WOOD, 
													"Wand of wall breaking",
													1, 0.3, oLayoutMap);
	}
	return nullptr;
}

sptr_ItemGeneric ItemMap::GenerateNormal(LayoutMap& oLayoutMap, 
										 UnitMap& oUnitMap,
										 std::default_random_engine& oRNG) const {
	std::uniform_int_distribution<size_t> oDistributionIndex(0, 4);
	const pair_size_t& TileXY(oLayoutMap.GetPassableRandom(oRNG));

	switch (oDistributionIndex(oRNG)) {
		case 0: {
			const EquipmentGeneric oNew("boot_leather_l.png", TileXY.first, 
										TileXY.second, MAT_CLOTH, 
										"Leather boot (L)", 5, 1.0, SLOT_LBOOT, 
										true, 2);

			return std::make_shared<EquipmentGeneric>(oNew);
		}
		case 1: {
			const EquipmentGeneric oNew("boot_leather_r.png", TileXY.first, 
										TileXY.second, MAT_CLOTH, 
										"Leather boot (R)", 5, 1.0, SLOT_RBOOT, 
										true, 2);
			
			return std::make_shared<EquipmentGeneric>(oNew);
		}
		case 2: {
			const EquipmentGeneric oNew("boot_iron_l.png", TileXY.first, 
										TileXY.second, MAT_METAL, 
										"Iron boot (L)", 10, 3.0, SLOT_LBOOT, 
										true, 5);
			
			return std::make_shared<EquipmentGeneric>(oNew);
		}
		case 3: {
			const EquipmentGeneric oNew("boot_iron_r.png", TileXY.first, 
										TileXY.second, MAT_METAL, 
										"Iron boot (R)", 10, 3.0, SLOT_RBOOT, 
										true, 5);
			
			return std::make_shared<EquipmentGeneric>(oNew);
		}
		case 4: {
			const EquipmentGeneric oNew("hatchet.png", TileXY.first, 
										TileXY.second, MAT_METAL, "Hatchet", 
										100, 2.5, SLOT_RHAND, true, 0, 10, 0);
			
			return std::make_shared<EquipmentGeneric>(oNew);
		}
	}
	return nullptr;
}

void ItemMap::AddGoalKey(LayoutMap& oLayoutMap, UnitMap& oUnitMap,
						 std::default_random_engine& oRNG) {
	const ConsumableKey oKey("key_special.png", 0, 0, MAT_METAL, "Unusual key", 
							 1, 0.1, oUnitMap, false, true);
	const auto poKey(std::make_shared<ConsumableKey>(oKey));

	if (std::bernoulli_distribution()(oRNG) &&
		oUnitMap.GetContainers().size()) {
		AddToRandomContainer(oUnitMap, poKey, oRNG);
		return;
	}

	const uset_pair_size_t& setPassables(oLayoutMap.GetPassableAll());
	const csptr_HumanoidGeneric poPlayer(oUnitMap.GetPlayer());
	const std::unordered_map<short, uset_pair_size_t>& msetFOV(poPlayer->GetFOV());
	uset_pair_size_t setFOV;
	uset_pair_size_t setNotInFOV(oLayoutMap.GetPassableAll()); //a copy

	for (const std::pair<short, uset_pair_size_t>& KeyValue : msetFOV) {
		const uset_pair_size_t& setPartFOV(KeyValue.second);

		setFOV.insert(setPartFOV.cbegin(), setPartFOV.cend());
	}
	for (auto Itr(setNotInFOV.begin()); Itr != setNotInFOV.end(); )
		Itr = setFOV.count(*Itr) ? setNotInFOV.erase(Itr) : std::next(Itr);

	if (setNotInFOV.size())
		poKey->SetCoordinates(GetRandomElement<pair_size_t>(setNotInFOV, 
															oRNG));
	else
		poKey->SetCoordinates(GetRandomElement<pair_size_t>(setPassables, 
															oRNG));
	Add(poKey, false);
}