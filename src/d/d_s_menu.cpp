/**
 * d_s_menu.cpp
 * Debug Map Select menu
 */

#include "d/d_s_menu.h"
#include "JSystem/JFramework/JFWDisplay.h"
#include "JSystem/JFramework/JFWSystem.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTDbPrint.h"
#include "JSystem/JUtility/JUTReport.h"
#include "d/d_com_inf_game.h"
#include "d/d_com_lib_game.h"
#include "d/d_procname.h"
#include "f_ap/f_ap_game.h"
#include "f_op/f_op_scene_mng.h"
#if VERSION == VERSION_PAL
#include "m_Do/m_Do_MemCard.h"
#endif
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_dvd_thread.h"
#include "m_Do/m_Do_graphic.h"
#include "m_Do/m_Do_main.h"
#if VERSION == VERSION_PAL
#include "stdio.h"
#endif

s32 l_startID;
s32 l_cursolID;
int l_timepat;
s16 l_weekpat;
s16 l_demo23;
s8* l_groupPoint;
u8 l_languageType;

#if VERSION == VERSION_PAL
static mDoDvdThd_mountXArchive_c* l_bmgData2;
static int mBmgStatus2;

static void dScnMenu_bmg_data_set() {
    if (mBmgStatus2 == 0) {
        dComIfGp_getMsgDtArchive()->unmount();
        char sp08[40];
        sprintf(sp08, "/res/Msg/data%d/bmgres.arc", dComIfGs_getPalLanguage());
        l_bmgData2 = mDoDvdThd_mountXArchive_c::create(sp08, 0, JKRArchive::MOUNT_MEM);
        mBmgStatus2 = 1;
    }
    if (mBmgStatus2 == 1 && l_bmgData2->sync()) {
        mBmgStatus2 = 2;
    }
    if (mBmgStatus2 == 2) {
        dComIfGp_setMsgDtArchive(l_bmgData2->getArchive());
        delete l_bmgData2;
        mBmgStatus2 = 3;
    }
}

static void dScnMenu_tex_data_set() {
    if (mBmgStatus2 == 3) {
        dComIfGp_getActionIconArchive()->unmount();
        char sp08[40];
        sprintf(sp08, "/res/Msg/data%d/acticon.arc", dComIfGs_getPalLanguage());
        l_bmgData2 = mDoDvdThd_mountXArchive_c::create(sp08, 0, JKRArchive::MOUNT_ARAM);
        mBmgStatus2 = 4;
    }
    if (mBmgStatus2 == 4 && l_bmgData2->sync()) {
        mBmgStatus2 = 5;
    }
    if (mBmgStatus2 == 5) {
        dComIfGp_setActionIconArchive(l_bmgData2->getArchive());
        delete l_bmgData2;
        mBmgStatus2 = 6;
    }
}
#endif

/* 8022E9F4-8022ED50       .text dScnMenu_Draw__FP19menu_of_scene_class */
static BOOL dScnMenu_Draw(menu_of_scene_class* i_this) {
    JUTReport(300, 50, "メニュー");
    if (i_this->startCode) {
        JUTReport(400, 50,"<%d>", i_this->startCode - 1);
    }
    menu_of_scene_class::menu_inf* info = i_this->info;
    s32 y = 70;
    u8 num;
    s32 scroll = l_cursolID - l_startID;
    num = info->num;
    s32 lineNum = num < 20 ? num : 20;
    if (scroll < 5) {
        l_startID += scroll - 5;
        if (l_startID < 0) {
            l_startID = 0;
        }
    } else if (scroll > 15) {
        int r5 = num - 20;
        if (r5 < 0) {
            r5 = 0;
        }
        l_startID += scroll - 15;
        if (l_startID > r5) {
            l_startID = r5;
        }
    }
    for (s32 id = l_startID, i = 0; i < lineNum; id++, y += 16, i++) {
        JUTReport(20, y, "%c %2d %s　＜%s＞", l_cursolID == id ? (s8)79 : (s8)32, id, info->stage[id].name, info->stage[id].roomPtr[l_groupPoint[id]].name);
    }
    JUTReport(280,400,"Ｘ：進む　Ｙ：戻る");
    char* timepat_str[] = {"通常", "高速経過", "朝（あさ）に固定", "昼（ひる）に固定", "夕方（ゆうがた）に固定", "夜（よる）に固定", "時に固定"};
    char* weekpat_str[] = {"日曜日", "月曜日", "火曜日", "水曜日", "木曜日", "金曜日", "土曜日"};
    if (l_timepat >= 6) {
        JUTReport(280, 420, "時刻：%d%s", l_timepat - 6, timepat_str[6]);
    } else {
        JUTReport(280, 420, "時刻：%s", timepat_str[l_timepat]);
    }
    JUTReport(40, 420, "十字右：進む　十字左：戻る");
    JUTReport(200, 400, "曜日：%s", weekpat_str[l_weekpat]);
#if VERSION > VERSION_JPN
    static const char* language[] = {"ENGLISH", "GERMAN", "FRENCH", "SPANISH", "ITALIAN"};
    JUTReport(40, 440, "%s", language[dComIfGs_getPalLanguage()]);
#endif
    if (dComIfGs_isEventBit(0x2d01)) {
        JUTReport(400, 420, "３コン（Ａ）:デモ２３　ＯＮ");
    } else {
        JUTReport(400, 420, "３コン（Ａ）:デモ２３　ＯＦＦ");
    }
    JUTReport(36, 40, "NDEBUG %s %s", mDoMain::COPYDATE_STRING, "FINAL");
    return true;
}

/* 8022ED50-8022F318       .text dScnMenu_Execute__FP19menu_of_scene_class */
static BOOL dScnMenu_Execute(menu_of_scene_class* i_this) {
    menu_of_scene_class::menu_inf* info = i_this->info;
    if (CPad_CHECK_HOLD_UP(0) || CPad_CHECK_HOLD_DOWN(0)) {
        BOOL trig = TRUE;
        if (CPad_CHECK_TRIG_UP(0) || CPad_CHECK_TRIG_DOWN(0)) {
            i_this->field_0x1e1 = 20;
        } else {
            if (cLib_calcTimer(&i_this->field_0x1e1) == 0)
                i_this->field_0x1e1 = 4;
            else
                trig = FALSE;
        }

        if (trig) {
            if (CPad_CHECK_HOLD_UP(0)) {
                if (--l_cursolID < 0)
                    l_cursolID = info->num - 1;
            } else {
                if (++l_cursolID >= info->num)
                    l_cursolID = 0;
            }
        }
    }

    if (CPad_CHECK_HOLD_B(0) || CPad_CHECK_HOLD_A(0)) {
        BOOL trig = TRUE;
        if (CPad_CHECK_TRIG_B(0) || CPad_CHECK_TRIG_A(0)) {
            i_this->field_0x1e2 = 20;
        } else {
            if (cLib_calcTimer(&i_this->field_0x1e2) == 0)
                i_this->field_0x1e2 = 4;
            else
                trig = FALSE;
        }

        if (trig) {
            if (CPad_CHECK_HOLD_B(0)) {
                if (--l_groupPoint[l_cursolID] < 0)
                    l_groupPoint[l_cursolID] = info->stage[l_cursolID].roomNum - 1;
            } else {
                if (++l_groupPoint[l_cursolID] >= info->stage[l_cursolID].roomNum)
                    l_groupPoint[l_cursolID] = 0;
            }
        }
    }

    if (CPad_CHECK_TRIG_R(0)) {
        if (++i_this->startCode == 0)
            i_this->startCode++;
    } else if (CPad_CHECK_TRIG_L(0)) {
        if (--i_this->startCode == 0)
            i_this->startCode--;
    }

    if (CPad_CHECK_TRIG_START(0)) {
        menu_of_scene_class::room_inf* room = &info->stage[l_cursolID].roomPtr[l_groupPoint[l_cursolID]];
        dComIfGp_offEnableNextStage();
        s16 startCode = (i_this->startCode != 0) ? i_this->startCode - 1 : room->startCode;
        dComIfGp_setNextStage(room->stageName, startCode, room->roomNo, room->layerNo);
        if (strcmp(dComIfGp_getNextStageName(), "ENDING") == 0) {
            fopScnM_ChangeReq(i_this, PROC_ENDING_SCENE, PROC_OVERLAP0, 5);
            mDoAud_bgmStop(30);
        } else {
            fopScnM_ChangeReq(i_this, PROC_PLAY_SCENE, PROC_OVERLAP0, 5);
            dComIfGs_setRestartRoomParam(0);
            mDoAud_setSceneName(dComIfGp_getNextStageName(), dComIfGp_getNextStageRoomNo(), dComIfGp_getNextStageLayer());
        }
    }

    if (CPad_CHECK_TRIG_Y(0)) {
        if (--l_timepat < 0)
            l_timepat = 0x1d;
    } else if (CPad_CHECK_TRIG_X(0)) {
        if (++l_timepat > 0x1d)
            l_timepat = 0;
    }

    if (CPad_CHECK_TRIG_LEFT(0)) {
        if (--l_weekpat < 0)
            l_weekpat = 6;
    } else if (CPad_CHECK_TRIG_RIGHT(0)) {
        if (++l_weekpat >= 7)
            l_weekpat = 0;
    }
    dComIfGs_setDate(l_weekpat);

#if VERSION > VERSION_JPN
    static const u8 language[] = { 0, 1, 2, 3, 4, };
    if (CPad_CHECK_TRIG_Z(3)) {
        if (++l_languageType > 4)
            l_languageType = 0;
        dComIfGs_setPalLanguage(language[l_languageType]);
#if VERSION == VERSION_PAL
        g_mDoMemCd_control.field_0x165B = language[l_languageType];
#endif
    }
#endif

    if (CPad_CHECK_TRIG_A(2)) {
        l_demo23 ^= 1;
        if (l_demo23)
            dComIfGs_onEventBit(0x2d01);
        else
            dComIfGs_offEventBit(0x2d01);
    }

    g_env_light.mTimeAdv = 0.0f;
    if (l_timepat >= 6) {
        dComIfGs_setTime((l_timepat - 6) * 15.0f);
        g_env_light.mTimeAdv = 1000.0f;
    } else {
        switch (l_timepat) {
        case 0:
            dComIfGs_setTime(120.0f);
            g_env_light.mTimeAdv = 0.02f;
            break;
        case 1:
            dComIfGs_setTime(120.0f);
            g_env_light.mTimeAdv = 0.25f;
            break;
        case 2:
            dComIfGs_setTime(75.0f);
            g_env_light.mCurTime = 75.0f;
            g_env_light.mTimeAdv = 1000.0f;
            break;
        case 3:
            dComIfGs_setTime(195.0f);
            g_env_light.mCurTime = 195.0f;
            g_env_light.mTimeAdv = 1001.0f;
            break;
        case 4:
            dComIfGs_setTime(255.0f);
            g_env_light.mCurTime = 255.0f;
            g_env_light.mTimeAdv = 1002.0f;
            break;
        case 5:
            dComIfGs_setTime(315.0f);
            g_env_light.mCurTime = 315.0f;
            g_env_light.mTimeAdv = 1003.0f;
            break;
        }
    }

    return TRUE;
}

/* 8022F318-8022F320       .text dScnMenu_IsDelete__FP19menu_of_scene_class */
static BOOL dScnMenu_IsDelete(menu_of_scene_class*) {
    return true;
}

/* 8022F320-8022F3C4       .text dScnMenu_Delete__FP19menu_of_scene_class */
static BOOL dScnMenu_Delete(menu_of_scene_class* i_this) {
#if VERSION == VERSION_PAL
    dScnMenu_bmg_data_set();
    dScnMenu_tex_data_set();
    if (mBmgStatus2 < 6) {
        return false;
    }
    mBmgStatus2 = 0;
#endif
    JUTDbPrint::getManager()->changeFont(JFWSystem::systemFont);
    delete i_this->font;
    JKRFree(i_this->info);
    JKRFree(i_this->fontRes);
    g_HIO.mDisplayFlag &= ~2;
    g_HIO.mDisplayFlag &= ~2;
    dComIfGs_setRestartOption(0);
    return true;
}

/* 8022F3C4-8022F4B0       .text phase_1__FP19menu_of_scene_class */
cPhs_State phase_1(menu_of_scene_class* i_this) {
    i_this->command = mDoDvdThd_toMainRam_c::create("/res/Menu/Menu1.dat", 0, NULL);
    JUT_ASSERT(VERSION_SELECT(616, 616, 732, 732), i_this->command != NULL);
    i_this->fontCommand = mDoDvdThd_toMainRam_c::create("/res/Menu/kanfont_fix16.bfn", 0, NULL);
    JUT_ASSERT(VERSION_SELECT(619, 619, 735, 735), i_this->fontCommand != NULL);
    return cPhs_NEXT_e;
}

/* 8022F4B0-8022F70C       .text phase_2__FP19menu_of_scene_class */
cPhs_State phase_2(menu_of_scene_class* i_this) {
    if (!i_this->command->sync() || !i_this->fontCommand->sync()) {
        return cPhs_INIT_e;
    }
    i_this->info = (menu_of_scene_class::menu_inf*)i_this->command->getMemAddress();
    JUT_ASSERT(VERSION_SELECT(663, 663, 779, 779), i_this->info != NULL);
    delete i_this->command;
    menu_of_scene_class::menu_inf* info = i_this->info;
    info->stage = (menu_of_scene_class::stage_inf*)(u32(info->stage) + u32(info));
    for (int i = 0; i < info->num; i++) {
        info->stage[i].roomPtr = (menu_of_scene_class::room_inf*)((u8*)info + u32(info->stage[i].roomPtr));
    }
    if (!l_groupPoint) {
        l_groupPoint = new s8[info->num];
        JUT_ASSERT(VERSION_SELECT(676, 676, 792, 792), l_groupPoint != NULL);
        for (int i = 0; i < info->num; i++) {
            l_groupPoint[i] = 0;
        }
    }
    i_this->fontRes = (ResFONT*)i_this->fontCommand->getMemAddress();
    delete i_this->fontCommand;
    if (i_this->fontRes) {
        i_this->font = new myFontClass(i_this->fontRes, NULL);
        if (i_this->font) {
            JUTDbPrint::getManager()->changeFont(i_this->font);
        }
    }
    JFWDisplay::getManager()->setTickRate(OS_TIMER_CLOCK / 60);
    mDoGph_gInf_c::setBackColor(g_clearColor);
    g_HIO.mDisplayFlag |= 2;
    return cPhs_COMPLEATE_e;
}

/* 8022F70C-8022F748       .text dScnMenu_Create__FP11scene_class */
static cPhs_State dScnMenu_Create(scene_class* i_scn) {
    static cPhs__Handler l_method[] = {
        (cPhs__Handler)phase_1,
        (cPhs__Handler)phase_2,
    };
    menu_of_scene_class* i_this = (menu_of_scene_class *)i_scn;
#if VERSION > VERSION_JPN
    l_languageType = dComIfGs_getPalLanguage();
#endif
    return dComLbG_PhaseHandler(&i_this->mPhs, l_method, i_this);
}

/* 8022F748-8022F7A8       .text __dt__11myFontClassFv */
myFontClass::~myFontClass() {}

/* 8022F7A8-8022F7CC       .text drawChar_scale__11myFontClassFffffib */
f32 myFontClass::drawChar_scale(f32 param_1, f32 param_2, f32 param_3, f32 param_4, int param_5, bool param_6) {
    return JUTResFont::drawChar_scale(param_1, param_2, 12.0f, param_4, param_5, param_6);
}

scene_method_class l_dScnMenu_Method = {
    (process_method_func)dScnMenu_Create,
    (process_method_func)dScnMenu_Delete,
    (process_method_func)dScnMenu_Execute,
    (process_method_func)dScnMenu_IsDelete,
    (process_method_func)dScnMenu_Draw,
};

scene_process_profile_definition g_profile_MENU_SCENE = {
    /* LayerID      */ fpcLy_ROOT_e,
    /* ListID       */ 1,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_MENU_SCENE,
    /* Proc SubMtd  */ &g_fpcNd_Method.base,
    /* Size         */ sizeof(menu_of_scene_class),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Node SubMtd  */ &g_fopScn_Method.base,
    /* Scene SubMtd */ &l_dScnMenu_Method,
};
