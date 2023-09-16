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

#include "LayoutMap.h"

LayoutMap::LayoutMap(const size_t unWidth, const size_t unHeight):
	FieldObjectVector2D<LayoutGeneric>::FieldObjectVector2D(unWidth, unHeight)
{
	if (unWidth < 3 || unHeight < 3)
		throw std::logic_error("LayoutMap::Fill(): width or height less than "
							   "3.");
}

void LayoutMap::Fill() {
	const size_t unWidth(GetWidth());
	const size_t unHeight(GetHeight());

	for (size_t unY(0); unY < unHeight; ++unY)
		for (size_t unX(0); unX < unWidth; ++unX)
			Replace(WallGeneric(unX, unY), false);
}

void LayoutMap::Clear() {
	GetPassableAll().clear();
	FieldObjectVector2D<LayoutGeneric>::Clear();
}

void LayoutMap::PassableAdd(const pair_size_t& TileXY) {
	if (!GetPassableAll().insert(TileXY).second)
		throw std::logic_error("LayoutMap::PassableAdd(): duplicate pair (" +
							   ofToString(TileXY.first) + ", " + 
							   ofToString(TileXY.second) + ") found.");
}

void LayoutMap::Generate(std::default_random_engine& oRNG) {
	GetPassableAll().reserve((GetWidth() - 2) * (GetHeight() - 2)); //no borders
	Fill();
    GeneratePassages(oRNG);
    //GenerateRooms(oRNG);
    //GeneratePassagesExtra(oRNG);
    GenerateWater(oRNG);
}

//returns a copy
pair_size_t LayoutMap::GetRandom(std::default_random_engine& oRNG,
								 const bool bIncludeBorders) const {
	const size_t unWidth(GetWidth());
	const size_t unHeight(GetHeight());
	const size_t unStart(bIncludeBorders ? 0 : 1); //the same for both X and Y
	const size_t unEndX(bIncludeBorders ? unWidth - 1 : unWidth - 2);
	const size_t unEndY(bIncludeBorders ? unHeight - 1 : unHeight - 2);
	std::uniform_int_distribution<size_t> oDistributionX(unStart, unEndX);
	std::uniform_int_distribution<size_t> oDistributionY(unStart, unEndY);

    return std::make_pair(oDistributionX(oRNG),
						  oDistributionY(oRNG));
}

//does not update sprite offsets
bool LayoutMap::Replace(const LayoutGeneric& oNew, const bool bUseTexture) {
	const pair_size_t TileXY(oNew.GetCoordinates());
    const csptr_LayoutGeneric poExisting(Get(TileXY));

	if (poExisting) {
		if (*poExisting == oNew) //no need to replace
			return false;

		const bool bExistingPassable(poExisting->IsPassable());

		if (bExistingPassable != oNew.IsPassable()) {
			if (bExistingPassable) //was passable, the new one isn't
				PassableRemove(TileXY);
			else //was not passable, the new one is
				PassableAdd(TileXY);
		}
	}
	else //the existing one is nullptr
		if (oNew.IsPassable())
			PassableAdd(TileXY);

	const sptr_LayoutGeneric poNew(std::make_shared<LayoutGeneric>(oNew));

	if (poExisting) {
		poNew->SetDiscovered(poExisting->IsDiscovered());
		poNew->SetPerceivedSprite(poExisting->GetPerceivedSprite());
		poNew->SetPerceivedSpriteOffsets(poExisting->GetPerceivedSpriteOffsets());
		poNew->UpdatePerceivedWindowCoordinates();
	}
	Set(poNew, bUseTexture);
	return true;
}

void LayoutMap::Replace(const FloorGeneric& oNew,
						std::default_random_engine& oRNG,
						const bool bUseTexture) {
	if (!Replace(static_cast<const LayoutGeneric>(oNew), bUseTexture))
		return;

	const pair_size_t& TileXY(oNew.GetCoordinates());
	const sptr_LayoutGeneric poAbove(Get(TileXY.first, TileXY.second - 1));
	const sptr_LayoutGeneric poNew(Get(TileXY.first, TileXY.second));
	const sptr_LayoutGeneric poBelow(Get(TileXY.first, TileXY.second + 1));

	if (poBelow && IsWater(poBelow) && poBelow->GetSpriteOffsetX() >= 2) {
		if (!poNew->GetSpriteOffsetX()) //wooden floor
			poBelow->SetSpriteOffsetX(1); //water and wooden beams
		else
			poBelow->SetSpriteOffsetX(0); //water and bricks
	}
	if (poAbove && IsWall(poAbove) && !poAbove->GetSpriteOffsetX())
		RandomiseTileOffsetX(poAbove, oRNG);
	if (poNew->GetSpriteOffsetX())
		RandomiseTileOffsetX(poNew, oRNG);
	return;
}

void LayoutMap::Replace(const WallGeneric& oNew,
						std::default_random_engine& oRNG,
						const bool bUseTexture) {
	if (!Replace(static_cast<const LayoutGeneric>(oNew), bUseTexture))
		return;

	const pair_size_t& TileXY(oNew.GetCoordinates());
	const sptr_LayoutGeneric poNew(Get(TileXY.first, TileXY.second));
	const sptr_LayoutGeneric poBelow(Get(TileXY.first, TileXY.second + 1));

	if (!poBelow)
		return;
	if (!IsWall(poBelow) && !poNew->GetSpriteOffsetX()) { //brick "roof"
		RandomiseTileOffsetX(poNew, oRNG);
		return;
	}
	if (IsWater(poBelow) && poBelow->GetSpriteOffsetX() >= 2)
		poBelow->SetSpriteOffsetX(0); //water and bricks
	return;
}

void LayoutMap::Replace(const WaterGeneric& oNew,
						std::default_random_engine& oRNG,
						const bool bUseTexture) {
	if (!Replace(static_cast<const LayoutGeneric>(oNew), bUseTexture))
		return;

	const pair_size_t& TileXY(oNew.GetCoordinates());
	const sptr_LayoutGeneric poAbove(Get(TileXY.first, TileXY.second - 1));
	const sptr_LayoutGeneric poNew(Get(TileXY.first, TileXY.second));
	const sptr_LayoutGeneric poBelow(Get(TileXY.first, TileXY.second + 1));
	const size_t unAboveOffsetX(poAbove->GetSpriteOffsetX());

	if (poBelow && IsWater(poBelow) && poBelow->GetSpriteOffsetX() < 2)
		poBelow->SetSpriteOffsetX(2); //water "roof"
	if (!poAbove)
		return;
	if (IsWater(poAbove)) {
		poNew->SetSpriteOffsetX(2); //water "roof"
		return;
	} //implies !IsWater(poAbove)
	poNew->SetSpriteOffsetX(0); //water and bricks by default
	if (IsFloor(poAbove)) { 
		if (!unAboveOffsetX) //wooden floor
			poNew->SetSpriteOffsetX(1); //water and wooden beams
		return; //no need to check further (not a wall)
	}
	if (IsWall(poAbove) && !unAboveOffsetX) { //a brick "roof"
		RandomiseTileOffsetX(poAbove, oRNG);
		return;
	}
}

void LayoutMap::UpdatePerceivedWater(const sptr_LayoutGeneric poWater,
									 const csptr_LayoutGeneric poAbove) {
	if (!IsWater(poWater))
		return;
	if (poAbove->IsDiscovered())
		poWater->UpdateSpritePerceivedOffsets();
	else
		poWater->SetPerceivedSpriteOffsetX(2); //water "roof"
}

void LayoutMap::UpdatePerceivedWall(const sptr_LayoutGeneric poWall,
									const csptr_LayoutGeneric poBelow) {
	if (!IsWall(poWall))
		return;
	if (poBelow->IsDiscovered())
		poWall->UpdateSpritePerceivedOffsets();
	else
		poWall->SetPerceivedSpriteOffsetX(0); //brick "roof"
}

//called by LevelMap::UpdateFromFOV()
void LayoutMap::Update(std::default_random_engine& oRNG,
					   const size_t unX, const size_t unY) {
	const sptr_LayoutGeneric poInFOV(Get(unX, unY));

	if (!poInFOV->IsDiscovered())
		poInFOV->SetDiscovered(true);
	if (!poInFOV->IsInFOV())
		poInFOV->SetInFOV(true);
	poInFOV->Update();
	if (!unX || unX == GetWidth() - 1 || unY == GetHeight() - 1)
		return; //always a brick "roof", no offset update needed
	
	const sptr_LayoutGeneric poAbove(Get(unX, unY - 1));
	const sptr_LayoutGeneric poBelow(Get(unX, unY + 1));

	UpdatePerceivedWall(poInFOV, poBelow);
	UpdatePerceivedWater(poBelow, poInFOV);
	if (!poAbove) //no tiles above
		return;
	UpdatePerceivedWall(poAbove, poInFOV);
	UpdatePerceivedWater(poInFOV, poAbove);
}

void LayoutMap::MarkAsNotInFOV(const std::unordered_map<short, uset_pair_size_t>& msetFOV) {
	for (const std::pair<short, uset_pair_size_t>& KeyValue: msetFOV)
		for (const pair_size_t& InFOVXY : KeyValue.second)
			Get(InFOVXY)->SetInFOV(false);
}

void LayoutMap::RandomiseTileOffsetX(const sptr_LayoutGeneric poTile,
									 std::default_random_engine& oRNG) {
	const size_t unOffsetMaxX(CountTileVariants(poTile->GetSprite()) - 1);
	const size_t unOffsetMinX(IsWater(poTile) ? 2 : 1);
	std::uniform_int_distribution<size_t> oDistributionOffset(unOffsetMinX, 
															  unOffsetMaxX);

	poTile->SetSpriteOffsetX(oDistributionOffset(oRNG));
}

void LayoutMap::GeneratePassages(std::default_random_engine& oRNG) {
	const size_t unTileNumber(GetWidth() * GetHeight());
	const size_t unDotsMin(unTileNumber < 100 ? 1 : unTileNumber / 100);
	const size_t unDotsMax(unTileNumber < 70 ? 1 : unTileNumber / 70);
    std::uniform_int_distribution<size_t> oDistributionDotNumber(unDotsMin,
																 unDotsMax);
    std::bernoulli_distribution oDistributionBernoulli;
    uset_pair_size_t setDotsXY; //set of tile coordinates between which passages are created
	const size_t unDots(oDistributionDotNumber(oRNG));

    while (setDotsXY.size() < unDots)
        setDotsXY.emplace(GetRandom(oRNG));
	for (auto Itr(++setDotsXY.cbegin()); Itr != setDotsXY.cend(); //avoid the 1st element 
		 std::advance(Itr, 2)) {
		const pair_size_t& PreviousXY = *Itr;
		const pair_size_t& CurrentXY = *(--Itr);
		const size_t unPreviousX(PreviousXY.first);
		const size_t unPreviousY(PreviousXY.second);
		const size_t unCurrentX(CurrentXY.first);
		const size_t unCurrentY(CurrentXY.second);
		const long nXDistance(unCurrentX - unPreviousX); //to the current
		const long nYDistance(unCurrentY - unPreviousY); //to the current
		const char nStepX(GetStepFromDistance(nXDistance));
		const char nStepY(GetStepFromDistance(nYDistance));

		if (!nStepX) { //straight vertical passage
			for (size_t iii(unPreviousY); iii != unCurrentY; iii += nStepY)
				Replace(FloorGeneric(unPreviousX, iii), oRNG, false);
			continue;
		}
		if (!nStepY) { //straight horizontal passage
			for (size_t iii(unPreviousX); iii != unCurrentX; iii += nStepX)
				Replace(FloorGeneric(iii, unPreviousY), oRNG, false);
			continue;
		}
		for (size_t unX(unPreviousX), unY(unPreviousY); 
			 unX != unCurrentX || unY != unCurrentY; ) {
			Replace(FloorGeneric(unX, unY), oRNG, false);
			if (unX == unCurrentX) //right above or below the end of the passage
				unY += nStepY;
			else
				if (unY == unCurrentY) //to the right or to the left of the end of the passage
					unX += nStepX;
				else
					if (oDistributionBernoulli(oRNG))
						unX += nStepX;
					else
						unY += nStepY;
		}
	}

	const pair_size_t& FirstPassageStartXY(*setDotsXY.cbegin());
	const FloorGeneric oFloor(FirstPassageStartXY.first, 
							  FirstPassageStartXY.second);

	Replace(oFloor, oRNG, false); //the first "tail"
}

void LayoutMap::GenerateRooms(std::default_random_engine& oRNG) {
	const size_t unWidth(GetWidth());
	const size_t unHeight(GetHeight());
	const size_t unTileNumber(GetWidth() * GetHeight());
    std::uniform_int_distribution<size_t> oDistributionRoomNumber(unTileNumber / 200, 
																  unTileNumber / 100);
	std::uniform_int_distribution<size_t> oDistributionRoomExpansions(1, 
																	  unTileNumber / 200);
    uset_pair_size_t setRoomOriginsXY;
	const size_t unRooms(oDistributionRoomNumber(oRNG));
	const uset_pair_size_t& setPassableTilesXY(GetPassableAll());

	while (setRoomOriginsXY.size() < unRooms)
		setRoomOriginsXY.insert(GetPassableRandom(oRNG));
    for (const pair_size_t& RoomOriginXY : setRoomOriginsXY) {
		const size_t unRoomOriginX(RoomOriginXY.first);
		const size_t unRoomOriginY(RoomOriginXY.second);
		const size_t unDistanceToBorderUp(unRoomOriginY - 1); //0 is border, so -1
		const size_t unDistanceToBorderRight(unWidth - 2 - unRoomOriginX); //unWidth-1 is border
		const size_t unDistanceToBorderDown(unHeight - 2 - unRoomOriginY); //unHeight-1 is border
		const size_t unDistanceToBorderLeft(unRoomOriginX - 1); //0 is border, so -1
		size_t unExpandUp(oDistributionRoomExpansions(oRNG));
		size_t unExpandRight(oDistributionRoomExpansions(oRNG));
		size_t unExpandLeft(oDistributionRoomExpansions(oRNG));
		size_t unExpandDown(oDistributionRoomExpansions(oRNG));

        if (unExpandRight > unDistanceToBorderRight)
            unExpandRight = unDistanceToBorderRight;
        if (unExpandLeft > unDistanceToBorderLeft)
            unExpandLeft = unDistanceToBorderLeft;
        if (unExpandUp > unDistanceToBorderUp)
            unExpandUp = unDistanceToBorderUp;
        if (unExpandDown > unDistanceToBorderDown)
            unExpandDown = unDistanceToBorderDown;

		const size_t unRoomTopLeftX(unRoomOriginX - unExpandLeft);
		const size_t unRoomTopLeftY(unRoomOriginY - unExpandUp);
		const size_t unRoomBottomRightX(unRoomOriginX + unExpandRight);
		const size_t unRoomBottomRightY(unRoomOriginY + unExpandDown);

        for (size_t unY(unRoomTopLeftY); unY != unRoomBottomRightY; ++unY)
            for (size_t unX(unRoomTopLeftX); unX != unRoomBottomRightX; ++unX)
				if (!IsFloor(Get(unX, unY)))
					Replace(FloorGeneric(unX, unY), oRNG, false);
    }
}

void LayoutMap::GenerateWater(std::default_random_engine& oRNG) {
	const size_t unWaterNumber(GetWidth() * GetHeight() / 80);
    std::uniform_int_distribution<size_t> oDistributionWaterNumber(0, 
																   unWaterNumber);
	const size_t unWaterTiles(oDistributionWaterNumber(oRNG));

	for (size_t iii(0); iii != unWaterTiles; ++iii) {
		const pair_size_t& RandomXY(GetPassableRandom(oRNG)); //a copy

		ExpandWater(RandomXY.first, RandomXY.second, oRNG);
	}
}

//recursive
void LayoutMap::ExpandWater(const size_t unX, const size_t unY, 
                            std::default_random_engine& oRNG,
							const float fProbability) {
	const csptr_LayoutGeneric poTile(Get(unX, unY));

	if (IsBorder(poTile) || IsWater(poTile) ||
		!TrueWithProbability(oRNG, fProbability))
		return;
	Replace(WaterGeneric(unX, unY), oRNG, false);
	for (size_t iii(0); iii != 7; ++iii) { //8 directions (0-7) in total
		const enumDirection eDirection(static_cast<enumDirection>(iii));
		const size_t unNewX(unX + GetStepX(eDirection));
		const size_t unNewY(unY + GetStepY(eDirection));

		ExpandWater(unNewX, unNewY, oRNG, 0.13);
	}
}

bool LayoutMap::IsBorder(const csptr_LayoutGeneric poTile) const {
	const size_t unX(poTile->GetX());
	const size_t unY(poTile->GetY());

	return !unX || !unY || unX == GetWidth() - 1 || unY == GetHeight() - 1;
}