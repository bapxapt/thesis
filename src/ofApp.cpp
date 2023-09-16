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

#include "ofApp.h"

ofApp::ofApp(const size_t unFieldWidth, const size_t unFieldHeight):
	m_oLevelMap(unFieldWidth, unFieldHeight, GetGameFieldUI()),
	m_eGameState(STATE_MAIN_MENU), 
	m_oCamera(unFieldWidth, unFieldHeight),
	m_oMainMenuUI(),
    m_oGameFieldUI(),
	m_oInventoryUI(),
	m_unRedrawsNeeded(0), //will be set by ofApp::windowResized()
	m_oMapGenerationThread(GetLevelMap()),
	m_nModifierKey(-1),
	m_bGameFinished(true)
{}

void ofApp::setup() {
    ofSetFrameRate(60); 
	//ofSetVerticalSync(true);
	ofSetBackgroundAuto(false);
	ofSetEscapeQuitsApp(false);
}

void ofApp::update() {
	switch (GetGameState()) {
		case STATE_LOADING:
			if (updateLoading())
				break;
			return;
		case STATE_GAME_FIELD:
			if (updateGameField())
				break;
			return;
		default:
			return;
	}
	SetRedrawsNeeded();
}

bool ofApp::updateLoading() {
	if (GetMapGenerationThread().isThreadRunning())
		return false; //has not finished loading yet

	LevelMap& oLevelMap(GetLevelMap());
	UnitMap& oUnitMap(oLevelMap.GetUnitMap());
	const sptr_HumanoidGeneric poPlayer(oUnitMap.GetPlayer());
	InventoryUI& oInventoryUI(GetInventoryUI());

	oLevelMap.UpdateTextures();
	oInventoryUI.SetPlayer(poPlayer);
	oInventoryUI.SetInitialisationNeeded(true);
	oInventoryUI.Initialise();
	GetMainMenuUI().SetButtonLabel(0, "Restart");
	SetGameFinished(false);
	SetGameState(STATE_GAME_FIELD);
	return true;
}

bool ofApp::updateGameField() {
	Camera2D& oCamera(GetCamera());

	if (!oCamera.IsFreeRoamMode())
		return false; //did not move (no redraw needed)
	return oCamera.MoveToMousePointer(ofGetMouseX(), ofGetMouseY());
}

void ofApp::draw() {
	const size_t unRedrawsNeeded(GetRedrawsNeeded());
	const GameFieldUI& oGameFieldUI(GetGameFieldUI());

	if (!unRedrawsNeeded)
		return;
	SetRedrawsNeeded(unRedrawsNeeded - 1);
    switch (GetGameState()) {
        case STATE_MAIN_MENU: 
			GetMainMenuUI().Draw();
			break;
		case STATE_LOADING:
			GetLoadingScreenUI().Draw();
			break;
        case STATE_GAME_FIELD: {
			const LevelMap& oLevelMap(GetLevelMap());
			const auto LambdaDraw([&oLevelMap] (const Camera2D& oCamera) {
									  oLevelMap.Draw(oCamera); 
								  });

			oGameFieldUI.Draw(oLevelMap.GetUnitMap(), LambdaDraw, GetCamera());
			break;
		}
        case STATE_INVENTORY:
			GetInventoryUI().Draw();
			oGameFieldUI.DrawLog();
			break;
    }
    ofDrawBitmapString(ofToString(ofGetFrameRate()) + " FPS", 8, 18);
}

void ofApp::keyReleased(const int nKey) {
	if (nKey == OF_KEY_SHIFT || nKey == OF_KEY_CONTROL || 
		nKey == OF_KEY_ALT)
		SetModifierKey(-1);
}

void ofApp::mouseMoved(const int nX, const int nY) {
	InventoryUI& oInventoryUI(GetInventoryUI());

    switch (GetGameState()) {
        case STATE_MAIN_MENU:
			if (mouseMovedMainMenu(nX, nY))
				break;
			return;
        case STATE_GAME_FIELD:
			return;
        case STATE_INVENTORY:
            if (mouseMovedInventory(nX, nY))
				break;
			return;
		default:
			return;
    }
	SetRedrawsNeeded();
}

bool ofApp::mouseMovedMainMenu(const int nX, const int nY) {
	MainMenuUI& oMainMenuUI(GetMainMenuUI());

	if (oMainMenuUI.IsInHelpMode())
		return false;
	return oMainMenuUI.HoverButton(nX, nY) && !GetRedrawsNeeded();
}

bool ofApp::mouseMovedInventory(const int nX, const int nY) {
	InventoryUI& oInventoryUI(GetInventoryUI());
	
	return oInventoryUI.HoverSlotBackpack(nX, nY) && !GetRedrawsNeeded() ||
		   oInventoryUI.HoverSlotEquipment(nX, nY) && !GetRedrawsNeeded();
}

void ofApp::mouseDragged(const int nX, const int nY, const int nMouseButton) {
	mouseMoved(nX, nY);
}

void ofApp::mousePressed(const int nX, const int nY, const int nMouseButton) {
	;
}

void ofApp::mouseReleased(const int nX, const int nY, const int nMouseButton) {
	if (nMouseButton) //0 - left, 1 - middle button, 2 - right
		return; //accepting only left clicks
    switch (GetGameState()) {
        case STATE_MAIN_MENU: 
			if (mouseReleasedMainMenu(nX, nY, nMouseButton))
				break; 
			return;
        case STATE_GAME_FIELD:
			if (mouseReleasedGameField(nX, nY, nMouseButton))
				break;
			return;
        case STATE_INVENTORY:
			if (mouseReleasedInventory(nX, nY, nMouseButton))
				break;
			return;
		default:
			return;
    }
	SetRedrawsNeeded();
}

bool ofApp::mouseReleasedMainMenu(const int nX, const int nY, 
								  const int nMouseButton) {
	MainMenuUI& oMainMenuUI(GetMainMenuUI());
	const short nButtonIndex(oMainMenuUI.PollButton(nX, nY));

	if (oMainMenuUI.IsInHelpMode()) {
		oMainMenuUI.SetInHelpMode(false);
		mouseMovedMainMenu(nX, nY);
		return true;
	}
	if (nButtonIndex == -1) //outside of all buttons
		return false;
	if (nButtonIndex == 1) {
		oMainMenuUI.SetInHelpMode(true);
		return true;
	}
	if (nButtonIndex == 2) {
		ofExit();
		return false; //ofExit() does not immediately close the application
	}

	LevelMap& oLevelMap(GetLevelMap());

	SetGameState(STATE_LOADING);
	GetMainMenuUI().SetTextMain(""); //no label initialisation needed
	if (oLevelMap.GetLayoutMap().Get(0, 0)) { //not a nullptr
		GetCamera().Reset();
		GetGameFieldUI().ClearLog();
		GetInventoryUI().SetPlayer(nullptr);
	}
	GetMapGenerationThread().startThread();
	return true;
}

bool ofApp::mouseReleasedGameField(const int nX, const int nY, 
								   const int nMouseButton) {
	Camera2D& oCamera(GetCamera());
	const UnitMap& oUnitMap(GetLevelMap().GetUnitMap());

	if (!oCamera.IsFreeRoamMode())
		return false;
	oCamera.Center(oUnitMap.GetPlayer()->GetCoordinates());
	oCamera.SetFreeRoamMode(false);
	return true;
}

bool ofApp::mouseReleasedInventory(const int nX, const int nY, 
								   const int nMouseButton) {
	GameFieldUI& oGameFieldUI(GetGameFieldUI());
	InventoryUI& oInventoryUI(GetInventoryUI());
	LevelMap& oLevelMap(GetLevelMap());
	const sptr_HumanoidGeneric poPlayer(oLevelMap.GetUnitMap().GetPlayer());
	const short nBackpackIndex(oInventoryUI.PollSlotsBackpack(nX, nY));

	if (nBackpackIndex == -1) { //clicked outside of all backpack slots
		const short nEquipmentIndex(oInventoryUI.PollSlotsEquipment(nX, nY));

		if (nEquipmentIndex == -1 || !poPlayer->GetEquipped(nEquipmentIndex))
			return false; //clicked outside of all equipment slots, or no item

		const short nBackpackIndex(oLevelMap.PlayerUnequip(nEquipmentIndex));

		if (nBackpackIndex >= 0) {
			oInventoryUI.CalculateCoordinatesBackpack(nBackpackIndex);
			oInventoryUI.EnableSlotEquipment(nEquipmentIndex, false);
			oInventoryUI.EnableSlotBackpack(nBackpackIndex, true);
		}
		return true;
	}
	
	const sptr_ItemGeneric poInSlot(poPlayer->GetBackpackItem(nBackpackIndex));
	const short nItemCountBefore(poPlayer->CountBackpackItems());

	if (!poInSlot)
		return false;
	if (GetModifierKey() == OF_KEY_SHIFT) {
		oLevelMap.PlayerDrop(nBackpackIndex);
		oInventoryUI.EnableSlotBackpack(poPlayer->CountBackpackItems(), false);
		oInventoryUI.CalculateCoordinatesBackpack(nBackpackIndex);
		return true;
	}
    switch (poInSlot->GetType()) {
		case ItemGeneric::TYPE_EQUIPMENT: {
			const short nEquipmentIndex(oLevelMap.PlayerEquip(nBackpackIndex));

            if (nEquipmentIndex < 0)
				break;

			const short nItemCountAfter(poPlayer->CountBackpackItems());

			oInventoryUI.CalculateCoordinatesEquipment(nEquipmentIndex);
			oInventoryUI.CalculateCoordinatesBackpack(nBackpackIndex);
			if (nItemCountAfter != nItemCountBefore) { //no swap happened
				oInventoryUI.EnableSlotBackpack(nItemCountAfter, false);
				oInventoryUI.EnableSlotEquipment(nEquipmentIndex, true);
			}
			break;
		}
        case ItemGeneric::TYPE_CONSUMABLE: {
			const short nResult(oLevelMap.PlayerConsume(nBackpackIndex));

			if (nResult < 0) 
				break; //consumption failed
			if (nItemCountBefore != poPlayer->CountBackpackItems()) { //shift ^
				oInventoryUI.CalculateCoordinatesBackpack(nBackpackIndex);
				oInventoryUI.EnableSlotBackpack(poPlayer->CountBackpackItems(), 
												false);
			}
			else
				oInventoryUI.UpdateLabelTextItemQuantity(nBackpackIndex);
			break;
		}
		default:
			return false;
    }
	return true;
}

void ofApp::windowResized(const int nWidth, const int nHeight) {
	if (!nWidth || !nHeight)
		return;

	const UnitMap& oUnitMap(GetLevelMap().GetUnitMap());
	const csptr_HumanoidGeneric poPlayer(oUnitMap.GetPlayer());

	GetMainMenuUI().SetInitialisationNeeded(true);
	GetLoadingScreenUI().SetInitialisationNeeded(true);
	GetGameFieldUI().SetInitialisationNeeded(true);
	GetInventoryUI().SetInitialisationNeeded(true);
	InitialiseCurrentUI();
	SetRedrawsNeeded(5); //less than 4 does not make the window redraw
}

void ofApp::gotMessage(ofMessage msg) {
    ;
}

void ofApp::dragEvent(ofDragInfo dragInfo) {
    ;
}

void ofApp::ChoosePlayerAction(const int nKey) {
	LevelMap& oLevelMap(GetLevelMap());
	UnitMap& oUnitMap(oLevelMap.GetUnitMap());
	const sptr_HumanoidGeneric poPlayer(oUnitMap.GetPlayer());
	const enumDirection eNew(GetDirectionFromKeyPressed(nKey));

	if (poPlayer->GetDirection() != eNew) { //turn instead of moving forward
		oLevelMap.PlayerTurn(eNew);
		return;
	} //implies an attempt to move forward

	const pair_size_t NewXY(poPlayer->GetCoordinatesInFront());
	const csptr_StandingGeneric poInFront(oUnitMap.Get(NewXY));

	if (poInFront) {
		if (poInFront->IsLiving())
			oLevelMap.PlayerHit(); //attack by default
		else
			if (std::static_pointer_cast<const FurnitureGeneric>(poInFront)->IsContainer())
				oLevelMap.PlayerLoot(); //loot by default
	}
	else //no unit in front
		if (oLevelMap.PlayerMove(NewXY))
			GetCamera().Center(poPlayer->GetCoordinates()); //the XY changed
}

void ofApp::keyPressed(const int nKey) {
	if (nKey == OF_KEY_SHIFT || nKey == OF_KEY_CONTROL || 
		nKey == OF_KEY_ALT) {
		SetModifierKey(nKey);
		return;
	}
    switch (GetGameState()) {
        case STATE_MAIN_MENU: 
			if (keyPressedMainMenu(nKey))
				break;
			return; 
        case STATE_GAME_FIELD: 
			if (keyPressedGameField(nKey))
				break;
			return;
        case STATE_INVENTORY: 
			if (keyPressedInventory(nKey))
				break;
			return;
		default:
			return;
    }
	SetRedrawsNeeded();
}

bool ofApp::keyPressedMainMenu(const int nKey) {
	MainMenuUI& oMainMenuUI(GetMainMenuUI());

	if (oMainMenuUI.IsInHelpMode()) {
		oMainMenuUI.SetInHelpMode(false);
		mouseMovedMainMenu(ofGetMouseX(), ofGetMouseY());
		return true;
	}
	if (nKey != 27) //not Esc
		return false;
	if (IsGameFinished())
		ofExit();
	else
		SetGameState(STATE_GAME_FIELD); //back to the field
	return true;
}

bool ofApp::keyPressedGameField(const int nKey) {
	LevelMap& oLevelMap(GetLevelMap());
	UnitMap& oUnitMap(oLevelMap.GetUnitMap());
	const sptr_HumanoidGeneric poPlayer(oLevelMap.GetUnitMap().GetPlayer());

	if (IsGameFinished()) {
		SetGameState(STATE_MAIN_MENU);
		return true;
	}
	if (nKey == OF_KEY_ESC) {
		SetGameState(STATE_MAIN_MENU);
		return true;
	}

	Camera2D& oCamera(GetCamera());
	const bool bFreeRoamMode(oCamera.IsFreeRoamMode());

	if (bFreeRoamMode) {
        oCamera.Center(poPlayer->GetCoordinates());
		oCamera.SetFreeRoamMode(false); //pressing any key exits the mode
		return true;
	}
    if (nKey >= 49 && nKey <= 57 && nKey != 53 || //53 is for waiting
		nKey >= 57356 && nKey <= 57359) {
        ChoosePlayerAction(nKey);
		if (!oUnitMap.GetPlayer()) { //the player fell after an action
			MainMenuUI& oMainMenuUI(GetMainMenuUI());

			GetGameFieldUI().AddToLog("You have fallen.");
			oMainMenuUI.SetTextMain("Game over.");
			oMainMenuUI.SetButtonLabel(0, "Start");
			SetGameFinished(true);
		}
		return true;
    }

	InventoryUI& oInventoryUI(GetInventoryUI());

	switch (nKey) {
        case 'm': 
			oCamera.SetFreeRoamMode(!bFreeRoamMode);
			break;
        case 's': 
		case '.': 
		case 53:
			break; //wait for the smallest amount of time possible
        case 'f': //forced attack
			oLevelMap.PlayerHit();
			break; 
        case 'g': 
		case ',': { //pick up an item from the ground
			const short nIndex(oLevelMap.PlayerPickUp());
				
			if (nIndex < 0)
				break; //failed to pick up
			if (!oInventoryUI.IsSlotBackpackEnabled(nIndex)) { //a new item
				oInventoryUI.CalculateCoordinatesBackpack(nIndex);
				oInventoryUI.EnableSlotBackpack(nIndex, true);
				oInventoryUI.UpdateLabelTextItemName(nIndex);
			}
			oInventoryUI.UpdateLabelTextItemQuantity(nIndex);
			GoalCheck(poPlayer->GetBackpackItem(nIndex));
			break;
		}
        case 'p': //push a furniture
            oLevelMap.PlayerPush();
            oCamera.Center(poPlayer->GetCoordinates()); //updated XY
			break;
        case 'i': //switch to the inventory mode
            SetGameState(STATE_INVENTORY);
			break;
		default:
			return false; //skip redrawing and adding a log line
    }
	return true;
}

bool ofApp::keyPressedInventory(const int nKey) {
	if (nKey != OF_KEY_ESC && nKey != 'i')
		return false;
	SetGameState(STATE_GAME_FIELD);
	return true;
}

void ofApp::GoalCheck(const csptr_ItemGeneric poPickedUp) {
	if (poPickedUp->GetName() != "Final orb")
		return;

	const UnitMap& oUnitMap(GetLevelMap().GetUnitMap());
	const csptr_HumanoidGeneric poPlayer(oUnitMap.GetPlayer());
	MainMenuUI& oMainMenuUI(GetMainMenuUI());
	const auto Lambda([] (const csptr_ItemGeneric poInBackpack) -> bool {
						  return poInBackpack->GetName() == "Coin"; 
					  });
	const short nIndex(poPlayer->GetBackpackSlotIndex(Lambda));

	GetGameFieldUI().AddToLog("You have reached the goal!");
	if (nIndex == -1)
		oMainMenuUI.SetTextMain("The victory is yours.");
	else {
		const size_t unCoins(poPlayer->GetBackpackItem(nIndex)->GetQuantity());

		oMainMenuUI.SetTextMain("You have won and collected " + 
								ofToString(unCoins) + 
								" coin" + (unCoins > 1 ? "s." : "."));
	}
	oMainMenuUI.SetButtonLabel(0, "Start");
	SetGameFinished(true);
}

void ofApp::SetGameState(const enumGameState eState) {
	m_eGameState = eState;
	InitialiseCurrentUI();
}

void ofApp::InitialiseCurrentUI() {
	GameFieldUI& oGameFieldUI(GetGameFieldUI());
	const int nMouseX(ofGetMouseX());
	const int nMouseY(ofGetMouseY());

	switch (GetGameState()) {
		case STATE_MAIN_MENU:
			GetMainMenuUI().Initialise();
			mouseMovedMainMenu(nMouseX, nMouseY); //for hovering
			return;
		case STATE_LOADING:
			GetLoadingScreenUI().Initialise();
			return;
		case STATE_GAME_FIELD: {
			const UnitMap& oUnitMap(GetLevelMap().GetUnitMap());
			const csptr_HumanoidGeneric poPlayer(oUnitMap.GetPlayer());

			oGameFieldUI.Initialise();
			if (poPlayer)
				GetCamera().Center(poPlayer->GetCoordinates());
			return;
		}
		case STATE_INVENTORY: 
			oGameFieldUI.Initialise(); //log window
			GetInventoryUI().Initialise();
			mouseMovedInventory(nMouseX, nMouseY); //for hovering
			return;
	}
}