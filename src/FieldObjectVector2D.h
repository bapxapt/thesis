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

#ifndef NOTAGAME_FIELD_OBJECT_VECTOR_2D_H
#define NOTAGAME_FIELD_OBJECT_VECTOR_2D_H

#include "Camera2D.h"
#include "SpriteMap.h"
#include "LayoutGeneric.h" //for a forward declaration in .cpp

#include <utility> //for std::pair
#include <algorithm> //for std::fill

template<class T> 
class FieldObjectVector2D: public SpriteMap {
public:
    FieldObjectVector2D(size_t, size_t);
	std::shared_ptr<const T> Get(const pair_size_t&) const;
	std::shared_ptr<T> Get(const pair_size_t&);
	std::shared_ptr<const T> Get(size_t, size_t) const;
	std::shared_ptr<T> Get(size_t, size_t);
	void Set(std::shared_ptr<T>, bool);
	void Draw(const Camera2D&) const;
	virtual void Clear();
	size_t GetWidth() const;
	size_t GetHeight() const;
	const std::vector<std::vector<std::shared_ptr<T>>>& GetAll() const;
private:
	std::vector<std::vector<std::shared_ptr<T>>>& GetAll();

    std::vector<std::vector<std::shared_ptr<T>>> m_vvpoObjects;
};

template<class T>
inline std::shared_ptr<const T> FieldObjectVector2D<T>::Get(const pair_size_t& TileXY) const {
	return Get(TileXY.first, TileXY.second);
}

template<class T>
inline std::shared_ptr<T> FieldObjectVector2D<T>::Get(const pair_size_t& TileXY) {
	return Get(TileXY.first, TileXY.second);
}

template<class T>
inline std::shared_ptr<const T> FieldObjectVector2D<T>::Get(const size_t unX, 
															const size_t unY) const {
	return unY >= GetHeight() || unX >= GetWidth() ? nullptr : 
		   GetAll().at(unY).at(unX);
}

template<class T>
inline std::shared_ptr<T> FieldObjectVector2D<T>::Get(const size_t unX, 
													  const size_t unY) {
	return unY >= GetHeight() || unX >= GetWidth() ? nullptr : 
		   GetAll().at(unY).at(unX);
}

template<class T>
inline size_t FieldObjectVector2D<T>::GetWidth() const {
	return GetHeight() ? GetAll().at(0).size() : 0; 
}

template<class T>
inline size_t FieldObjectVector2D<T>::GetHeight() const {
	return GetAll().size();
}

template<class T>
inline const std::vector<std::vector<std::shared_ptr<T>>>& FieldObjectVector2D<T>::GetAll() const {
	return m_vvpoObjects;
}

template<class T>
inline std::vector<std::vector<std::shared_ptr<T>>>& FieldObjectVector2D<T>::GetAll() {
	return m_vvpoObjects;
}

#endif