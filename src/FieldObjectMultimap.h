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

#ifndef NOTAGAME_FIELD_OBJECT_MULTIMAP_H
#define NOTAGAME_FIELD_OBJECT_MULTIMAP_H

#include "Camera2D.h"
#include "SpriteMap.h"
#include "ItemGeneric.h" //for a forward declaration in .cpp

#include <utility> //for std::pair
#include <algorithm> //for std::find

template<class T> 
class FieldObjectMultimap: public SpriteMap {
	typedef std::unordered_set<std::shared_ptr<T>> uset_sptr_T;
public:
    FieldObjectMultimap();
	virtual void Clear();
	size_t Count(const pair_size_t&) const;
	size_t Count(size_t, size_t) const;
	std::shared_ptr<T> Get(const pair_size_t&, std::shared_ptr<const T>);
	std::shared_ptr<const T> GetFirst(const pair_size_t&) const;
	std::shared_ptr<T> GetFirst(const pair_size_t&);
	const uset_sptr_T& Get(const pair_size_t&) const;
	uset_sptr_T& Get(const pair_size_t&);
	const uset_sptr_T& Get(size_t, size_t) const;
	uset_sptr_T& Get(size_t, size_t);
	void Add(std::shared_ptr<T>, bool);
	void Draw(const Camera2D&) const;
    void Remove(std::shared_ptr<T>);
	const std::unordered_map<pair_size_t, uset_sptr_T>& GetAll() const;
private:
	std::unordered_map<pair_size_t, uset_sptr_T>& GetAll();

    std::unordered_map<pair_size_t, uset_sptr_T> m_msetpoObjects;
};

template<class T>
inline void FieldObjectMultimap<T>::Clear() {
	GetAll().clear();
}

template<class T>
inline size_t FieldObjectMultimap<T>::Count(const pair_size_t& XY) const {
	return GetAll().count(XY) ? Get(XY).size() : 0;
}

template<class T>
inline size_t FieldObjectMultimap<T>::Count(const size_t unX, 
											const size_t unY) const {
	return Count(std::make_pair(unX, unY));
}

template<class T>
inline std::shared_ptr<const T> FieldObjectMultimap<T>::GetFirst(const pair_size_t& XY) const {
	return Count(XY) ? *Get(XY).cbegin() : nullptr;
}

template<class T>
inline std::shared_ptr<T> FieldObjectMultimap<T>::GetFirst(const pair_size_t& XY) {
	return Count(XY) ? *Get(XY).begin() : nullptr;
}

template<class T>
inline const std::unordered_set<std::shared_ptr<T>>& FieldObjectMultimap<T>::Get(const pair_size_t& XY) const {
	return GetAll().at(XY);
}

template<class T>
inline std::unordered_set<std::shared_ptr<T>>& FieldObjectMultimap<T>::Get(const pair_size_t& XY) {
	return GetAll().at(XY);
}

template<class T>
inline const std::unordered_set<std::shared_ptr<T>>& FieldObjectMultimap<T>::Get(const size_t unX, 
																				 const size_t unY) const {
	return Get(std::make_pair(unX, unY));
}

template<class T> 
inline std::unordered_set<std::shared_ptr<T>>& FieldObjectMultimap<T>::Get(const size_t unX, 
																		   const size_t unY) {
	return Get(std::make_pair(unX, unY));
}

template<class T>
inline const std::unordered_map<pair_size_t, std::unordered_set<std::shared_ptr<T>>>& FieldObjectMultimap<T>::GetAll() const {
	return m_msetpoObjects;
}

template<class T>
inline std::unordered_map<pair_size_t, std::unordered_set<std::shared_ptr<T>>>& FieldObjectMultimap<T>::GetAll() {
	return m_msetpoObjects;
}

#endif