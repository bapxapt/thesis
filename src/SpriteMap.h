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

#ifndef NOTAGAME_SPRITE_MAP_H
#define NOTAGAME_SPRITE_MAP_H

#include "DrawableGeneric.h"
#include "define.h"

#include "ofImage.h"

#include <utility> //for std::pair
#include <string>
#include <unordered_map>

class SpriteMap {
public:
	SpriteMap();
	bool ReuseOrLoadSprite(sptr_DrawableGeneric, bool);
	std::shared_ptr<const ofImage> GetSprite(const std::string&) const;
	void UpdateTextures();
private:
	std::shared_ptr<ofImage> GetSprite(const std::string&);
	void AddSprite(const std::string&, std::shared_ptr<ofImage>);
	const std::unordered_map<std::string, std::shared_ptr<ofImage>>& GetSprites() const;
	std::unordered_map<std::string, std::shared_ptr<ofImage>>& GetSprites();

    std::unordered_map<std::string, std::shared_ptr<ofImage>> m_mpoSprites;
};

inline std::shared_ptr<const ofImage> SpriteMap::GetSprite(const std::string& strPath) const {
	return GetSprites().count(strPath) ? GetSprites().at(strPath) : nullptr;
}

inline std::shared_ptr<ofImage> SpriteMap::GetSprite(const std::string& strPath) {
	return GetSprites().count(strPath) ? GetSprites().at(strPath) : nullptr;
}

inline void SpriteMap::AddSprite(const std::string& strSpritePathKey, 
								 const std::shared_ptr<ofImage> poSpriteValue) {
	GetSprites().emplace(strSpritePathKey, poSpriteValue);
}

inline const std::unordered_map<std::string, std::shared_ptr<ofImage>>& SpriteMap::GetSprites() const {
	return m_mpoSprites;
}

inline std::unordered_map<std::string, std::shared_ptr<ofImage>>& SpriteMap::GetSprites() {
	return m_mpoSprites;
}

#endif