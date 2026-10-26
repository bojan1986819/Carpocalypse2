#include "oppoproc.h"

#include <string.h>

#include "loading.h"
#include "opponent.h"
#include "platform.h"
#include "sound.h"
#include "flicplay.h"
#include "grafdata.h"
#include "displays.h"
#include "graphics2.h"
#include "globvars.h"
#include "utility.h"

#include "carpocalypse2_macros.h"
#include "oppoproc.h"

// GLOBAL: CARMA2_HW 0x00691790
br_pixelmap* gDials_pix;

// GLOBAL: CARMA2_HW 0x0058a61c
const br_scalar gFLOAT_0058a61c = 0.0f;

// GLOBAL: CARMA2_HW 0x0058a630
const double gDOUBLE_0058a630 = 1.0;

// GLOBAL: CARMA2_HW 0x0058a638
const br_scalar gFLOAT_0058a638 = 1.0f;

// GLOBAL: CARMA2_HW 0x0058a644
const br_scalar gFLOAT_0058a644 = -1.0f;

// GLOBAL: CARMA2_HW 0x0058a64c
const br_scalar gFLOAT_0058a64c = 0.1f;

// GLOBAL: CARMA2_HW 0x0058a670
const br_scalar gFLOAT_0058a670 = 2.384185791015625e-7f;

// GLOBAL: CARMA2_HW 0x0058a688
const br_scalar gFLOAT_0058a688 = 0.001f;

// GLOBAL: CARMA2_HW 0x0058a68c
const br_scalar gFLOAT_0058a68c = -2.0f;

// GLOBAL: CARMA2_HW 0x0058a690
const br_scalar gFLOAT_0058a690 = 9.0f;

// GLOBAL: CARMA2_HW 0x0058a694
const br_scalar gFLOAT_0058a694 = 0.3333333432674408f;

// GLOBAL: CARMA2_HW 0x0058a698
const br_scalar gFLOAT_0058a698 = 0.7f;

// DoNotDprintf

// StraightestArcForCorner2D

// CornerFudge

// MaxCurvatureForCarSpeed

// Vector2Cross

// RoughlyColinear

// GetStraight

// TimeToStopStruggling

// TimeToBeBrakingWhenStruggling

// CalcCorners

// Distance2D

// SectionLength2D

// NearestSectionInStraight

// SetMaxSpeedFromSOCs

// CalcSOCs

// StuffDataFromCorner

// SetCurvature

// ProcessFollowPath

// FollowCheatyPath
// FUNCTION: CARMA2_HW 0x004b13c0
void C2_HOOK_CDECL DoNotDprintf_oppoproc(const char* pMessage, ...) {
}

br_scalar C2_HOOK_FASTCALL Vector2Cross(const br_vector2* pV1, const br_vector2* pV2) {

    return pV1->v[0] * pV2->v[1] - pV1->v[1] * pV2->v[0];
}

tFollow_path_result FollowCheatyPath(tOpponent_spec* pOpponent_spec) {
    tFollow_path_data* data;
    br_vector3 a;
    br_vector3 p;
    br_vector3 section_v;
    br_vector3 car_to_end;
    br_vector3 car_to_intersect;
    const br_vector3* start;
    const br_vector3* finish;
    br_scalar t;
    br_scalar frame_period_in_secs;
    br_scalar distance_to_end;
    br_scalar distance_to_intersect;
    br_scalar desired_speed_BRU;

    data = &pOpponent_spec->follow_path_data;
    start = GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no);
    finish = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no);
    if (pOpponent_spec->follow_path_data.cheating != pOpponent_spec->cheating) {
        data->cheating = pOpponent_spec->cheating;
        if (data->cheating) {
            DoNotDprintf_oppoproc("%s: Dematerialising", pOpponent_spec->car_spec->driver_name);
            BrVector3Sub(&section_v, finish, start);
            BrVector3Sub(&p, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, start);
            t = BrVector3Dot(&section_v, &p) / BrVector3LengthSquared(&section_v);
            if (t < 0.0f) {
                BrVector3Copy(&data->cheaty_intersect, start);
            } else if (t > 1.f) {
                BrVector3Copy(&data->cheaty_intersect, finish);
            } else {
                BrVector3Scale(&data->cheaty_intersect, &section_v, t);
                BrVector3Accumulate(&data->cheaty_intersect, start);
            }
            BrVector3Sub(&car_to_intersect, &data->cheaty_intersect,
                &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
            distance_to_intersect = BrVector3Length(&car_to_intersect);
            frame_period_in_secs = gFrame_period_for_this_munging / 1000.0f * 6.0f;
            TurnOpponentPhysicsOff(pOpponent_spec);
            if (distance_to_intersect >= frame_period_in_secs) {
                data->moving_to_intersect = 1;
                BrVector3Normalise(&car_to_intersect, &car_to_intersect);
                BrVector3Scale(&car_to_intersect, &car_to_intersect, frame_period_in_secs);
                BrVector3Accumulate(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &car_to_intersect);
                return eFPR_OK;
            } else {
                data->moving_to_intersect = 0;
                BrVector3Copy(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &data->cheaty_intersect);
                return eFPR_OK;
            }
        }
        BrVector3Sub(&car_to_end, finish, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
        distance_to_end = BrVector3Length(&car_to_end);
        DoNotDprintf_oppoproc("%s: Rematerialising from FollowCheatyPath()...", pOpponent_spec->car_spec->driver_name);
        if (pOpponent_spec->follow_path_data.section_no < 0) {
            PDFatalError("No useful section number in ProcessCheatyPath()");
        }
        if (GetOpponentsSectionMaxSpeed(pOpponent_spec, pOpponent_spec->follow_path_data.section_no, 1) < 255) {
            desired_speed_BRU = GetOpponentsSectionMaxSpeed(pOpponent_spec, pOpponent_spec->follow_path_data.section_no, 1) / WORLD_SCALE;
        } else if (GetOpponentsSectionMinSpeed(pOpponent_spec, pOpponent_spec->follow_path_data.section_no, 1) > 0) {
            desired_speed_BRU = GetOpponentsSectionMinSpeed(pOpponent_spec, pOpponent_spec->follow_path_data.section_no, 1) / WORLD_SCALE;
        } else {
            desired_speed_BRU = MIN(7.0f, MAX(1.0f, 2.f * distance_to_end));
        }

        if (RematerialiseOpponentOnThisSection(pOpponent_spec, data->section_no, desired_speed_BRU)) {
            pOpponent_spec->car_spec->brake_force = 0.f;
            pOpponent_spec->car_spec->acc_force = 0.f;
            return eFPR_OK;
        }
        data->made_it = 1;
    }
    frame_period_in_secs = gFrame_period_for_this_munging / 1000.0f * 6.0f;
    BrVector3Set(&pOpponent_spec->car_spec->collision_info->v, 0.f, 41.4f, 0.f);

    if (data->moving_to_intersect) {
        BrVector3Sub(&car_to_intersect, &data->cheaty_intersect, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
        distance_to_intersect = BrVector3Length(&car_to_intersect);
        if (distance_to_intersect < frame_period_in_secs) {
            data->moving_to_intersect = 0;
            BrVector3Copy(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &data->cheaty_intersect);
            BrVector3Sub(&car_to_end, finish, start);
        } else {
            BrVector3Normalise(&car_to_intersect, &car_to_intersect);
            BrVector3Scale(&car_to_intersect, &car_to_intersect, frame_period_in_secs);
            BrVector3Accumulate(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &car_to_intersect);
        }
        PointActorAlongThisBloodyVector(pOpponent_spec->car_spec->car_master_actor, &car_to_end);
    } else {
        while (frame_period_in_secs > 0.0f) {
            BrVector3Sub(&a, finish, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
            distance_to_end = BrVector3Length(&a);
            if (distance_to_end < frame_period_in_secs) {
                BrVector3Copy(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, finish);
                DoNotDprintf_oppoproc("%s: ProcessFollowPath() - current section %d(#%d) (cheating)", pOpponent_spec->car_spec->driver_name, data->section_no, GetOpponentsRealSection(pOpponent_spec, data->section_no));
                data->section_no = GetOpponentsNextSection(pOpponent_spec, data->section_no);
                if (data->section_no == -1) {
                    pOpponent_spec->car_spec->acc_force = 0.f;
                    pOpponent_spec->car_spec->brake_force = 0.f;
                    DoNotDprintf_oppoproc("%s: ProcessFollowPath() - reached end of path while cheating", pOpponent_spec->car_spec->driver_name);
                    return eFPR_end_of_path;
                }
                DoNotDprintf_oppoproc("%s: ProcessFollowPath() - next section %d(#%d) (cheating)", pOpponent_spec->car_spec->driver_name, data->section_no, GetOpponentsRealSection(pOpponent_spec, data->section_no));
                start = GetOpponentsSectionStartNodePoint(pOpponent_spec, data->section_no);
                finish = GetOpponentsSectionFinishNodePoint(pOpponent_spec, data->section_no);
                frame_period_in_secs -= distance_to_end;
            } else {
                BrVector3Scale(&p, &a, frame_period_in_secs / distance_to_end);
                BrVector3Accumulate(&pOpponent_spec->car_spec->car_master_actor->t.t.translate.t, &p);
                frame_period_in_secs = 0.0;
            }
        }
        BrVector3Sub(&car_to_end, finish, &pOpponent_spec->car_spec->car_master_actor->t.t.translate.t);
        PointActorAlongThisBloodyVector(pOpponent_spec->car_spec->car_master_actor, &car_to_end);
    }
    return eFPR_OK;
}

#pragma auto_inline(off)
// C2PORT-NONMATCHING: best 57.53% | difftest PASS 2000 trials (spec difftest/specs/004b13d0_GetStraight.json) | reason VC5 x87 scheduling and stack layout
// FUNCTION: CARMA2_HW 0x004b13d0
int C2_HOOK_FASTCALL GetStraight(br_vector2* pStart, br_vector2* pFinish, float* pWidth, tS16 pSection, tOpponent_spec* pOpponent_spec) {
    float fVar1;
    float fVar2;
    float fVar3;
    float fVar4;
    float fVar5;
    float fVar6;
    int bVar7;
    int iVar8;
    const float* pfVar9;
    int iVar10;
    float fVar11;

    iVar8 = (int)GetOpponentsSectionStartNodePoint(pOpponent_spec, pSection);
    *pStart->v = *(float*)(iVar8 + 8);
    pfVar9 = (const float*)GetOpponentsSectionStartNodePoint(pOpponent_spec, pSection);
    pStart->v[1] = *pfVar9;
    iVar8 = (int)GetOpponentsSectionFinishNodePoint(pOpponent_spec, pSection);
    *pFinish->v = *(float*)(iVar8 + 8);
    pfVar9 = (const float*)GetOpponentsSectionFinishNodePoint(pOpponent_spec, pSection);
    pFinish->v[1] = *pfVar9;
    fVar11 = GetOpponentsSectionWidth(pOpponent_spec, pSection);
    *pWidth = fVar11;
    iVar8 = GetOpponentsNextSection(pOpponent_spec, pSection);
    if (iVar8 != -1) {
        for (;;) {
            iVar10 = (int)GetOpponentsSectionFinishNodePoint(pOpponent_spec, (tS16)iVar8);
            fVar2 = *(float*)(iVar10 + 8);
            pfVar9 = (const float*)GetOpponentsSectionFinishNodePoint(pOpponent_spec, (tS16)iVar8);
            fVar1 = *pfVar9;
            fVar3 = pFinish->v[0] - pStart->v[0];
            fVar5 = pFinish->v[1] - pStart->v[1];
            fVar4 = (fVar2 - pFinish->v[0]) + fVar3;
            fVar6 = (fVar1 - pFinish->v[1]) + fVar5;
            if ((fVar6 * fVar5 + fVar4 * fVar3 < gFLOAT_0058a61c)
                    || (gFLOAT_0058a64c <= fabs((fVar6 * fVar3 + -fVar5 * fVar4)
                    / sqrt(fVar6 * fVar6 + fVar4 * fVar4)))) {
                bVar7 = 0;
            } else {
                bVar7 = 1;
            }
            if (bVar7 == 0) {
                break;
            }
            pFinish->v[0] = fVar2;
            pFinish->v[1] = fVar1;
            fVar11 = GetOpponentsSectionWidth(pOpponent_spec, (tS16)iVar8);
            if (fVar11 < *pWidth) {
                *pWidth = fVar11;
            }
            iVar8 = GetOpponentsNextSection(pOpponent_spec, (tS16)iVar8);
            if (iVar8 == -1) {
                return -1;
            }
        }
    }
    return iVar8;
}
#pragma auto_inline(on)

// C2PORT-NONMATCHING: best 39.44% | difftest PASS 2000 trials (spec 004b1560_CalcCorners.json) | reason VC5 x87 scheduling and stack layout
// FUNCTION: CARMA2_HW 0x004b1560
int C2_HOOK_FASTCALL CalcCorners(tCorner* pCorners, int pSection, float pWidth, br_vector2* pStart, tOpponent_spec* pOpponent_spec) {
    float fVar1;
    float fVar2;
    float fVar3;
    float fVar4;
    float fVar5;
    float fVar6;
    float fVar7;
    float fVar9;
    int iVar10;
    tCorner* pCorner;
    br_vector2 local_start;
    float local_18;
    float local_14;
    float local_10;
    float local_c;
    br_vector2 local_finish;
    float distance_from_previous_sq;
    float section_length_sq;

    local_10 = pStart->v[0];
    local_c = pStart->v[1];
    iVar10 = 0;
    if (pSection != -1) {
        pCorner = pCorners;
        do {
            fVar9 = pWidth;
            if (3 < iVar10) {
                return iVar10;
            }
            pCorner->section = pSection;
            pSection = GetStraight(&local_start, &local_finish, &pWidth, (tS16)pSection, pOpponent_spec);
            local_18 = local_10 - local_start.v[0];
            local_14 = local_c - local_start.v[1];
            fVar1 = local_finish.v[0] - local_start.v[0];
            fVar2 = local_finish.v[1] - local_start.v[1];
            distance_from_previous_sq = local_14 * local_14 + local_18 * local_18;
            section_length_sq = fVar2 * fVar2 + fVar1 * fVar1;
            if (distance_from_previous_sq == gFLOAT_0058a61c || section_length_sq == gFLOAT_0058a61c) {
                goto LAB_004b1839;
            }
            fVar3 = fVar2 * local_14 + fVar1 * local_18;
            fVar6 = fVar1 * local_14 + local_18 * -fVar2;
            fVar7 = (float)sqrt(fVar6 * fVar6 + fVar3 * fVar3);
            fVar4 = gFLOAT_0058a638;
            fVar5 = gFLOAT_0058a61c;
            if (gFLOAT_0058a670 < fVar7) {
                fVar7 = (float)(gDOUBLE_0058a630 / fVar7);
                fVar4 = fVar7 * fVar3;
                fVar5 = fVar7 * fVar6;
            }
            if (fVar5 <= gFLOAT_0058a61c) {
                pCorner->left_not_right = 0;
                fVar5 = -fVar5;
            } else {
                pCorner->left_not_right = 1;
            }
            if (fVar5 < gFLOAT_0058a688) {
                goto LAB_004b1839;
            }
            fVar9 = (fVar4 * pWidth + fVar9) / fVar5;
            fVar5 = fVar5 / (fVar4 - gFLOAT_0058a644);
            fVar9 = fVar5 * pWidth
                    + sqrtf(((fVar5 * fVar5 * pWidth - fVar9 * fVar5 * gFLOAT_0058a68c) - pWidth) * pWidth)
                    + fVar9;
            fVar3 = fVar9 * fVar9 * gFLOAT_0058a690;
            pCorner->field_0x0c = fVar9;
            if (distance_from_previous_sq < fVar3) {
                pCorner->field_0x0c = (float)sqrt(distance_from_previous_sq) * gFLOAT_0058a694;
            }
            if (section_length_sq < pCorner->field_0x0c * pCorner->field_0x0c * gFLOAT_0058a690) {
                pCorner->field_0x0c = (float)sqrt(section_length_sq) * gFLOAT_0058a694;
            }
            fVar5 = fVar5 * pCorner->field_0x0c;
            pCorner->field_0x08 = fVar5;
            if (pCorner->left_not_right == 0) {
                pCorner->field_0x00.v[0] = fVar1 * pCorner->field_0x0c + fVar2 * fVar5;
                pCorner->field_0x00.v[1] = fVar2 * pCorner->field_0x0c + -pCorner->field_0x08 * fVar1;
            } else {
                pCorner->field_0x00.v[0] = fVar5 * -fVar2 + fVar1 * pCorner->field_0x0c;
                pCorner->field_0x00.v[1] = fVar2 * pCorner->field_0x0c + fVar1 * pCorner->field_0x08;
            }
            fVar7 = (float)sqrt(section_length_sq);
            pCorner->field_0x00.v[0] = pCorner->field_0x00.v[0] / fVar7 + local_start.v[0];
            pCorner->field_0x00.v[1] = pCorner->field_0x00.v[1] / fVar7 + local_start.v[1];
            goto LAB_004b187e;
LAB_004b1839:
            pCorner->field_0x00.v[0] = local_start.v[0];
            pCorner->field_0x00.v[1] = local_start.v[1];
            if (pWidth <= fVar9) {
                pCorner->field_0x08 = pWidth;
            } else {
                pCorner->field_0x08 = fVar9;
            }
            pCorner->field_0x0c = pCorner->field_0x08 * gFLOAT_0058a698;
            pCorner->left_not_right = IRandomBetween(0, 1);
LAB_004b187e:
            iVar10 = iVar10 + 1;
            pCorner++;
            local_10 = local_start.v[0];
            local_c = local_start.v[1];
        } while (pSection != -1);
    }
    return iVar10;
}

// FUNCTION: CARMA2_HW 0x004b1ab0
int C2_HOOK_FASTCALL CalcSOCs(int pNext_section, int pCount_corners, tCorner *pCorners, tOpponent_spec *pOpponent_spec, tSOC* pSocs, tCar_spec* pCar) {
    int count_socs;
    int i;

    count_socs = 0;
    for (i = 0; i < pCount_corners; i++) {
        if (pNext_section != -1) {
            while (pNext_section != -1) {
                const br_vector3* start;
                const br_vector3* finish;
                float distance;
                tSOC* pSoc;

                if (count_socs >= 10) {
                    goto next_corner;
                }
                pSoc = &pSocs[count_socs];
                if (pNext_section == pCorners[i].section) {
                    break;
                }
                start = GetOpponentsSectionStartNodePoint(pOpponent_spec, (tS16)pCorners[i].section);
                finish = GetOpponentsSectionFinishNodePoint(pOpponent_spec, pNext_section);
                distance = sqrtf(
                    (finish->v[0] - start->v[0]) * (finish->v[0] - start->v[0])
                    + (finish->v[2] - start->v[2]) * (finish->v[2] - start->v[2]));
                if (distance <= pCorners[i].field_0x0c) {
                    break;
                }
                pSoc->field_0x0 |= 1;
                pSoc->field_0x4 = pNext_section;
                count_socs++;
                pNext_section = GetOpponentsNextSection(pOpponent_spec, pNext_section);
            }
        }
        if (count_socs < 10) {
            pSocs[count_socs].field_0x0 &= ~1;
            pSocs[count_socs].field_0x4 = i;
            pCorners[i].field_0x18 = (undefined4)sqrtf(pCorners[i].field_0x08 * 6.9f * 7.5f * 1.96f);
            if (pNext_section != pCorners[i].section) {
                do {
                    if ((int)pCorners[i].field_0x18 > GetOpponentsSectionMaxSpeed(pOpponent_spec, pNext_section, 1)) {
                        pCorners[i].field_0x18 = GetOpponentsSectionMaxSpeed(pOpponent_spec, pNext_section, 1);
                    }
                    pNext_section = GetOpponentsNextSection(pOpponent_spec, pNext_section);
                } while (pNext_section != pCorners[i].section);
            }
            while (pNext_section != -1) {
                const br_vector3* start;
                const br_vector3* finish;
                float distance;

                start = GetOpponentsSectionStartNodePoint(pOpponent_spec, (tS16)pCorners[i].section);
                finish = GetOpponentsSectionFinishNodePoint(pOpponent_spec, pNext_section);
                distance = sqrtf(
                    (finish->v[0] - start->v[0]) * (finish->v[0] - start->v[0])
                    + (finish->v[2] - start->v[2]) * (finish->v[2] - start->v[2]));
                if (distance >= pCorners[i].field_0x0c) {
                    break;
                }
                if ((int)pCorners[i].field_0x18 > GetOpponentsSectionMaxSpeed(pOpponent_spec, pNext_section, 1)) {
                    pCorners[i].field_0x18 = GetOpponentsSectionMaxSpeed(pOpponent_spec, pNext_section, 1);
                }
                pNext_section = GetOpponentsNextSection(pOpponent_spec, pNext_section);
            }
            count_socs++;
        }
next_corner:
        ;
    }
    return count_socs;
}

#define DRAW_DIAL_FRAME(pY) \
    RemoveTransientBitmaps(1); \
    DRPixelmapRectangleMaskedCopy(gBack_screen, \
        *(br_int_16*)((int*)&gCurrent_graf_data->field_0x114 + pDial_index), \
        *(br_int_16*)((int*)&gCurrent_graf_data->field_0x11c + pDial_index), \
        gDials_pix, 0, (pY), gDials_pix->width, 64); \
    ProcessFlicQueue(gFrame_period); \
    DoMouseCursor(); \
    PDScreenBufferSwap(0)

// FUNCTION: CARMA2_HW 0x004b1d80
void C2_HOOK_FASTCALL DrawDial(int pDial_index, volatile int pFrom_value, int pTo_value) {
    int initial_time;
    int interval_start;
    int elapsed;
    int frame_value;
    tU32 elapsed_u;

    DRAW_DIAL_FRAME(pFrom_value * 64);
    initial_time = PDGetTotalTime();
    elapsed = PDGetTotalTime();
    for (elapsed = elapsed - initial_time; elapsed < 100; elapsed = elapsed - initial_time) {
        frame_value = pFrom_value + elapsed * (pTo_value - pFrom_value) / 100;
        DRAW_DIAL_FRAME(frame_value * 64);
        elapsed = PDGetTotalTime();
    }
    DRAW_DIAL_FRAME(pTo_value * 64);

    interval_start = PDGetTotalTime();
    frame_value = pTo_value < 0x18 ? pTo_value + 1 : 0x16;
    DRAW_DIAL_FRAME(frame_value * 64);
    elapsed_u = (tU32)(PDGetTotalTime() - interval_start);
    while (elapsed_u < 0x14) {
        elapsed_u = (tU32)(PDGetTotalTime() - interval_start);
    }

    interval_start = PDGetTotalTime();
    frame_value = pTo_value == 0 ? 2 : pTo_value - 1;
    DRAW_DIAL_FRAME(frame_value * 64);
    elapsed_u = (tU32)(PDGetTotalTime() - interval_start);
    while (elapsed_u < 0x14) {
        elapsed_u = (tU32)(PDGetTotalTime() - interval_start);
    }

    interval_start = PDGetTotalTime();
    frame_value = pTo_value < 0x18 ? pTo_value + 1 : 0x16;
    DRAW_DIAL_FRAME(frame_value * 64);
    elapsed_u = (tU32)(PDGetTotalTime() - interval_start);
    while (elapsed_u < 0x14) {
        elapsed_u = (tU32)(PDGetTotalTime() - interval_start);
    }

    interval_start = PDGetTotalTime();
    DRAW_DIAL_FRAME(initial_time);
    elapsed_u = (tU32)(PDGetTotalTime() - interval_start);
    while (elapsed_u < 0x14) {
        elapsed_u = (tU32)(PDGetTotalTime() - interval_start);
    }

    interval_start = PDGetTotalTime();
    frame_value = pTo_value == 0 ? 2 : pTo_value - 1;
    DRAW_DIAL_FRAME(frame_value * 64);
    elapsed_u = (tU32)(PDGetTotalTime() - interval_start);
    while (elapsed_u < 0x14) {
        elapsed_u = (tU32)(PDGetTotalTime() - interval_start);
    }

    DRAW_DIAL_FRAME(initial_time);
}

#undef DRAW_DIAL_FRAME

// GLOBAL: CARMA2_HW 0x0075bbf0
int gUNK_0075bbf0;

// GLOBAL: CARMA2_HW 0x0075bbf4
int gUNK_0075bbf4;

// FUNCTION: CARMA2_HW 0x004b1ca0
void C2_HOOK_FASTCALL FUN_004b1ca0(void) {

    RemoveTransientBitmaps(1);
    DRPixelmapRectangleMaskedCopy(gBack_screen, gCurrent_graf_data->field_0x114, gCurrent_graf_data->field_0x11c,
        gDials_pix, 0, 0, gDials_pix->width, 64);
    ProcessFlicQueue(gFrame_period);
    DoMouseCursor();
    PDScreenBufferSwap(0);
    RemoveTransientBitmaps(1);
    DRPixelmapRectangleMaskedCopy(gBack_screen, gCurrent_graf_data->field_0x118, gCurrent_graf_data->field_0x120,
        gDials_pix, 0, 0, gDials_pix->width, 64);
    ProcessFlicQueue(gFrame_period);
    DoMouseCursor();
    PDScreenBufferSwap(0);
    DrawDial(0, 0, gUNK_0075bbf0 * 4);
    DrawDial(1, 0, gUNK_0075bbf4 * 4);
}

// FUNCTION: CARMA2_HW 0x004b25c0
int C2_HOOK_FASTCALL FUN_004b25c0(int* pArg1, int* pArg2) {
    if (*pArg2 == 0) {
        *pArg2 = 1;
        *pArg1 = 13;
    } else if (*pArg1 == 1) {
        *pArg2 = 0;
        *pArg1 = 0;
    } else {
        goto failed;
    }
    DRS3StartSound(gEffects_outlet, eSoundId_LeftButton);
    return 1;

failed:
    return 0;
}

// GLOBAL: CARMA2_HW 0x00691788
int gUNK_00691788;

// GLOBAL: CARMA2_HW 0x006918cc
tU8* gUNK_006918cc;

// GLOBAL: CARMA2_HW 0x0065bd78
tDial_flic_option gDial_flic_options[13] = {
    { 4, 36, 0, { 132, 175, 221, 253, 0 } },
    { 3, 45, 0, { 132, 157, 217, 0, 0 } },
    { 3, 54, 0, { 132, 183, 236, 0, 0 } },
    { 3, 67, 0, { 132, 194, 234, 0, 0 } },
    { 3, 76, 0, { 132, 194, 234, 0, 0 } },
    { 4, 89, 0, { 132, 176, 204, 246, 0 } },
    { 4, 98, 0, { 132, 176, 204, 246, 0 } },
    { 2, 111, 0, { 132, 158, 0, 0, 0 } },
    { 2, 120, 0, { 132, 158, 0, 0, 0 } },
    { 2, 129, 0, { 132, 158, 0, 0, 0 } },
    { 2, 138, 0, { 132, 158, 0, 0, 0 } },
    { 3, 150, 0, { 132, 164, 207, 0, 0 } },
    { 4, 153, 0, { 177, 199, 220, 242, 0 } },
};

// GLOBAL: CARMA2_HW 0x00768f68
int gUNK_00768f68;

// C2PORT-NONMATCHING: best 72.16% | difftest PASS 2000 trials (spec difftest/specs/004b24f0_FUN_004b24f0.json) | reason register-allocation/struct-interior-operands
// FUNCTION: CARMA2_HW 0x004b24f0
int C2_HOOK_FASTCALL FUN_004b24f0(int* pDial_index) {
    int dial_value;
    int current_selection;
    int next_selection;

    DRS3StartSound(gEffects_outlet, 3000);
    dial_value = *pDial_index;
    next_selection = gDial_flic_options[dial_value - 2].selected + 1;
    if (next_selection == gDial_flic_options[dial_value - 2].count) {
        next_selection = 0;
    }
    current_selection = gDial_flic_options[dial_value - 2].selected;
    RemoveTransientBitmaps(1);
    DontLetFlicFuckWithPalettes();
    TurnFlicTransparencyOn();
    RunFlicAt(0x11f, gDial_flic_options[dial_value - 2].flic_frames[current_selection], gDial_flic_options[dial_value - 2].y);
    TurnFlicTransparencyOff();
    LetFlicFuckWithPalettes();
    RemoveTransientBitmaps(1);
    DontLetFlicFuckWithPalettes();
    TurnFlicTransparencyOn();
    RunFlicAt(0x120, gDial_flic_options[dial_value - 2].flic_frames[next_selection], gDial_flic_options[dial_value - 2].y);
    TurnFlicTransparencyOff();
    LetFlicFuckWithPalettes();
    gDial_flic_options[dial_value - 2].selected = next_selection;
    return 1;
}
// C2PORT-NONMATCHING: best 51.16% | difftest PASS 2000 trials (spec difftest/specs/004b2740_FUN_004b2740.json) | reason unknown-source-shape
// FUNCTION: CARMA2_HW 0x004b2740
int C2_HOOK_FASTCALL FUN_004b2740(int* pArg1) {
    int idx;
    int cur;
    int next;
    tDial_flic_option* dial;

    DRS3StartSound(gEffects_outlet, 3000);
    idx = *pArg1 - 2;
    dial = &gDial_flic_options[idx];
    next = dial->selected + 1;
    if (next == dial->count) {
        next = 0;
    }
    cur = dial->selected;
    RemoveTransientBitmaps(1);
    DontLetFlicFuckWithPalettes();
    TurnFlicTransparencyOn();
    RunFlicAt(0x11f, dial->flic_frames[cur], dial->y);
    TurnFlicTransparencyOff();
    LetFlicFuckWithPalettes();
    RemoveTransientBitmaps(1);
    DontLetFlicFuckWithPalettes();
    TurnFlicTransparencyOn();
    RunFlicAt(0x120, dial->flic_frames[next], dial->y);
    TurnFlicTransparencyOff();
    LetFlicFuckWithPalettes();
    dial->selected = next;
    return 0;
}

// C2PORT-NONMATCHING: best 79.17% | difftest PASS 2000 trials (spec difftest/specs/004b2430_FUN_004b2430.json) | reason register-allocation
// FUNCTION: CARMA2_HW 0x004b2430
int C2_HOOK_FASTCALL FUN_004b2430(int* pDial_index) {
    int dial_value;
    int previous_selection;
    int selection;

    DRS3StartSound(gEffects_outlet, 3000);
    dial_value = *pDial_index;
    selection = gDial_flic_options[dial_value - 2].selected - 1;
    if (selection < 0) {
        selection = gDial_flic_options[dial_value - 2].count - 1;
    }
    previous_selection = gDial_flic_options[dial_value - 2].selected;
    RemoveTransientBitmaps(1);
    DontLetFlicFuckWithPalettes();
    TurnFlicTransparencyOn();
    RunFlicAt(0x11f, gDial_flic_options[dial_value - 2].flic_frames[previous_selection], gDial_flic_options[dial_value - 2].y);
    TurnFlicTransparencyOff();
    LetFlicFuckWithPalettes();
    RemoveTransientBitmaps(1);
    DontLetFlicFuckWithPalettes();
    TurnFlicTransparencyOn();
    RunFlicAt(0x120, gDial_flic_options[dial_value - 2].flic_frames[selection], gDial_flic_options[dial_value - 2].y);
    TurnFlicTransparencyOff();
    LetFlicFuckWithPalettes();
    gDial_flic_options[dial_value - 2].selected = selection;
    return 1;
}

// C2PORT-NONMATCHING: best 78.87% | difftest PASS 2000 trials (spec difftest/specs/004b2640_FUN_004b2640.json) | reason vc5-loop-register-layout-and-struct-interior-operands
// FUNCTION: CARMA2_HW 0x004b2640
int C2_HOOK_FAKE_THISCALL FUN_004b2640(int* pDial_index, undefined4 pArg2, int pX_offset, undefined4 pArg3) {
    int dial_index;
    int selection;
    int previous_selection;
    int left_edge;
    int* pFrame;
    tU8* pGraf_data;

    CARPOCALYPSE2_THISCALL_UNUSED(pArg2);
    CARPOCALYPSE2_THISCALL_UNUSED(pArg3);
    dial_index = *pDial_index + -2;
    selection = gDial_flic_options[dial_index].count - 1;
    if (selection >= 0) {
        pGraf_data = *(tU8**)((tU8*)gUNK_006918cc + 0x120);
        left_edge = *(int*)(pGraf_data + gReal_graf_data_index * 4 + 0x60) + pX_offset + 3;
        pFrame = &gDial_flic_options[dial_index].flic_frames[selection];
        while (left_edge < *pFrame) {
            selection--;
            pFrame--;
            if (selection < 0) {
                return 0;
            }
        }

        DRS3StartSound(gEffects_outlet, 3000);
        dial_index = *pDial_index - 2;
        previous_selection = gDial_flic_options[dial_index].selected;
        RemoveTransientBitmaps(1);
        DontLetFlicFuckWithPalettes();
        TurnFlicTransparencyOn();
        RunFlicAt(0x11f, gDial_flic_options[dial_index].flic_frames[previous_selection], gDial_flic_options[dial_index].y);
        TurnFlicTransparencyOff();
        LetFlicFuckWithPalettes();
        RemoveTransientBitmaps(1);
        DontLetFlicFuckWithPalettes();
        TurnFlicTransparencyOn();
        RunFlicAt(0x120, gDial_flic_options[dial_index].flic_frames[selection], gDial_flic_options[dial_index].y);
        TurnFlicTransparencyOff();
        LetFlicFuckWithPalettes();
        gDial_flic_options[dial_index].selected = selection;
    }
    return 0;
}

// FUNCTION: CARMA2_HW 0x004b2800
void C2_HOOK_FASTCALL FUN_004b2800(int pIndex) {
    tU8* pGraf_data;

    if (gUNK_00691788 != pIndex) {
        if (gUNK_00691788 - 2 >= 0) {
            pGraf_data = *(tU8**)((tU8*)gUNK_006918cc + 0x120);
            DrawRectangleOutline(gBack_screen,
                *(int*)(pGraf_data + gReal_graf_data_index * 4 + 0x60) - 6,
                gDial_flic_options[gUNK_00691788 - 2].y - 3,
                *(int*)(pGraf_data + gReal_graf_data_index * 4 + 0x70) + 3,
                gUNK_00768f68 + 2 + gDial_flic_options[gUNK_00691788 - 2].y, 0);
        }
        if (pIndex - 2 >= 0) {
            pGraf_data = *(tU8**)((tU8*)gUNK_006918cc + 0x120);
            DrawRectangleOutline(gBack_screen,
                *(int*)(pGraf_data + gReal_graf_data_index * 4 + 0x60) - 6,
                gDial_flic_options[pIndex - 2].y - 3,
                *(int*)(pGraf_data + gReal_graf_data_index * 4 + 0x70) + 3,
                gUNK_00768f68 + 2 + gDial_flic_options[pIndex - 2].y, 0x2d);
        }
        gUNK_00691788 = pIndex;
    }
}
