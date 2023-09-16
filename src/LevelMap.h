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

#ifndef NOTAGAME_LEVEL_MAP_H
#define NOTAGAME_LEVEL_MAP_H

#include "ContainerGeneric.h"
#include "LayoutMap.h"
#include "UnitMap.h"
#include "TrapMap.h"
#include "ItemMap.h"
#include "ConsumableGeneric.h"
#include "GameFieldUI.h"
#include "define.h"

#include "ofImage.h"

#include <memory> //for std::shared_ptr
#include <random> //for std::default_random_engine
#include <stdexcept>
#include <string>
#include <utility> //for std::pair
#include <unordered_set>
#include <functional>

class LevelMap { //manages maps and their interactions
public:
    LevelMap(size_t, size_t, GameFieldUI&);
	void Draw(const Camera2D&) const;
    void Generate();
	void Clear();
	void UpdateTextures();
    ItemMap& GetItemMap();
    const ItemMap& GetItemMap() const;
    TrapMap& GetTrapMap();
    const TrapMap& GetTrapMap() const;
    LayoutMap& GetLayoutMap();
    const LayoutMap& GetLayoutMap() const;
	UnitMap& GetUnitMap();
	const UnitMap& GetUnitMap() const;
    std::default_random_engine& GetEngineRNG();
    void PlayerHit();
    short PlayerConsume(size_t);
    short PlayerEquip(size_t);
    void PlayerLoot();
    bool PlayerMove(const pair_size_t&);
    void PlayerPush();
    short PlayerPickUp();
    void PlayerTurn(enumDirection);
    short PlayerUnequip(size_t);
	void PlayerDrop(size_t);
private:
	short UnitMove(sptr_StandingGeneric, const pair_size_t&);
	void UnitRemove(sptr_StandingGeneric);
	short UnitHit(csptr_UnitGeneric);
	short UnitLoot(csptr_HumanoidGeneric);
    short UnitPush(sptr_UnitGeneric);
    short UnitPickUp(sptr_HumanoidGeneric);
    void UnitTurn(sptr_UnitGeneric, enumDirection);
	void UnitDrop(sptr_HumanoidGeneric, size_t);
	void UnitDropRemains(sptr_StandingGeneric);
	void UpdateFromFOV();
	void UpdateFromFOV(sptr_HumanoidGeneric);
    void DrawFog(const Camera2D&) const;
	GameFieldUI& GetGameFieldUI();

	std::default_random_engine m_oRNG;
    LayoutMap m_oLayoutMap;
	UnitMap m_oUnitMap;
    ItemMap m_oItemMap;
	TrapMap m_oTrapMap;
	GameFieldUI& m_oGameFieldUI;
};

inline LayoutMap& LevelMap::GetLayoutMap() {
	return m_oLayoutMap;
}

inline const LayoutMap& LevelMap::GetLayoutMap() const {
	return m_oLayoutMap;
}

inline ItemMap& LevelMap::GetItemMap() { 
	return m_oItemMap; 
}

inline const ItemMap& LevelMap::GetItemMap() const { 
	return m_oItemMap; 
}

inline TrapMap& LevelMap::GetTrapMap() {
	return m_oTrapMap;
}

inline const TrapMap& LevelMap::GetTrapMap() const {
	return m_oTrapMap;
}

inline UnitMap& LevelMap::GetUnitMap() {
	return m_oUnitMap;
}

inline const UnitMap& LevelMap::GetUnitMap() const {
	return m_oUnitMap;
}

inline std::default_random_engine& LevelMap::GetEngineRNG() { 
	return m_oRNG; 
}

inline GameFieldUI& LevelMap::GetGameFieldUI() {
	return m_oGameFieldUI;
}

inline void LevelMap::UpdateFromFOV() {
	UpdateFromFOV(GetUnitMap().GetPlayer());
}

#endif