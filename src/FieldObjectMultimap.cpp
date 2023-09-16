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

#include "FieldObjectMultimap.h"

template class FieldObjectMultimap<ItemGeneric>;

template<class T>
FieldObjectMultimap<T>::FieldObjectMultimap():
	m_msetpoObjects()
{}

template<class T>
void FieldObjectMultimap<T>::Draw(const Camera2D& oCamera) const {
	const std::unordered_map<pair_size_t, uset_sptr_T>& mpoObjects(GetAll());
	const size_t unVisibleTiles(oCamera.CountVisibleExistingTiles());

	if (unVisibleTiles < mpoObjects.size()) { //iterate over coordinates
		for (size_t unY(oCamera.GetTopLeftExistingY());
			 unY <= oCamera.GetBottomRightExistingY(); ++unY)
			for (size_t unX(oCamera.GetTopLeftExistingX()); 
				 unX <= oCamera.GetBottomRightExistingX(); ++unX) {
				if (!Count(unX, unY))
					continue;
				for (const std::shared_ptr<const T> poObject : Get(unX, unY)) {
					if (!poObject->IsDiscovered())
						break; //if one in a pile isn't discovered, all aren't
					poObject->Draw(oCamera.GetOffsetX(), oCamera.GetOffsetY());
				}
			}
		return;
	} //faster to iterate over all objects
	for (const std::pair<pair_size_t, uset_sptr_T>& KeyValue : mpoObjects) {
		if (!oCamera.IsInView(KeyValue.first))
			continue; //skip this tile
		for (const std::shared_ptr<const T> poObject : KeyValue.second) {
			if (!poObject->IsDiscovered())
				break; //if one in a pile isn't discovered, all aren't
			poObject->Draw(oCamera.GetOffsetX(), oCamera.GetOffsetY());
		}
	}
}

//compares objects pointed to, not pointer values
template<class T>
std::shared_ptr<T> FieldObjectMultimap<T>::Get(const pair_size_t& TileXY, 
											   const std::shared_ptr<const T> poObject) {
	if (!Count(TileXY))
		return false;

	const uset_sptr_T& setItemsInTile(Get(TileXY));
	const auto Lambda([poObject] (const csptr_ItemGeneric poInTile) -> bool {
						  return *poInTile == *poObject; 
					  });
	const auto FoundItr(std::find_if(setItemsInTile.cbegin(), 
									 setItemsInTile.cend(), Lambda));

    return FoundItr == setItemsInTile.cend() ? nullptr : *FoundItr;
}

template<class T>
void FieldObjectMultimap<T>::Add(const std::shared_ptr<T> poNew,
								 const bool bUseTexture) {
	const pair_size_t& TileXY(poNew->GetCoordinates());
	
	if (!Count(TileXY))
		GetAll().emplace(TileXY, uset_sptr_T());
	Get(TileXY).emplace(poNew);
	ReuseOrLoadSprite(poNew, bUseTexture);
}

template<class T>
void FieldObjectMultimap<T>::Remove(const std::shared_ptr<T> poNew) {
	const pair_size_t& TileXY(poNew->GetCoordinates());
	uset_sptr_T& setpoItems(Get(TileXY));

	setpoItems.erase(poNew);
	if (!setpoItems.size())
		GetAll().erase(TileXY);
}