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

#ifndef NOTAGAME_UNIT_GENERIC_H
#define NOTAGAME_UNIT_GENERIC_H

#include "StandingGeneric.h"
#include "utility.h" //for GetStepX()

#include <string>
#include <random> //for std::default_random_engine
#include <unordered_map>
#include <unordered_set>
#include <utility> //for std::pair
#include <stdexcept>

class UnitGeneric: public StandingGeneric { //any creature
public:
	enum enumStatusEffect: unsigned char {STATUS_BLIND = 0, STATUS_NONE};
    UnitGeneric(const std::string&, size_t, size_t, enumMaterial, 
				const std::string&, size_t, float, enumDirection, bool, size_t, 
				size_t, size_t, bool);
	void InitialiseFOV();
	void AddToFOV(short, pair_size_t);
	void AddToFOV(short, size_t, size_t);
	void ClearPartFOV(short);
	size_t GetBaseDistanceFOV() const;
	void SetDistanceFOV(size_t);
    size_t GetStrength() const;
    virtual void SetStrength(size_t);
    size_t GetIntelligence() const;
    void SetIntelligence(size_t);
	bool IsCapableOfAdvancedManipulations() const;
	void SetCapableOfAdvancedManipulations(bool);
    short Hit(sptr_StandingGeneric, std::default_random_engine&) const;
	std::unordered_map<short, uset_pair_size_t>& GetFOV(); //field of vision
	const std::unordered_map<short, uset_pair_size_t>& GetFOV() const;
	size_t GetBuffTurns(enumStatusEffect) const;
	void AddBuff(enumStatusEffect, size_t);
	void RemoveBuff(enumStatusEffect);
	void TickBuffs();
private:
	const std::unordered_map<enumStatusEffect, size_t>& GetBuffs() const;
	std::unordered_map<enumStatusEffect, size_t>& GetBuffs();

	size_t m_unBaseDistanceFOV; //orthogonal, not diagonals
    size_t m_unStrength;
    size_t m_unIntelligence;
	bool m_bCapableOfAdvancedManipulations; //opening doors, using wands
    std::unordered_map<short, uset_pair_size_t> m_msetTilesXYInView;
	std::unordered_map<enumStatusEffect, size_t> m_munStatusEffects;
};

inline void UnitGeneric::AddToFOV(const short nIndex, const pair_size_t XY) {
	GetFOV().at(nIndex).emplace(XY);
}

inline void UnitGeneric::AddToFOV(const short nIndex, size_t unX, 
								  size_t unY) {
	AddToFOV(nIndex, std::make_pair(unX, unY));
}

inline void UnitGeneric::ClearPartFOV(const short nIndex) {
	GetFOV().at(nIndex).clear();
}

inline std::unordered_map<short, uset_pair_size_t>& UnitGeneric::GetFOV() { 
	return m_msetTilesXYInView;
}

inline const std::unordered_map<short, uset_pair_size_t>& UnitGeneric::GetFOV() const { 
	return m_msetTilesXYInView;
}

inline size_t UnitGeneric::GetBaseDistanceFOV() const {
	return GetBuffTurns(STATUS_BLIND) ? 1 : m_unBaseDistanceFOV;
}

inline void UnitGeneric::SetDistanceFOV(const size_t unBaseDistanceFOV) {
	m_unBaseDistanceFOV = unBaseDistanceFOV;
}

inline size_t UnitGeneric::GetStrength() const { 
	return m_unStrength; 
}

inline void UnitGeneric::SetStrength(const size_t unStrength) { 
	m_unStrength = unStrength; 
}

inline size_t UnitGeneric::GetIntelligence() const { 
	return m_unIntelligence; 
}

inline void UnitGeneric::SetIntelligence(const size_t unIntelligence) { 
	m_unIntelligence = unIntelligence; 
}

inline bool UnitGeneric::IsCapableOfAdvancedManipulations() const {
	return m_bCapableOfAdvancedManipulations;
}

inline void UnitGeneric::SetCapableOfAdvancedManipulations(const bool bCapable) {
	m_bCapableOfAdvancedManipulations = bCapable;
}

inline const std::unordered_map<UnitGeneric::enumStatusEffect, size_t>& UnitGeneric::GetBuffs() const {
	return m_munStatusEffects;
}

inline std::unordered_map<UnitGeneric::enumStatusEffect, size_t>& UnitGeneric::GetBuffs() {
	return m_munStatusEffects;
}

inline size_t UnitGeneric::GetBuffTurns(const enumStatusEffect eEffect) const {
	return GetBuffs().count(eEffect) ? GetBuffs().at(eEffect) : 0;
}

inline void UnitGeneric::RemoveBuff(const enumStatusEffect eEffect) {
	GetBuffs().erase(eEffect);
}

#endif