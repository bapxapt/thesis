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

#ifndef NOTAGAME_TRAP_MAP_H
#define NOTAGAME_TRAP_MAP_H

#include "FieldObjectMap.h"
#include "TrapDamaging.h"
#include "TrapBlinding.h"
#include "utility.h"

#include "ofUtils.h" //for ofToString()

#include <memory> //for std::shared_ptr
#include <random> //for std::default_random_engine
#include <unordered_set>

class TrapMap: public FieldObjectMap<TrapGeneric> {
public:
    TrapMap();
	void Clear() override;
	template<typename T> void Add(const T&, bool = true);
	void Generate(const uset_pair_size_t&, UnitMap&,  
				  std::default_random_engine&);
	void TrappableAdd(const pair_size_t&);
	const pair_size_t& GetTrappableRandom(std::default_random_engine&) const;
private:
	const uset_pair_size_t& GetTrappableAll() const;
	uset_pair_size_t& GetTrappableAll();
	void TrappableRemove(const pair_size_t&);

    uset_pair_size_t m_setTrappableTilesXY; //passable and not trapped yet
};

template<typename T> 
void TrapMap::Add(const T& oNew, const bool bUseTexture) {
	const std::shared_ptr<T> poNew(std::make_shared<T>(oNew));

	TrappableRemove(poNew->GetCoordinates());
	FieldObjectMap<TrapGeneric>::Add(poNew, bUseTexture);
}

inline const uset_pair_size_t& TrapMap::GetTrappableAll() const {
	return m_setTrappableTilesXY;
}

inline uset_pair_size_t& TrapMap::GetTrappableAll() {
	return m_setTrappableTilesXY;
}

inline void TrapMap::TrappableRemove(const pair_size_t& TileXY) {
	GetTrappableAll().erase(TileXY); //throws an exception if not found
}

inline const pair_size_t& TrapMap::GetTrappableRandom(std::default_random_engine& oRNG) const {
	return GetRandomElement<pair_size_t>(GetTrappableAll(), oRNG);
}

#endif