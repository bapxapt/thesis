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

#include "SpriteMap.h"

SpriteMap::SpriteMap():
	m_mpoSprites()
{}

bool SpriteMap::ReuseOrLoadSprite(const sptr_DrawableGeneric poDrawable,
								  const bool bUseTexture) {
	const std::string& strSpritePath(poDrawable->GetSpritePath());
	const std::shared_ptr<ofImage> poSprite(GetSprite(strSpritePath));

	if (poSprite) {
		poDrawable->SetSprite(poSprite);
		return false; //already in the map
	}
	poDrawable->LoadSprite(bUseTexture);
	AddSprite(strSpritePath, poDrawable->GetSprite());
	return true; //a new sprite has been added
}

void SpriteMap::UpdateTextures() {
	std::unordered_map<std::string, std::shared_ptr<ofImage>>& mpoSprites(GetSprites());

	for (std::pair<const std::string, std::shared_ptr<ofImage>>& KeyValue : mpoSprites) {
		const std::shared_ptr<ofImage> poSprite(KeyValue.second);

		poSprite->setUseTexture(true);
		poSprite->update();
	}
}