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

#ifndef NOTAGAME_UNIT_MAP_H
#define NOTAGAME_UNIT_MAP_H

#include "LayoutMap.h"
#include "CalculatorFOV.h"
#include "FieldObjectMap.h"
#include "ContainerGeneric.h"
#include "HumanoidGeneric.h"
#include "define.h"

#include <memory> //for std::shared_ptr
#include <random> //for std::default_random_engine

class UnitMap: public FieldObjectMap<StandingGeneric> {
public:
    UnitMap(LayoutMap& oLayoutMap);
	void Clear() override;
	template<typename T> std::shared_ptr<T> Add(const T&, LayoutMap&, 
												bool = true);
	const pair_size_t Generate(uset_pair_size_t&, LayoutMap&, 
							   std::default_random_engine&);
	csptr_HumanoidGeneric GetPlayer() const;
    sptr_HumanoidGeneric GetPlayer();
	bool IsPlayer(csptr_StandingGeneric) const;
	const CalculatorFOV& GetCalculatorFOV() const;
	void Remove(const pair_size_t&) override;
	const std::unordered_set<sptr_ContainerGeneric>& GetContainers() const;
	std::unordered_set<sptr_ContainerGeneric>& GetContainers();
private:
	void GenerateFurniture(uset_pair_size_t&, LayoutMap&, 
						   std::default_random_engine&);
	void GenerateOpponents(LayoutMap&, std::default_random_engine&);
	void SetPlayer(sptr_HumanoidGeneric);

    sptr_HumanoidGeneric m_poPlayer;
	const CalculatorFOV m_oCalculatorFOV;
	std::unordered_set<sptr_ContainerGeneric> m_setpoContainers; //no goal
};

inline csptr_HumanoidGeneric UnitMap::GetPlayer() const { 
	return m_poPlayer;
}

inline sptr_HumanoidGeneric UnitMap::GetPlayer() { 
	return m_poPlayer;
}

inline bool UnitMap::IsPlayer(const csptr_StandingGeneric poUnit) const { 
	return poUnit == GetPlayer(); 
}

inline void UnitMap::SetPlayer(const sptr_HumanoidGeneric poPlayer) {
	m_poPlayer = poPlayer;
}

inline const CalculatorFOV& UnitMap::GetCalculatorFOV() const {
	return m_oCalculatorFOV;
}

inline const std::unordered_set<sptr_ContainerGeneric>& UnitMap::GetContainers() const {
	return m_setpoContainers;
}

inline std::unordered_set<sptr_ContainerGeneric>& UnitMap::GetContainers() {
	return m_setpoContainers;
}

#endif