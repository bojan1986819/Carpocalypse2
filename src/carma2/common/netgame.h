#ifndef GUARD_NETGAME_H
#define GUARD_NETGAME_H

#include "carpocalypse2_types.h"


#include "c2_hooks.h"

// SendShapeNumbers

// ClearShapeStatusFlag

// SendNonCar

// HostFillInCarContents

// ClientSendCarData

// DistanceBetweenCars

// CarShapeNeedsSendingToPlayer

// AddShapeStuffToMechanicsMessage

// TimeToSendData

// SendCarData
extern void C2_HOOK_FASTCALL ReceivedCarData(tNet_message_chunk_car_data* pContents);

// ReceivedRecover
extern void C2_HOOK_FASTCALL ReceivedCrushSync(tNet_message_chunk_car_crush_sync* pContents);

// CopyMechanics

// ReceivedShapeNumbers

// FillInSimpleCarPos

// ReceivedSimpleCarPos

// ReceivedMechanics

// ReceivedCopInfo

// SendAllNonCarPositions

// ReceivedNonCarPosition
extern void C2_HOOK_FASTCALL ReceivedNonCarPosition(tNet_message_chunk_non_car_position* pContents);

// ReceivedNonCar
extern void C2_HOOK_FASTCALL ReceivedNonCar(tNet_message_chunk_non_car* pContents);

// SignalToStartRace

// SignalToRepositionCar

// SetUpNetCarPositions

// ReinitialiseCar
void C2_HOOK_FASTCALL ReinitialiseCar(tCar_spec* pCar);
void C2_HOOK_FASTCALL FUN_004984c0(tNet_message_chunk_car_reposition* pContents);

// RepositionPlayer

// DisableCar

// EnableCar

// DoNetworkHeadups

// SortNetHeadAscending

// SortNetHeadDescending

// ClipName

// CreateBoxes

// DrawScoreBoxes

extern void C2_HOOK_FASTCALL DoNetScores2(int pOnly_sort_scores);

extern void C2_HOOK_FASTCALL DoNetScores(void);

extern void C2_HOOK_FASTCALL InitNetHeadups(void);

extern void C2_HOOK_FASTCALL DisposeNetHeadups(void);

// EverybodysLost

// DeclareWinner

// PlayerIsIt

// CheckForVampireWinner

void C2_HOOK_FASTCALL PlayerIsInfected(tNet_game_player_info* pPlayer);

// FarEnoughAway

// CarInContactWithFox

// SelectRandomFox

// CalcPlayerScores

// SendPlayerScores

// DoNetGameManagement

// InitialisePlayerScore

// InitPlayers

// UseGeneralScore

// NetSendEnvironmentChanges

void C2_HOOK_FASTCALL NetSendEnvironmentChanges(tNet_game_player_info* pRecipient);

extern void C2_HOOK_FASTCALL CheckForNeedyEnvironmentRecipients(void);

// ReceivedGameplay

// SendGameplay

// SendGameplayToAllPlayers

// SendGameplayToHost

// InitNetGameplayStuff

extern void C2_HOOK_FASTCALL DefaultNetName(void);

void C2_HOOK_FASTCALL AddPlayerToShapeStatusLists(tPlayer_ID pPlayer_id, int pPlayer_count, tNet_game_player_info* pPlayers);

void C2_HOOK_FASTCALL PackMatrixQuaternion(tU16* pPacked_quat, const br_matrix34* pMatrix);

void C2_HOOK_FASTCALL UnpackMatrixQuaternion(br_matrix34* pMatrix, const tU16* pPacked_quat);
void C2_HOOK_FASTCALL RemovePlayerFromShapeStatusLists(tPlayer_ID pPlayer_id, int pPlayer_count, tNet_game_player_info* pPlayers);

#define CARPOCALYPSE2_NETGAME_H

#include "carpocalypse2_types.h"

#include "c2_hooks.h"


extern int gInitialised_grid;

extern int gIt_or_fox;

extern tNet_game_player_info* gLast_lepper;

extern tU32 gLast_it_change;

extern int gNot_shown_race_type_headup;

extern int gWinner_declared;

extern tU32 gTime_for_punishment;

extern int gINT_0068d920;

void C2_HOOK_FASTCALL NetObtainSystemUserName(char* pName, int pMax_length);

void C2_HOOK_FASTCALL FUN_0049a940(void);

void C2_HOOK_FASTCALL DisableCar(tCar_spec* pCar);

void C2_HOOK_FASTCALL EnableCar(tCar_spec* pCar);

void C2_HOOK_FASTCALL InitialisePlayerScore(tNet_game_player_info* pPlayer);

void C2_HOOK_FASTCALL InitPlayers(void);
void C2_HOOK_FASTCALL FUN_0049b7e0(tU32 pPlayer_id);

void C2_HOOK_FASTCALL InitNetGameplayStuff(void);

void C2_HOOK_FASTCALL FUN_0049a990(tNet_game_player_info* pNet_player);
void C2_HOOK_FASTCALL FUN_0049a9e0(tNet_game_player_info* pNet_player);

void C2_HOOK_FASTCALL DeclareWinner(int pWinner_index);

void C2_HOOK_FASTCALL SendGameplay(tPlayer_ID pPlayer, tNet_gameplay_mess pMess, int pParam_1, int pParam_2, int pParam_3, int pParam_4);

void C2_HOOK_FASTCALL SendGameplayToAllPlayers(tNet_gameplay_mess pMess, int pParam_1, int pParam_2, int pParam_3, int pParam_4);

void C2_HOOK_FASTCALL CalcPlayerScores(void);

void C2_HOOK_FASTCALL SendPlayerScores(void);

void C2_HOOK_FASTCALL DoNetGameManagement(void);

void C2_HOOK_FASTCALL SetUpNetCarPositions(void);

void C2_HOOK_FASTCALL SignalToStartRace(void);

void C2_HOOK_FASTCALL DoNetworkHeadups(int pCredits);

void C2_HOOK_FASTCALL SendCarData(tU32 pNext_frame_time);
void C2_HOOK_FASTCALL SendLocalCarData(tU32 pNext_frame_time);
void C2_HOOK_FASTCALL SendNonCar(tNon_car_spec* pNon_car, tU32 pPlayer_id);

#endif // GUARD_NETGAME_H
