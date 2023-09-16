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

#ifndef NOTAGAME_LAYOUT_MAP_H
#define NOTAGAME_LAYOUT_MAP_H

#include "FieldObjectVector2D.h"
#include "FloorGeneric.h"
#include "WallGeneric.h"
#include "WaterGeneric.h"
#include "Camera2D.h"
#include "utility.h"

#include "ofImage.h"

#include <random> //for std::default_random_engine
#include <stdexcept>
#include <utility> //for std::pair
#include <unordered_map>
#include <unordered_set>

class LayoutMap: public FieldObjectVector2D<LayoutGeneric> {
public:
	LayoutMap(size_t, size_t);
	void Clear() override;
    void PassableAdd(const pair_size_t&);
	void PassableRemove(size_t, size_t);
    void PassableRemove(const pair_size_t&);
	size_t PassableCount() const;
	pair_size_t GetRandom(std::default_random_engine&, bool = false) const;
	const pair_size_t& GetPassableRandom(std::default_random_engine&) const;
	const uset_pair_size_t& GetPassableAll() const;
	uset_pair_size_t& GetPassableAll();
	void MarkAsNotInFOV(const std::unordered_map<short, uset_pair_size_t>&);
	bool IsFloor(csptr_LayoutGeneric) const;
    bool IsWall(csptr_LayoutGeneric) const;
    bool IsWater(csptr_LayoutGeneric) const;
    bool IsBorder(csptr_LayoutGeneric) const;
    void Generate(std::default_random_engine&);
    void Update(std::default_random_engine&, size_t, size_t);
	void Replace(const FloorGeneric&, std::default_random_engine&, 
				 bool = true);
	void Replace(const WallGeneric&, std::default_random_engine&,
				 bool = true);
	void Replace(const WaterGeneric&, std::default_random_engine&,
				 bool = true);
private:
	bool Replace(const LayoutGeneric&, bool);
	void UpdatePerceivedWall(sptr_LayoutGeneric, csptr_LayoutGeneric);
	void UpdatePerceivedWater(sptr_LayoutGeneric, csptr_LayoutGeneric);
	void RandomiseTileOffsetX(sptr_LayoutGeneric, std::default_random_engine&);
	std::shared_ptr<const ofImage> GetSpriteFloor() const;
	std::shared_ptr<const ofImage> GetSpriteWall() const;
	std::shared_ptr<const ofImage> GetSpriteWater() const;
	size_t CountTileVariants(std::shared_ptr<const ofImage>) const;
	void Fill();
    void GeneratePassages(std::default_random_engine&);
    void GenerateRooms(std::default_random_engine&);
    void GenerateWater(std::default_random_engine&);
    void ExpandWater(size_t, size_t, std::default_random_engine&, float = 1.0);

    uset_pair_size_t m_setPassableTilesXY; //passable and without units
};

inline void LayoutMap::PassableRemove(const pair_size_t& TileXY) {
    GetPassableAll().erase(TileXY); //throws an exception if not found
}

inline void LayoutMap::PassableRemove(const size_t unX, const size_t unY) { 
	return PassableRemove(std::make_pair(unX, unY)); 
}

inline size_t LayoutMap::PassableCount() const {
	return GetPassableAll().size();
}

inline const pair_size_t& LayoutMap::GetPassableRandom(std::default_random_engine& oRNG) const {
	return GetRandomElement<pair_size_t>(GetPassableAll(), oRNG);
}

inline uset_pair_size_t& LayoutMap::GetPassableAll() {
	return m_setPassableTilesXY;
}

inline const uset_pair_size_t& LayoutMap::GetPassableAll() const { 
	return m_setPassableTilesXY; 
}

inline bool LayoutMap::IsFloor(const csptr_LayoutGeneric poTile) const {
	return poTile->GetSprite() == GetSpriteFloor();
}

inline bool LayoutMap::IsWall(const csptr_LayoutGeneric poTile) const {
	return poTile->GetSprite() == GetSpriteWall();
}

inline bool LayoutMap::IsWater(const csptr_LayoutGeneric poTile) const {
	return poTile->GetSprite() == GetSpriteWater();
}

//only horizontally (X)
inline size_t LayoutMap::CountTileVariants(const std::shared_ptr<const ofImage> poImage) const {
	return poImage->getWidth() / SPRITE_W;
}

inline std::shared_ptr<const ofImage> LayoutMap::GetSpriteFloor() const {
	return GetSprite(GetSpriteFullPath("floor.png"));
}

inline std::shared_ptr<const ofImage> LayoutMap::GetSpriteWall() const {
	return GetSprite(GetSpriteFullPath("wall.png"));
}

inline std::shared_ptr<const ofImage> LayoutMap::GetSpriteWater() const {
	return GetSprite(GetSpriteFullPath("water.png"));
}

#endif