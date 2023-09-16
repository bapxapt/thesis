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

#ifndef NOTAGAME_DEFINE_H
#define NOTAGAME_DEFINE_H

#include <memory> //for std::shared_ptr
#include <unordered_set>

#define SPRITE_W 48 //sprite (tile) width in pixels
#define SPRITE_H 48 //sprite (tile) height in pixels

#define LOG_ENTRIES 10 //number of text lines stored to display in log window (UserInterface::GameField)

#define SLOT_N_BACKPACK 20 //maximal amount of inventory slots
#define SLOT_N_EQUIPMENT 12 //maximal amount of equipment slots

class DrawableGeneric;
class StandingGeneric; //forward declarations for typedefs that are below
class LyingGeneric;
class LayoutGeneric;
class FurnitureGeneric;
class ContainerGeneric;
class UnitGeneric;
class HumanoidGeneric;
class PlayerGeneric;
class ItemGeneric;
class EquipmentGeneric;
class ConsumableGeneric;
class TrapGeneric;

typedef std::shared_ptr<DrawableGeneric> sptr_DrawableGeneric;
typedef std::shared_ptr<StandingGeneric> sptr_StandingGeneric;
typedef std::shared_ptr<LyingGeneric> sptr_LyingGeneric;
typedef std::shared_ptr<LayoutGeneric> sptr_LayoutGeneric;
typedef std::shared_ptr<FurnitureGeneric> sptr_FurnitureGeneric;
typedef std::shared_ptr<ContainerGeneric> sptr_ContainerGeneric;
typedef std::shared_ptr<UnitGeneric> sptr_UnitGeneric;
typedef std::shared_ptr<HumanoidGeneric> sptr_HumanoidGeneric;
typedef std::shared_ptr<PlayerGeneric> sptr_PlayerGeneric;
typedef std::shared_ptr<ItemGeneric> sptr_ItemGeneric;
typedef std::shared_ptr<EquipmentGeneric> sptr_EquipmentGeneric;
typedef std::shared_ptr<ConsumableGeneric> sptr_ConsumableGeneric;
typedef std::shared_ptr<TrapGeneric> sptr_TrapGeneric;
typedef std::shared_ptr<const DrawableGeneric> csptr_DrawableGeneric;
typedef std::shared_ptr<const StandingGeneric> csptr_StandingGeneric;
typedef std::shared_ptr<const LyingGeneric> csptr_LyingGeneric;
typedef std::shared_ptr<const LayoutGeneric> csptr_LayoutGeneric;
typedef std::shared_ptr<const FurnitureGeneric> csptr_FurnitureGeneric;
typedef std::shared_ptr<const ContainerGeneric> csptr_ContainerGeneric;
typedef std::shared_ptr<const UnitGeneric> csptr_UnitGeneric;
typedef std::shared_ptr<const HumanoidGeneric> csptr_HumanoidGeneric;
typedef std::shared_ptr<const PlayerGeneric> csptr_PlayerGeneric;
typedef std::shared_ptr<const ItemGeneric> csptr_ItemGeneric;
typedef std::shared_ptr<const EquipmentGeneric> csptr_EquipmentGeneric;
typedef std::shared_ptr<const ConsumableGeneric> csptr_ConsumableGeneric;
typedef std::shared_ptr<const TrapGeneric> csptr_TrapGeneric;

enum enumDirection: unsigned char {DIR_NORTH = 0, DIR_NE, DIR_EAST, DIR_SE,
								   DIR_SOUTH, DIR_SW, DIR_WEST, DIR_NW, 
								   DIR_NONE}; //clockwise 
enum enumMaterial: unsigned char {MAT_FLESH = 0, MAT_CLOTH, MAT_WOOD, MAT_GAS,
								  MAT_GLASS, MAT_STONE, MAT_METAL, MAT_LIQUID};
enum enumEquipmentSlot: unsigned char {SLOT_LRING = 0, SLOT_LHAND, SLOT_LGLOVE,
									   SLOT_LBOOT, SLOT_NECK, SLOT_HEAD, 
									   SLOT_BODY, SLOT_PANTS, SLOT_RRING, 
									   SLOT_RHAND, SLOT_RGLOVE, SLOT_RBOOT,
									   SLOT_NONE};

typedef std::pair<size_t, size_t> pair_size_t;
typedef std::pair<long long, long long> pair_long_long;
typedef std::pair<int, int> pair_int;
typedef std::pair<float, float> pair_float;
typedef std::unordered_set<pair_size_t> uset_pair_size_t;

namespace std { //for std::unordered_set<std::pair<>>
	template<typename T> 
	struct hash<std::pair<T, T>> {
        inline size_t operator()(const std::pair<T, T>& ToHash) const { 
			return std::hash<T>()(ToHash.first) ^ 
				   std::hash<T>()(ToHash.second);
		}
    };
}

#endif