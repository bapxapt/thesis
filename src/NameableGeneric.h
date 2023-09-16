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

#ifndef NOTAGAME_NAMEABLE_GENERIC_H
#define NOTAGAME_NAMEABLE_GENERIC_H

#include "FieldObjectGeneric.h"

#include <memory> //for std::shared_ptr
#include <string>
#include <utility> //for std::pair
#include <regex> //for std::regex
#include <stdexcept>

class NameableGeneric: public FieldObjectGeneric { //anything with HP (non-layout)
public:
	NameableGeneric(const std::string&, size_t, size_t, enumMaterial, 
					const std::string&, size_t, float);
	bool operator==(const FieldObjectGeneric&) const = delete;
	bool operator==(const NameableGeneric&) const;
    bool IsPerceivedAsDamaged() const;
    const std::string& GetName() const;
	void SetName(const std::string&);
    const pair_size_t& GetHP() const;
	size_t GetHPCurrent() const;
	void SetHPCurrent(long long);
	size_t GetHPMaximal() const;
    void SetHPMaximal(size_t);
	float GetWeight() const;
	void SetWeight(float);
	const pair_size_t& GetPerceivedHP() const;
	size_t GetPerceivedHPCurrent() const;
	size_t GetPerceivedHPMaximal() const;
	virtual void Update() override;
	void UpdatePerceivedHP();
private:
    std::string m_strName;
    pair_size_t m_HP; //.second is maximal
	pair_size_t m_PerceivedHP;
	float m_fWeight;
};

inline bool NameableGeneric::operator==(const NameableGeneric& oOther) const {
	return FieldObjectGeneric::operator==(oOther) &&
		   GetName() == oOther.GetName() &&
		   GetHP() == oOther.GetHP() &&
		   GetWeight() == oOther.GetWeight();
}

inline bool NameableGeneric::IsPerceivedAsDamaged() const { 
	return GetPerceivedHPCurrent() < GetPerceivedHPMaximal(); 
}

inline const pair_size_t& NameableGeneric::GetHP() const {
	return m_HP;
}

inline size_t NameableGeneric::GetHPCurrent() const {
	return m_HP.first; 
}

inline size_t NameableGeneric::GetHPMaximal() const {
	return m_HP.second; 
} 

inline const std::string& NameableGeneric::GetName() const { 
	return m_strName; 
}

inline void NameableGeneric::SetName(const std::string& strNewName) { 
	m_strName = strNewName; 
}

inline float NameableGeneric::GetWeight() const { 
	return m_fWeight; 
}

inline void NameableGeneric::SetWeight(const float fWeight) {
	m_fWeight = fWeight;
}

inline void NameableGeneric::SetHPCurrent(const long long nHP) { 
	m_HP.first = nHP < 0 ? 0 : nHP;
}

inline void NameableGeneric::SetHPMaximal(const size_t unHPMax) { 
	m_HP.second = unHPMax; 
}

inline const pair_size_t& NameableGeneric::GetPerceivedHP() const {
	return m_PerceivedHP;
}

inline size_t NameableGeneric::GetPerceivedHPCurrent() const {
	return m_PerceivedHP.first;
}

inline size_t NameableGeneric::GetPerceivedHPMaximal() const {
	return m_PerceivedHP.second;
}

inline void NameableGeneric::UpdatePerceivedHP() {
	m_PerceivedHP = GetHP();
}

#endif