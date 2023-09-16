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

#ifndef NOTAGAME_ITEM_MAP_H
#define NOTAGAME_ITEM_MAP_H

#include "Camera2D.h"
#include "FieldObjectMultimap.h"
#include "ConsumableKey.h"
#include "ConsumableApple.h"
#include "ConsumableWand.h"
#include "ContainerGeneric.h"
#include "utility.h" //for GetRandom
#include "define.h"

#include <algorithm> //for std::set_difference
#include <vector>
#include <memory> //for std::shared_ptr
#include <random> //for std::default_random_engine
#include <stdexcept>

class ItemMap: public FieldObjectMultimap<ItemGeneric> {
	typedef std::unordered_set<sptr_ItemGeneric> uset_sptr_ItemGeneric;
public:
    ItemMap();
	template<typename T> void Add(std::shared_ptr<T>, bool = true);
	template<typename T> void Add(const T&, bool = true);
	void Generate(const uset_pair_size_t&, const pair_size_t&, LayoutMap&, 
				  UnitMap&, std::default_random_engine&);
	void AddGoalKey(LayoutMap&, UnitMap&, std::default_random_engine&);
private:
	void DivideItemsByRarity(std::vector<size_t>&, size_t, 
							 std::default_random_engine&) const;
	void GenerateOnFloor(LayoutMap&, UnitMap&, 
						 std::default_random_engine&);
	void GenerateInUnits(LayoutMap&, UnitMap&, 
						 std::default_random_engine&) const;
	sptr_ItemGeneric GenerateVeryCommon(LayoutMap&, UnitMap&, 
										std::default_random_engine&) const;
	sptr_ItemGeneric GenerateCommon(LayoutMap&, UnitMap&, 
									std::default_random_engine&) const;
	sptr_ItemGeneric GenerateNormal(LayoutMap&, UnitMap&, 
									std::default_random_engine&) const;
	void AddToRandomContainer(UnitMap&, sptr_ItemGeneric, 
							  std::default_random_engine&) const;
};

template<typename T> 
void ItemMap::Add(const std::shared_ptr<T> poNew, const bool bUseTexture) {
	if (!poNew->IsStackable()) { //not stackable - add in any case
		FieldObjectMultimap<ItemGeneric>::Add(poNew, bUseTexture);
		return;
	}

	const pair_size_t& TileXY(poNew->GetCoordinates());
	const sptr_ItemGeneric poExisting(Get(TileXY, poNew));

	if (poExisting) //stack it (increase the quantity)
		poExisting->SetQuantity(poExisting->GetQuantity() + 
								poNew->GetQuantity());
	else //stackable, but no existing item to stack with
		FieldObjectMultimap<ItemGeneric>::Add(poNew, bUseTexture);
}

template<typename T> 
inline void ItemMap::Add(const T& oNew, const bool bUseTexture) {
	Add(std::make_shared<T>(oNew), bUseTexture);
}

inline void ItemMap::AddToRandomContainer(UnitMap& oUnitMap,
										  const sptr_ItemGeneric poItem,
										  std::default_random_engine& oRNG) const {
	GetRandomElement<sptr_ContainerGeneric>(oUnitMap.GetContainers(), 
											oRNG)->Add(poItem);
}

#endif