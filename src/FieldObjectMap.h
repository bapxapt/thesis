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

#ifndef NOTAGAME_FIELD_OBJECT_MAP_H
#define NOTAGAME_FIELD_OBJECT_MAP_H

#include "Camera2D.h"
#include "SpriteMap.h"
#include "StandingGeneric.h" //for a forward declaration in .cpp
#include "TrapGeneric.h" //for a forward declaration in .cpp

#include <stdexcept>
#include <random> //for std::default_random_engine
#include <utility> //for std::pair

template<class T> 
class FieldObjectMap: public SpriteMap {
public:
    FieldObjectMap();
	virtual void Clear();
	void UpdateCoordinateKey(const pair_size_t&);
	std::shared_ptr<const T> Get(const pair_size_t&) const;
	std::shared_ptr<T> Get(const pair_size_t&);
	std::shared_ptr<const T> Get(size_t, size_t) const;
	std::shared_ptr<T> Get(size_t, size_t);
	void Add(std::shared_ptr<T>, bool);
	void Draw(const Camera2D&) const;
    virtual void Remove(const pair_size_t&);
	const std::unordered_map<pair_size_t, std::shared_ptr<T>>& GetAll() const;
private:
	std::unordered_map<pair_size_t, std::shared_ptr<T>>& GetAll();

    std::unordered_map<pair_size_t, std::shared_ptr<T>> m_mpoObjects;
};

template<class T>
inline void FieldObjectMap<T>::Clear() {
	GetAll().clear();
}

template<class T>
inline std::shared_ptr<const T> FieldObjectMap<T>::Get(const pair_size_t& XY) const {
	return GetAll().count(XY) ? GetAll().at(XY) : nullptr;
}

template<class T>
inline std::shared_ptr<T> FieldObjectMap<T>::Get(const pair_size_t& XY) {
	return GetAll().count(XY) ? GetAll().at(XY) : nullptr;
}

template<class T>
inline std::shared_ptr<const T> FieldObjectMap<T>::Get(const size_t unX, 
													   const size_t unY) const {
	return Get(std::make_pair(unX, unY));
}

template<class T> 
inline std::shared_ptr<T> FieldObjectMap<T>::Get(const size_t unX, 
												 const size_t unY) {
	return Get(std::make_pair(unX, unY));
}

template<class T>
inline void FieldObjectMap<T>::Remove(const pair_size_t& TileXY) {
	GetAll().erase(TileXY);
}

template<class T>
inline const std::unordered_map<pair_size_t, std::shared_ptr<T>>& FieldObjectMap<T>::GetAll() const {
	return m_mpoObjects;
}

template<class T>
inline std::unordered_map<pair_size_t, std::shared_ptr<T>>& FieldObjectMap<T>::GetAll() {
	return m_mpoObjects;
}

#endif