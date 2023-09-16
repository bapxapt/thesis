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

#include "LevelMap.h"

LevelMap::LevelMap(const size_t unX, const size_t unY, 
				   GameFieldUI& oGameFieldUI):
	m_oRNG(time(nullptr)),
    m_oLayoutMap(unX, unY),
	m_oUnitMap(GetLayoutMap()),
	m_oItemMap(),
	m_oTrapMap(),
	m_oGameFieldUI(oGameFieldUI)
{}

void LevelMap::Draw(const Camera2D& oCamera) const {
    GetLayoutMap().Draw(oCamera); 
    GetTrapMap().Draw(oCamera);
    GetItemMap().Draw(oCamera);
    GetUnitMap().Draw(oCamera);
    DrawFog(oCamera);
}

void LevelMap::Generate() {
	std::default_random_engine& oRNG(GetEngineRNG());
	LayoutMap& oLayoutMap(GetLayoutMap());
	UnitMap& oUnitMap(GetUnitMap());
	uset_pair_size_t setContainerXY;

    oLayoutMap.Generate(oRNG);
    GetTrapMap().Generate(oLayoutMap.GetPassableAll(), oUnitMap, oRNG);

	const pair_size_t& OrnateXY(oUnitMap.Generate(setContainerXY, oLayoutMap, 
												  oRNG));

	GetItemMap().Generate(setContainerXY, OrnateXY, oLayoutMap, oUnitMap, 
						  oRNG);
	UpdateFromFOV(); //the very first update
}

void LevelMap::Clear() {
    GetUnitMap().Clear();
	GetItemMap().Clear();
    GetTrapMap().Clear();
	GetLayoutMap().Clear();
}

void LevelMap::UpdateTextures() {
	GetLayoutMap().UpdateTextures();
    GetTrapMap().UpdateTextures();
	GetItemMap().UpdateTextures();
	GetUnitMap().UpdateTextures();
}

void LevelMap::UnitRemove(const sptr_StandingGeneric poRemoved) {
	const pair_size_t& RemovedXY(poRemoved->GetCoordinates());

	UnitDropRemains(poRemoved);
	GetUnitMap().Remove(RemovedXY);
	GetLayoutMap().PassableAdd(RemovedXY);
}

short LevelMap::UnitHit(const csptr_UnitGeneric poAttacker) {
    UnitMap& oUnitMap(GetUnitMap());
	const pair_size_t InFrontXY(poAttacker->GetCoordinatesInFront()); //a copy
    const sptr_StandingGeneric poTarget(oUnitMap.Get(InFrontXY));
    LayoutMap& oLayoutMap(GetLayoutMap());

	if (!poTarget) {
		const csptr_LayoutGeneric poTileInFront(oLayoutMap.Get(InFrontXY));

		return oLayoutMap.IsWall(poTileInFront) ? -2 : -3; //-3 - hit nothing
	}

	const short nHitResult(poAttacker->Hit(poTarget, GetEngineRNG()));

    if (!nHitResult) //the target has 0 HP left
        UnitRemove(poTarget);
    return nHitResult;
}

void LevelMap::PlayerHit() {
	UnitMap& oUnitMap(GetUnitMap());
	const csptr_HumanoidGeneric poPlayer(oUnitMap.GetPlayer());
	const pair_size_t InFrontXY(poPlayer->GetCoordinatesInFront()); //a copy
    const sptr_StandingGeneric poTarget(oUnitMap.Get(InFrontXY));
	const std::string& strTargetName(poTarget->GetName());
	GameFieldUI& oGameFieldUI(GetGameFieldUI());

	switch (UnitHit(poPlayer)) {
		case -3: {
			const sptr_TrapGeneric poTrap(GetTrapMap().Get(InFrontXY));

			if (poTrap && !poTrap->IsDiscovered()) {
				oGameFieldUI.AddToLog("You found a " + poTrap->GetName() + 
									  "!");
				poTrap->SetDiscovered(true);
				UpdateFromFOV();
			}
			else
				oGameFieldUI.AddToLog("You swing at nothing.");
			break;
		}
		case -2:
			oGameFieldUI.AddToLog("You hit a wall. It will not budge.");
			break;
		case -1:
			oGameFieldUI.AddToLog("You missed the " + strTargetName + ".");
			break;
		case 0: {
			const std::string strPart(poTarget->IsLiving() ? "defeat" : 
									  "destroy");

			oGameFieldUI.AddToLog("You hit the " + strTargetName + " and " +
								  strPart + " it!");
			UpdateFromFOV(); //full update: removed a unit, added items
			break;
		}
		case 1:
			oGameFieldUI.AddToLog("You hit the " + strTargetName + ".");
			poTarget->UpdatePerceivedHP();
			break;
		default:
			throw std::runtime_error("LevelMap::PlayerHit(): unexpected "
									 "return value of LevelMap::UnitHit().");
	}
}

short LevelMap::PlayerConsume(const size_t unIndex) {
    UnitMap& oUnitMap(GetUnitMap());
    const sptr_HumanoidGeneric poPlayer(oUnitMap.GetPlayer());
	const sptr_ItemGeneric poItem(poPlayer->GetBackpackItem(unIndex));
	const auto poConsumed(std::static_pointer_cast<ConsumableGeneric>(poItem));
	const short nConsumptionResult(poPlayer->Consume(unIndex, GetEngineRNG()));
	GameFieldUI& oGameFieldUI(GetGameFieldUI());

	if (nConsumptionResult >= 0) {
		const std::string& strItemName(poConsumed->GetName());

		oGameFieldUI.AddToLog("Used " + GetIndefiniteArticle(strItemName) + 
							  " " + strItemName + ".");
		oUnitMap.GetCalculatorFOV().Calculate(poPlayer);
		UpdateFromFOV();
	}
	oGameFieldUI.AddToLog(poConsumed->EffectMessage(poPlayer, 
													nConsumptionResult));
	return nConsumptionResult;
}

short LevelMap::PlayerUnequip(const size_t unEquipmentIndex) {
    const sptr_HumanoidGeneric poPlayer(GetUnitMap().GetPlayer());
    const bool bWieldable(unEquipmentIndex == SLOT_LHAND || 
						  unEquipmentIndex == SLOT_RHAND);
	const csptr_ItemGeneric poEquipped(poPlayer->GetEquipped(unEquipmentIndex));
	const short nBackpackIndex(poPlayer->Unequip(unEquipmentIndex));
	GameFieldUI& oGameFieldUI(GetGameFieldUI());

	switch (nBackpackIndex) {
		case -2:
			oGameFieldUI.AddToLog(std::string("Cannot ") + 
								  (bWieldable ? "unwield it." : 
								   "take it off."));
			break;
		case -1:
			oGameFieldUI.AddToLog(std::string("Cannot ") + 
								  (bWieldable ? "unwield it." : 
								   "take it off: ") + 
								  ": the backpack is full.");
			break;
		default: //from 0 to N-1 slot index
			oGameFieldUI.AddToLog(std::string("You ") + 
								  (bWieldable ? "put away" : 
								   "take off") +
								  " your " + poEquipped->GetName() + ".");
			UpdateFromFOV();
	}
	return nBackpackIndex;
}

short LevelMap::PlayerEquip(const size_t unBackpackIndex) {
    const sptr_HumanoidGeneric poPlayer(GetUnitMap().GetPlayer());
	const short nEquippedIndex(poPlayer->Equip(unBackpackIndex));
	GameFieldUI& oGameFieldUI(GetGameFieldUI());

	if (nEquippedIndex == -3) {
		const auto eEquipmentIndex(poPlayer->ToEquipmentIndex(unBackpackIndex));
		const bool bWieldable(eEquipmentIndex == SLOT_LHAND || 
							  eEquipmentIndex == SLOT_RHAND);

		oGameFieldUI.AddToLog(std::string("You are ") + 
							  (bWieldable ? "hold" : "wear") + 
							  "ing an identical item already.");
		return nEquippedIndex;
	}
	if (nEquippedIndex == -2) {
		const auto eEquipmentIndex(poPlayer->ToEquipmentIndex(unBackpackIndex));

		oGameFieldUI.AddToLog("Cannot unequip " +
							  poPlayer->GetEquipped(eEquipmentIndex)->GetName() + 
							  ".");
		return nEquippedIndex;
	}

	const auto eEquipmentIndex(static_cast<enumEquipmentSlot>(nEquippedIndex));
	const csptr_EquipmentGeneric poGear(poPlayer->GetEquipped(eEquipmentIndex));
	const bool bWieldable(eEquipmentIndex == SLOT_LHAND || 
						  eEquipmentIndex == SLOT_RHAND);

	oGameFieldUI.AddToLog(std::string("You ") + 
						  (bWieldable ? "wield" : "put on") + " your " + 
						  poGear->GetName() + ".");
	UpdateFromFOV();
    return nEquippedIndex;
}

short LevelMap::UnitLoot(const csptr_HumanoidGeneric poLooter) {
	const pair_size_t InFrontXY(poLooter->GetCoordinatesInFront()); //a copy
    const sptr_StandingGeneric poInFront(GetUnitMap().Get(InFrontXY));

	if (!poInFront) //nothing in front
		return -6;
	if (poInFront->IsLiving()) //not furniture
		return -5;

	const auto poFurniture(std::static_pointer_cast<FurnitureGeneric>(poInFront));

	if (!poFurniture->IsContainer()) //not a container
		return -4;

	const auto poContainer(std::static_pointer_cast<ContainerGeneric>(poInFront));
	const enumDirection eLooterMain(poLooter->GetDirection());
	const enumDirection eContainerMain(poContainer->GetDirection());
    const enumDirection eContainerOpposite(GetDirectionOpposite(eContainerMain));

    if (eLooterMain != eContainerOpposite && 
		eLooterMain != GetDirectionTurned(eContainerOpposite, -1) &&
        eLooterMain != GetDirectionTurned(eContainerOpposite, 1))
        return -3; //opening from a wrong direction
    if (poContainer->IsLocked())
        return -2;

	ItemMap& oItemMap(GetItemMap());
	const size_t unItems(poContainer->Count());

    if (!unItems)
		return -1; //empty
	for (size_t iii(0); iii != unItems; ++iii) {
		const sptr_ItemGeneric poItem(poContainer->Get(iii));

		poItem->SetCoordinates(poLooter->GetCoordinates());
		oItemMap.Add(poItem);
	}
	poContainer->Clear();
	return 0;
}

void LevelMap::PlayerLoot() {
    const csptr_HumanoidGeneric poPlayer(GetUnitMap().GetPlayer());
	const pair_size_t InFrontXY(poPlayer->GetCoordinatesInFront()); //a copy
    const std::string& strInFrontName(GetUnitMap().Get(InFrontXY)->GetName());
	GameFieldUI& oGameFieldUI(GetGameFieldUI());
	
	switch (UnitLoot(poPlayer)) {
		case -6:
			oGameFieldUI.AddToLog("There is nothing to open.");
			break;
		case -5:
		case -4:
			oGameFieldUI.AddToLog("This is not a container.");
			break;
		case -3:
			oGameFieldUI.AddToLog("You can't open it and look inside from "
								  "this side.");
			break;
		case -2:
			oGameFieldUI.AddToLog("The " + strInFrontName + 
								  " is secured with a lock.");
			break;
		case -1:
			oGameFieldUI.AddToLog("The " + strInFrontName + " is empty.");
			break;
		case 0:
			oGameFieldUI.AddToLog("You find some items in the " + 
								  strInFrontName + ".");
			UpdateFromFOV();
			break;
		default:
			throw std::runtime_error("LevelMap::PlayerLoot(): unexpected "
									 "return value of LevelMap::UnitLoot().");
	}
}

//returns values from -2 to 2 (1-2 signalises trap activation)
short LevelMap::UnitMove(const sptr_StandingGeneric poStanding, 
						 const pair_size_t& NewXY) {
	LayoutMap& oLayoutMap(GetLayoutMap());
	const csptr_LayoutGeneric poTileNew(oLayoutMap.Get(NewXY));
	const pair_size_t OldXY(poStanding->GetCoordinates()); //a copy

    if (OldXY == NewXY || !poTileNew->IsPassable())
        return -2; //the XY did not change
	if (poStanding->IsLiving()) {
		const auto poUnit(std::static_pointer_cast<UnitGeneric>(poStanding));

		if (poUnit->IsCapableOfAdvancedManipulations()) {
			const auto poHumanoid(std::static_pointer_cast<HumanoidGeneric>(poUnit));
			
			if (poHumanoid->IsOverEncumbered())
				return -1; //carries too much to move
		}
	}

	UnitMap& oUnitMap(GetUnitMap());
	TrapMap& oTrapMap(GetTrapMap());
	const sptr_TrapGeneric poTrap(oTrapMap.Get(NewXY));
	short nTrapResult(0);

	if (poTrap) { //at the new coordinates
		GameFieldUI& oGameFieldUI(GetGameFieldUI());
		const bool bInFOV(poTileNew->IsInFOV());

		nTrapResult = poTrap->Activate(poStanding, GetEngineRNG(), bInFOV);
		if (!poTrap->GetHPCurrent())
			oTrapMap.Remove(NewXY);
		if (nTrapResult >= 0 && bInFOV) { //the trap got activated
			oGameFieldUI.AddToLog("A " + poTrap->GetName() + " activates!");
			oGameFieldUI.AddToLog(poTrap->EffectMessage(poStanding, 
															nTrapResult));
		}
	}
    poStanding->SetCoordinates(NewXY);
	oUnitMap.UpdateCoordinateKey(OldXY); //after setting new coordinates
	oLayoutMap.PassableAdd(OldXY); //frees the old XY
    oLayoutMap.PassableRemove(NewXY); //occupies the new XY
	if (poStanding->IsLiving()) {
		const auto poUnit(std::static_pointer_cast<UnitGeneric>(poStanding));

		poUnit->TickBuffs();
		oUnitMap.GetCalculatorFOV().Calculate(poUnit); //a whole new FOV
	}
	if (!poStanding->GetHPCurrent()) //can happen after a trap
		UnitRemove(poStanding);
	return nTrapResult; //0 if no trap was triggered
}

//returns false if the player's XY didn't change, true if they did
bool LevelMap::PlayerMove(const pair_size_t& NewXY) {
	const sptr_HumanoidGeneric poPlayer(GetUnitMap().GetPlayer());
	const pair_size_t InFrontXY(poPlayer->GetCoordinatesInFront());
	const csptr_TrapGeneric poTrap(GetTrapMap().Get(InFrontXY));
	const std::unordered_map<short, uset_pair_size_t> oOldFOV(poPlayer->GetFOV());
	GameFieldUI& oGameFieldUI(GetGameFieldUI());
	const short nMoveResult(UnitMove(poPlayer, NewXY));

	if (nMoveResult == -2)
		return false; //no need to center (the XY did not change)
	if (nMoveResult == -1) {
		oGameFieldUI.AddToLog("You are over-encumbered and cannot move.");
		return false; //no need to center (the XY did not change)
	}
	GetLayoutMap().MarkAsNotInFOV(oOldFOV);
    UpdateFromFOV(poPlayer); //might not exist in the UnitMap after moving
    return true; //the camera needs to be centered (the XY changed)
}

short LevelMap::UnitPush(const sptr_UnitGeneric poPusher) {
	UnitMap& oUnitMap(GetUnitMap());
	const pair_size_t InFrontXY(poPusher->GetCoordinatesInFront()); //a copy
    const sptr_StandingGeneric poInFront(oUnitMap.Get(InFrontXY));

	if (!poInFront) //nothing to push
        return -6;
    if (poInFront->IsLiving()) //not a furniture
        return -5;

	const auto poPushed(std::static_pointer_cast<FurnitureGeneric>(poInFront));

	if (!poPushed->IsMovable()) //immovable
        return -4;

    const enumDirection eDirection(poPusher->GetDirection());
	const size_t unPushedToX(poPusher->GetX() + 2 * GetStepX(eDirection));
	const size_t unPushedToY(poPusher->GetY() + 2 * GetStepY(eDirection));
    LayoutMap& oLayoutMap(GetLayoutMap());
	const csptr_LayoutGeneric poTilePushedTo(oLayoutMap.Get(unPushedToX, 
															unPushedToY));

    if (oUnitMap.Get(unPushedToX, unPushedToY) || //a unit is behind
		oLayoutMap.IsWall(poTilePushedTo)) //a wall is behind
        return -3;
    if (!poPusher->GetStrength())
        return -2;
	if (oLayoutMap.IsWater(poTilePushedTo)) { //pushing into water
		const FloorGeneric oFloor(unPushedToX, unPushedToY, 0, 
								  poPushed->GetMaterial());

		if (poPushed->IsContainer()) {
			const auto poContainer(std::static_pointer_cast<ContainerGeneric>(poPushed));
			const auto Lambda([] (const csptr_ItemGeneric poItem) -> bool {
								  return poItem->GetName() == "Unusual key"; 
							  });

			if (poContainer->Contains(Lambda)) {
				if (poTilePushedTo->IsInFOV())
					GetGameFieldUI().AddToLog("Something important changed "
											  "its place.");
				GetItemMap().AddGoalKey(oLayoutMap, oUnitMap, GetEngineRNG());
			}
		}
        oUnitMap.Remove(InFrontXY); //in front of the pusher
        oLayoutMap.Replace(oFloor, GetEngineRNG());
		return -1;
    }
    return UnitMove(poPushed, std::make_pair(unPushedToX, unPushedToY));
}

void LevelMap::PlayerPush() {
    const sptr_HumanoidGeneric poPlayer(GetUnitMap().GetPlayer());
	const pair_size_t InFrontXY(poPlayer->GetCoordinatesInFront());
	const pair_size_t TwoTilesInFrontXY(poPlayer->GetCoordinatesInFront(2));
	const csptr_StandingGeneric poInFront(GetUnitMap().Get(InFrontXY));
	const csptr_TrapGeneric poTrap(GetTrapMap().Get(TwoTilesInFrontXY));
	GameFieldUI& oGameFieldUI(GetGameFieldUI());

	switch (UnitPush(poPlayer)) {
		case -6:
			oGameFieldUI.AddToLog("Nothing to push.");
			return;
		case -5:
			oGameFieldUI.AddToLog("Only furniture can be pushed.");
			return;
		case -4:
			oGameFieldUI.AddToLog("The " + poInFront->GetName() + 
								  " is fixed to the floor.");
			return;
		case -3:
			oGameFieldUI.AddToLog("Can't push: there is something behind it.");
			return;
		case -2:
			oGameFieldUI.AddToLog("You struggle to push it, but in vain.");
			return;
		case -1:
			oGameFieldUI.AddToLog("You push the " + poInFront->GetName() + 
								  ", and it falls into water!");
			break;
	}
	PlayerMove(poPlayer->GetCoordinatesInFront());
}

short LevelMap::UnitPickUp(const sptr_HumanoidGeneric poTaker) {
	ItemMap& oItemMap(GetItemMap());
	const pair_size_t& TileXY(poTaker->GetCoordinates());
	const sptr_ItemGeneric poPickedUp(oItemMap.GetFirst(TileXY));

	if (!poPickedUp) //no items at this tile
        return -2;

	const short nBackpackIndex(poTaker->PickUp(poPickedUp)); //from -1 to N-1

	if (nBackpackIndex != -1) //-1 means no slots, and cannot stack the item 
		oItemMap.Remove(poPickedUp); //remove from the floor
	return nBackpackIndex;
}

short LevelMap::PlayerPickUp() {
    const sptr_HumanoidGeneric poPlayer(GetUnitMap().GetPlayer());
	const pair_size_t& PlayerXY(poPlayer->GetCoordinates());
	const csptr_ItemGeneric poItemFirst(GetItemMap().GetFirst(PlayerXY));
	const short nPickResult(UnitPickUp(poPlayer));
	GameFieldUI& oGameFieldUI(GetGameFieldUI());

	switch (nPickResult) {
		case -2:
			oGameFieldUI.AddToLog("Nothing to pick up.");
			break;
		case -1:
			oGameFieldUI.AddToLog("Can't pick up: the backpack is full.");
			break;
		default: { //from 0 to N-1 slot index
			const size_t unItemQuantity(poItemFirst->GetQuantity());
			const std::string& strItemName(poItemFirst->GetName());

			oGameFieldUI.AddToLog("You pick up " + ofToString(unItemQuantity) +
								  " " + strItemName + 
								  (unItemQuantity > 1 ? "(s)." : "."));
			break;
		}
	}
	return nPickResult;
}

void LevelMap::UnitDrop(const sptr_HumanoidGeneric poDropper,
						const size_t unBackpackIndex) {
	const sptr_ItemGeneric poDropped(poDropper->GetBackpackItem(unBackpackIndex));
	const size_t unQuantity(poDropped->GetQuantity());
	ItemMap& oItemMap(GetItemMap());
	
	poDropper->RemoveBackpackItem(unBackpackIndex);
	poDropped->SetCoordinates(poDropper->GetCoordinates());
	oItemMap.Add(poDropped);
}

void LevelMap::PlayerDrop(const size_t unBackpackIndex) {
    const sptr_HumanoidGeneric poPlayer(GetUnitMap().GetPlayer());
	const sptr_ItemGeneric poDropped(poPlayer->GetBackpackItem(unBackpackIndex));
	const std::string& strItemName(poDropped->GetName());
	const size_t unQuantity(poDropped->GetQuantity());
	
	UnitDrop(poPlayer, unBackpackIndex);
	if (poDropped->IsStackable())
		GetGameFieldUI().AddToLog("You drop " + ofToString(unQuantity) + " " + 
								  strItemName + 
								  (unQuantity > 1 ? "(s)." : "."));
	else
		GetGameFieldUI().AddToLog("You drop your " + strItemName + ".");
	UpdateFromFOV();
}

void LevelMap::UnitTurn(const sptr_UnitGeneric poUnit, 
						const enumDirection eNew) {
	const enumDirection eOld(poUnit->GetDirection());

	poUnit->SetDirection(eNew);
    GetUnitMap().GetCalculatorFOV().Calculate(poUnit, eOld);
}

void LevelMap::PlayerTurn(const enumDirection eNew) {
    const sptr_HumanoidGeneric poPlayer(GetUnitMap().GetPlayer());

	GetLayoutMap().MarkAsNotInFOV(poPlayer->GetFOV());
	UnitTurn(poPlayer, eNew);
    UpdateFromFOV();
}

void LevelMap::UnitDropRemains(const sptr_StandingGeneric poStanding) {
    ItemMap& oItemMap(GetItemMap());
	std::default_random_engine& oRNG(GetEngineRNG());
	const pair_size_t& TargetXY(poStanding->GetCoordinates());

	if (poStanding->IsLiving()) {
		const auto poUnit(std::static_pointer_cast<UnitGeneric>(poStanding));

        if (poUnit->IsCapableOfAdvancedManipulations()) {
			const auto poHumanoid(std::static_pointer_cast<HumanoidGeneric>(poUnit));
			const size_t unItems(poHumanoid->CountBackpackItems());

			for (size_t iii(SLOT_LRING); iii != SLOT_NONE; ++iii) {
				const sptr_EquipmentGeneric poGear(poHumanoid->GetEquipped(iii));

				if (!poGear)
					continue;
				poGear->SetCoordinates(TargetXY);
				oItemMap.Add(poGear);
			}
			for (size_t iii(0); iii < unItems; ++iii) {
				const sptr_ItemGeneric poItem(poHumanoid->GetBackpackItem(iii));

				poItem->SetCoordinates(TargetXY);
				oItemMap.Add(poItem);
			}
		}
		return;
    }

	const float fWeight(poStanding->GetWeight());
	const size_t unMaxSticks(fWeight / 8);
	const size_t unMaxPlanks(fWeight / 14);
	std::uniform_int_distribution<size_t> oDistributionSticks(0, unMaxSticks);
	std::uniform_int_distribution<size_t> oDistributionPlanks(0, unMaxPlanks);
	size_t unPlankNumber(oDistributionSticks(oRNG));
	size_t unStickNumber(oDistributionPlanks(oRNG));
	const enumMaterial eMaterial(poStanding->GetMaterial());
	const auto poFurniture(std::static_pointer_cast<FurnitureGeneric>(poStanding));

	if (poFurniture->IsContainer()) {
		const auto poContainer(std::static_pointer_cast<ContainerGeneric>(poFurniture));
		const size_t unItems(poContainer->Count());

		for (size_t iii(0); iii < unItems; ++iii) {
			const sptr_ItemGeneric poItem(poContainer->Get(iii));

			poItem->SetCoordinates(TargetXY);
			oItemMap.Add(poItem);
		}
	}
	if (unPlankNumber)
		oItemMap.Add(ItemGeneric("plank.png", TargetXY.first, TargetXY.second, 
								 eMaterial, "Plank", 1, 5.0, 
								 ItemGeneric::TYPE_MISC, unPlankNumber));
	if (unStickNumber) 
		oItemMap.Add(ItemGeneric("stick.png", TargetXY.first, TargetXY.second, 
								 eMaterial, "Stick", 1, 2.0, 
								 ItemGeneric::TYPE_MISC, unStickNumber));
}

//updates m_bDiscovered in layout/unit/item maps
void LevelMap::UpdateFromFOV(const sptr_HumanoidGeneric poPlayer) {
    const std::unordered_map<short, uset_pair_size_t>& mFOV(poPlayer->GetFOV());
	LayoutMap& oLayoutMap(GetLayoutMap());
	UnitMap& oUnitMap(GetUnitMap());
	ItemMap& oItemMap(GetItemMap());
	TrapMap& oTrapMap(GetTrapMap());

    for (const std::pair<short, uset_pair_size_t>& KeyValue : mFOV) {
		const uset_pair_size_t& setPartFOV(KeyValue.second);

		for (const pair_size_t& CurrentXY : setPartFOV) {
			const sptr_StandingGeneric poStanding(oUnitMap.Get(CurrentXY));
			const sptr_TrapGeneric poTrap(oTrapMap.Get(CurrentXY));

			oLayoutMap.Update(GetEngineRNG(), CurrentXY.first, 
							  CurrentXY.second);
			if (poStanding) { //a unit/furniture in current tile
				if (!poStanding->IsDiscovered())
					poStanding->SetDiscovered(true); //will be drawn under fog
				poStanding->Update();
			}
			if (oItemMap.Count(CurrentXY)) {
				std::unordered_set<sptr_ItemGeneric>& setpoItems(oItemMap.Get(CurrentXY));

				for (const sptr_ItemGeneric poItem : setpoItems) {
					if (!poItem->IsDiscovered())
						poItem->SetDiscovered(true);
					poItem->Update();
				}
			}
			if (poTrap && poTrap->IsDiscovered())
				poTrap->Update();
		}
    }
}

//draws fog on previously explored tiles only if they're out of FOV
void LevelMap::DrawFog(const Camera2D& oCamera) const {
	const LayoutMap& oLayoutMap(GetLayoutMap());

	ofPushStyle();
	ofSetColor(0, 0, 0, 60); //the 2nd argument is alpha
    for (size_t unY(oCamera.GetTopLeftExistingY()); 
		 unY <= oCamera.GetBottomRightExistingY(); ++unY)
		for (size_t unX(oCamera.GetTopLeftExistingX()); 
			 unX <= oCamera.GetBottomRightExistingX(); ++unX) {
			const csptr_LayoutGeneric poTile(oLayoutMap.Get(unX, unY));

            if (poTile->IsDiscovered() && !poTile->IsInFOV())
				ofDrawRectangle(unX * SPRITE_W + oCamera.GetOffsetX(), 
								unY * SPRITE_H + oCamera.GetOffsetY(), 
								SPRITE_W, SPRITE_H);
		}
	ofPopStyle(); //return color to normal
}