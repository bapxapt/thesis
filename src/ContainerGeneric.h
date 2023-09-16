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

#ifndef NOTAGAME_CONTAINER_GENERIC_H
#define NOTAGAME_CONTAINER_GENERIC_H

#include "FurnitureGeneric.h"
#include "ItemGeneric.h"

#include <vector>
#include <algorithm>

class ContainerGeneric: public FurnitureGeneric {
public:
    ContainerGeneric(const std::string&, size_t, size_t, enumMaterial, 
					 const std::string&, size_t, float, enumDirection, bool, 
					 bool, bool);
	template<typename T> void Add(std::shared_ptr<T>);
	template<typename F> bool Contains(const F&);
	csptr_ItemGeneric Get(size_t) const;
	sptr_ItemGeneric Get(size_t);
	size_t Count() const;
	void Clear();
    bool IsLocked() const;
    void SetLocked(bool);
private:
	const std::vector<sptr_ItemGeneric>& GetAll() const;
	std::vector<sptr_ItemGeneric>& GetAll();

    bool m_bLocked;
    std::vector<sptr_ItemGeneric> m_vpoContents;
};

template<typename T> 
void ContainerGeneric::Add(const std::shared_ptr<T> poItem) {
	GetAll().push_back(poItem);
	SetWeight(GetWeight() + poItem->GetWeight());
}

template<typename F>
bool ContainerGeneric::Contains(const F& Lambda) {
	const std::vector<sptr_ItemGeneric>& vpoContents(GetAll());
	const auto FoundItr(std::find_if(vpoContents.cbegin(), vpoContents.cend(), 
						Lambda));

	return FoundItr != vpoContents.cend();
}

inline const std::vector<sptr_ItemGeneric>& ContainerGeneric::GetAll() const {
	return m_vpoContents; 
}

inline std::vector<sptr_ItemGeneric>& ContainerGeneric::GetAll() {
	return m_vpoContents; 
}

inline csptr_ItemGeneric ContainerGeneric::Get(const size_t unIndex) const {
	return GetAll().at(unIndex);
}

inline sptr_ItemGeneric ContainerGeneric::Get(const size_t unIndex) {
	return GetAll().at(unIndex);
}

inline size_t ContainerGeneric::Count() const {
	return GetAll().size();
}

inline bool ContainerGeneric::IsLocked() const { 
	return m_bLocked; 
}

inline void ContainerGeneric::SetLocked(bool bLocked) { 
	m_bLocked = bLocked; 
}

#endif