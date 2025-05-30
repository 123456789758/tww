/**
 * d_a_obj_iceisland.cpp
 * Object - Icy clouds surrounding Ice Ring Isle
 */

#include "d/actor/d_a_obj_iceisland.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "d/d_com_inf_game.h"
#include "d/res/res_gicel.h"
#include "JSystem/JKernel/JKRExpHeap.h"

/* 00000078-00000098       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return static_cast<daObjIceisland_c*>(i_this)->CreateHeap();
}

/* 00000098-00000338       .text CreateHeap__16daObjIceisland_cFv */
BOOL daObjIceisland_c::CreateHeap() {
    mDoExt_getGameHeap()->check();
    
    J3DModelData* model_data = (J3DModelData*)(dComIfG_getObjectRes("GiceL", GICEL_BDL_GICEL00));
    JUT_ASSERT(0x66, model_data != NULL);
    mpModel = mDoExt_J3DModel__create(model_data, 0, 0x11020203);

    J3DAnmTextureSRTKey* btk1 = static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes("GiceL", GICEL_BTK_GICEL00_01));
    JUT_ASSERT(0x6D, btk1 != NULL);
    int result1 = mBtkAnm1.init(model_data, btk1, true, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false, 0);

    J3DAnmTextureSRTKey* btk2 = static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes("GiceL", GICEL_BTK_GICEL00_02));
    JUT_ASSERT(0x73, btk2 != NULL);
    int result2 = mBtkAnm2.init(model_data, btk2, true, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false, 0);

    J3DAnmTevRegKey * brk = static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("GiceL", GICEL_BRK_GICEL00));
    JUT_ASSERT(0x7A, brk != NULL);
    int result3 = mBrkAnm.init(model_data, brk, true, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false, 0);
    return((mpModel != NULL) && (result1 != 0) && (result2 != 0) && (result3 != 0));  
}

/* 00000338-00000410       .text daObjIceisland_particle_set__16daObjIceisland_cFv */
void daObjIceisland_c::daObjIceisland_particle_set() {
    if(mEmitter1 == NULL) {
        mEmitter1 = dComIfGp_particle_set(dPa_name::ID_SCENE_81AA, &current.pos, &current.angle);
    }
    if(mEmitter2 == NULL) {
        mEmitter2 = dComIfGp_particle_set(dPa_name::ID_SCENE_81AB, &current.pos, &current.angle);
    }
}

/* 00000410-00000588       .text CreateInit__16daObjIceisland_cFv */
void daObjIceisland_c::CreateInit() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeFar(this, 1.0f);
    set_mtx();
    dKy_tevstr_init(&mTevStr, fopAcM_GetHomeRoomNo(this), 0xFF);
    g_env_light.settingTevStruct(TEV_TYPE_BG0, &current.pos, &mTevStr);
    mEmitter1 = NULL;
    mEmitter2 = NULL;
    u8 switchNo = daObjIceisland_prm::getSwitchNo(this);
    if(fopAcM_isSwitch(this, switchNo)){
        mBrkAnm.setFrame(mBrkAnm.getEndFrame());
        if(dComIfGs_getStartPoint() == 2 && current.roomNo == dComIfGs_getRestartRoomNo()){
            field_0x39C = 6;
        }else {
            field_0x39C = 3;
        }
        field_0x396 = 0;
    } else{
        daObjIceisland_particle_set();
        field_0x39C = 0;
        field_0x396 = 100;
    }
    mMeltIceEventIdx = dComIfGp_evmng_getEventIdx("MELT_ICE");
    mFreezeIceEventIdx = dComIfGp_evmng_getEventIdx("FREEZE_ICE");
}

/* 00000588-00000608       .text set_mtx__16daObjIceisland_cFv */
void daObjIceisland_c::set_mtx() {
    mpModel->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

/* 00000608-0000067C       .text daObjIceisland_freeze_main__16daObjIceisland_cFv */
void daObjIceisland_c::daObjIceisland_freeze_main() {
    u8 switchNo = daObjIceisland_prm::getSwitchNo(this);
    if(fopAcM_isSwitch(this, switchNo)){
        fopAcM_orderOtherEventId(this, mMeltIceEventIdx);
        field_0x39C = 1;
    }
}

/* 0000067C-00000774       .text daObjIceisland_melt_demo_wait__16daObjIceisland_cFv */
void daObjIceisland_c::daObjIceisland_melt_demo_wait() {
    if(eventInfo.mCommand == dEvtCmd_INDEMO_e){
        mDoAud_seStart(JA_SE_READ_RIDDLE_1);
        JPABaseEmitter* emitter = mEmitter1;
        if(emitter != NULL) {
            emitter->becomeInvalidEmitter();
            mEmitter1 = NULL;
        }
        emitter = mEmitter2;
        if(emitter != NULL){
            emitter->becomeInvalidEmitter();
            mEmitter2 = NULL;
        }
        mBrkAnm.setPlaySpeed(1.0f);
        field_0x39C = 2;
    } else {
        fopAcM_orderOtherEventId(this, mMeltIceEventIdx);
    }
}

/* 00000774-000007DC       .text daObjIceisland_melt_demo__16daObjIceisland_cFv */
void daObjIceisland_c::daObjIceisland_melt_demo() {
    mBrkAnm.play();
    if (dComIfGp_evmng_endCheck(mMeltIceEventIdx)) {
        dComIfGp_event_reset();
        field_0x39C = 3;
    }
}

/* 000007DC-00000850       .text daObjIceisland_melt_main__16daObjIceisland_cFv */
void daObjIceisland_c::daObjIceisland_melt_main() {
    u8 switchNo = daObjIceisland_prm::getSwitchNo(this);
    if(!fopAcM_isSwitch(this, switchNo)){
        fopAcM_orderOtherEventId(this, mFreezeIceEventIdx);
        field_0x39C = 4;
    }
}

/* 00000850-000008C0       .text daObjIceisland_freeze_demo_wait__16daObjIceisland_cFv */
void daObjIceisland_c::daObjIceisland_freeze_demo_wait() {
    if(eventInfo.mCommand == dEvtCmd_INDEMO_e){
        daObjIceisland_particle_set();
        mBrkAnm.setPlaySpeed(-1.0f);
        field_0x39C = 5;
    }
    else {
        fopAcM_orderOtherEventId(this, mFreezeIceEventIdx);
    }
}

/* 000008C0-00000928       .text daObjIceisland_freeze_demo__16daObjIceisland_cFv */
void daObjIceisland_c::daObjIceisland_freeze_demo() {
    mBrkAnm.play();
    if (dComIfGp_evmng_endCheck(mFreezeIceEventIdx)) {
        dComIfGp_event_reset();
        field_0x39C = 0;
    }
}

/* 00000928-00000994       .text daObjIceisland_fail_demo_wait__16daObjIceisland_cFv */
void daObjIceisland_c::daObjIceisland_fail_demo_wait() {
    u8 switchNo = daObjIceisland_prm::getSwitchNo(this);
    if(!fopAcM_isSwitch(this, switchNo)){
        daObjIceisland_particle_set();
        mBrkAnm.setPlaySpeed(1.0f);
        field_0x39C = 7;
    }
}

/* 00000994-000009B8       .text daObjIceisland_fail_demo_main__16daObjIceisland_cFv */
void daObjIceisland_c::daObjIceisland_fail_demo_main() {
    mBrkAnm.play();
}

cPhs_State daObjIceisland_c::_create(){
    fopAcM_SetupActor(this, daObjIceisland_c);
    cPhs_State phase_state = dComIfG_resLoad(&mPhs, "GiceL");
    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, 0x13D0)) {
            return cPhs_ERROR_e;
        } else {
            CreateInit();
        }
    }
    return phase_state;
}

bool daObjIceisland_c::_delete(){
    dComIfG_resDelete(&mPhs, "GiceL");
    return true;
}

bool daObjIceisland_c::_draw(){
    g_env_light.settingTevStruct(TEV_TYPE_BG0, &current.pos, &mTevStr);
    g_env_light.settingTevStruct(TEV_TYPE_BG2, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mpModel, &tevStr);
    mBtkAnm1.entry(mpModel->getModelData());
    mBtkAnm2.entry(mpModel->getModelData());
    mBrkAnm.entry(mpModel->getModelData());
    mDoExt_modelUpdateDL(mpModel);
    return true;
}

bool daObjIceisland_c::_execute(){
    switch(field_0x39C) {
        case 0:
            daObjIceisland_freeze_main();
            break;
        case 1:
            daObjIceisland_melt_demo_wait();
            break;
        case 2:
            daObjIceisland_melt_demo();
            break;
        case 3:
            daObjIceisland_melt_main();
            break;
        case 4:
            daObjIceisland_freeze_demo_wait();
            break;
        case 5:
            daObjIceisland_freeze_demo();
            break;
        case 6:
            daObjIceisland_fail_demo_wait();
            break;
        case 7:
            daObjIceisland_fail_demo_main();
            break;
    }
    mBtkAnm1.play();
    mBtkAnm2.play();
    set_mtx();
    u8 switchNo = daObjIceisland_prm::getSwitchNo(this);
    if(fopAcM_isSwitch(this, switchNo)){
        if(field_0x396 > 0){
            field_0x396--;
        }
    } else if(field_0x396 < 100){
        field_0x396++;
    }
    fopAcM_seStartCurrent(this, JA_SE_ATM_ICEBERG_WIND, field_0x396);
    JPABaseEmitter* emitter = mEmitter1;
    if(emitter != NULL) {
        
        u8 colorK0R = (mTevStr.mColorK0.r / 2) + 0x80;
        u8 colorK0G = (mTevStr.mColorK0.g / 2) + 0x80;
        u8 colorK0B = (mTevStr.mColorK0.b / 2) + 0x80;

        emitter->setGlobalPrmColor(colorK0R, colorK0G, colorK0B);
        emitter = mEmitter1;
        emitter->setGlobalEnvColor(colorK0R, colorK0G, colorK0B);
        
    }
    return true;
}

/* 000009B8-00000B2C       .text daObjIceisland_Create__FPv */
static cPhs_State daObjIceisland_Create(void* i_this) {
    return static_cast<daObjIceisland_c*>(i_this)->_create();
}

/* 00000C2C-00000C5C       .text daObjIceisland_Delete__FPv */
static BOOL daObjIceisland_Delete(void* i_this) {
    return static_cast<daObjIceisland_c*>(i_this)->_delete();
}

/* 00000C5C-00000D2C       .text daObjIceisland_Draw__FPv */
static BOOL daObjIceisland_Draw(void* i_this) {
    return static_cast<daObjIceisland_c*>(i_this)->_draw();
}

/* 00000D2C-00000EC0       .text daObjIceisland_Execute__FPv */
static BOOL daObjIceisland_Execute(void* i_this) {
    return static_cast<daObjIceisland_c*>(i_this)->_execute();
}

/* 00000EC0-00000EC8       .text daObjIceisland_IsDelete__FPv */
static BOOL daObjIceisland_IsDelete(void*) {
    return TRUE;
}

static actor_method_class daObj_IceislandMethodTable = {
    (process_method_func)daObjIceisland_Create,
    (process_method_func)daObjIceisland_Delete,
    (process_method_func)daObjIceisland_Execute,
    (process_method_func)daObjIceisland_IsDelete,
    (process_method_func)daObjIceisland_Draw,
};

actor_process_profile_definition g_profile_Obj_Iceisland = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0003,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_Obj_Iceisland,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjIceisland_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_Obj_Iceisland,
    /* Actor SubMtd */ &daObj_IceislandMethodTable,
    /* Status       */ fopAcStts_UNK4000_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
