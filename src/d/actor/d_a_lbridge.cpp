/**
 * d_a_lbridge.cpp
 * Object - Tower of the Gods - Glowing light bridge
 */

#include "d/actor/d_a_lbridge.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "d/res/res_gbrg00.h"

const char daLbridge_c::m_arcname[] = "Gbrg00";

/* 00000078-00000098       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return static_cast<daLbridge_c*>(i_this)->CreateHeap();
}

/* 00000098-00000384       .text CreateHeap__11daLbridge_cFv */
BOOL daLbridge_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(m_arcname, GBRG00_BDL_GBRG00);
    JUT_ASSERT(0xD6, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000U, 0x11000223U);

    if (!mpModel) {
        return FALSE;
    }

    J3DAnmTextureSRTKey* pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(m_arcname, GBRG00_BTK_GBRG00);
    JUT_ASSERT(0xE8, pbtk != NULL);

    if (!mBtkAnm.init(modelData, pbtk, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false, 0)) {
        return FALSE;
    }

    J3DAnmColor* pbpk = (J3DAnmColor*)dComIfG_getObjectRes(m_arcname, GBRG00_BPK_GBRG00);
    JUT_ASSERT(0xF6, pbpk != NULL);

    if (!mBpkAnm.init(modelData, pbpk, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false, 0)) {
        return FALSE;
    }

    mBpkAnm.setFrame(0.0f);
    mBpkAnm.setPlaySpeed(1.0f);

    J3DAnmTevRegKey* pbrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(m_arcname, GBRG00_BRK_GBRG00);
    JUT_ASSERT(0x106, pbrk != NULL);

    if (!mBrkAnm.init(modelData, pbrk, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false, 0)) {
        return FALSE;
    }

    setMoveBGMtx();

    mpBgW = new dBgW();

    if (mpBgW != NULL) {
        if (mpBgW->Set((cBgD_t*)dComIfG_getObjectRes(m_arcname, GBRG00_DZB_HHASHI1), cBgW::MOVE_BG_e, &mMtx) == true) {
            return FALSE;
        } else {
            return TRUE;
        }
    }

    return FALSE;
}

/* 00000384-00000544       .text CreateInit__11daLbridge_cFv */
void daLbridge_c::CreateInit() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());

    fopAcM_setCullSizeBox(this, -600.0f, -100.0f, -150.0f, 600.0f, 100.0f, 150.0f);
    fopAcM_setCullSizeFar(this, 1.5f);

    mpEmitter = dComIfGp_particle_set(dPa_name::ID_SCENE_810F, &current.pos, &current.angle);

    if (mpEmitter != NULL) {
        mpEmitter->stopDrawParticle();
        mpEmitter->setGlobalAlpha(0);
    }

    mSwitchNo = daLbridge_prm::getSwitchNo(this);

    mIsSw = (bool)fopAcM_isSwitch(this, mSwitchNo);

    mAppearEventIdx = dComIfGp_evmng_getEventIdx("EFFAPPEAR");
    mDisappearEventIdx = dComIfGp_evmng_getEventIdx("BRIDGE_DISAPPEAR");

    if (fopAcM_isSwitch(this, mSwitchNo) || mSwitchNo == 0xFF) {
        dComIfG_Bgsp()->Regist(mpBgW, this);
        set_mtx();

        mpBgW->Move();

        if (mpEmitter != NULL) {
            mpEmitter->playDrawParticle();
            mpEmitter->setGlobalAlpha(0xFF);
        }
    }
}

/* 00000544-0000063C       .text _create__11daLbridge_cFv */
cPhs_State daLbridge_c::_create() {
    fopAcM_SetupActor(this, daLbridge_c);

    cPhs_State ret = dComIfG_resLoad(&mPhs, m_arcname);

    if (ret == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, 0x2E10U)) {
            return cPhs_ERROR_e;
        }
        CreateInit();
    }

    return ret;
}

/* 00000798-00000818       .text set_mtx__11daLbridge_cFv */
void daLbridge_c::set_mtx() {
    mpModel->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

/* 00000818-00000888       .text setMoveBGMtx__11daLbridge_cFv */
void daLbridge_c::setMoveBGMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::scaleM(scale);
    mDoMtx_copy(mDoMtx_stack_c::get(), mMtx);
}

/* 00000888-00000914       .text _execute__11daLbridge_cFv */
bool daLbridge_c::_execute() {
    bool isSw = fopAcM_isSwitch(this, mSwitchNo);

    sw_check();
    demo();
    mBtkAnm.play();
    mBpkAnm.play();
    mBrkAnm.play();
    set_mtx();

    mIsSw = isSw;

    return TRUE;
}

/* 00000914-00000A6C       .text sw_check__11daLbridge_cFv */
void daLbridge_c::sw_check() {
    bool isSw = fopAcM_isSwitch(this, mSwitchNo);

    if (mSwitchNo == 0xFF) {
        return;
    }

    if (isSw != mIsSw) {
        if (isSw) {
            appear_bridge();
        } else {
            disappear_bridge();
        }
    }

    u8 alpha;

    if (!isSw) {
        if (mBpkAnm.getFrame() == (f32)mBpkAnm.getFrameCtrl()->getStart()) {
            fopAcM_offDraw(this);
        }

        if (mpEmitter != NULL) {
            alpha = mpEmitter->getGlobalAlpha();
            cLib_chaseUC(&alpha, 0x0, 0x8);
            mpEmitter->setGlobalAlpha(alpha);
        }
    } else {
        fopAcM_onDraw(this);

        cLib_calcTimer(&mTimer);

        if (mTimer == 0 && mpEmitter != NULL) {
            alpha = mpEmitter->getGlobalAlpha();
            cLib_chaseUC(&alpha, 0xFF, 0x8);
            mpEmitter->setGlobalAlpha(alpha);
        }
    }
}

/* 00000A6C-00000C18       .text demo__11daLbridge_cFv */
void daLbridge_c::demo() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        if (dComIfGp_evmng_startCheck(mAppearEventIdx) && unk31C == 1) {
            unk31C = 0;
        }

        if (dComIfGp_evmng_startCheck(mDisappearEventIdx) && unk31C == 2) {
            unk31C = 0;
        }

        if (dComIfGp_evmng_endCheck(mAppearEventIdx)) {
            dComIfGp_event_reset();
            dComIfGs_onEventBit(0xE01U);
        }

        if (dComIfGp_evmng_endCheck(mDisappearEventIdx)) {
            dComIfGp_event_reset();
            dComIfGs_onEventBit(0xF40U);
        }
    } else {
        if (dComIfGs_isEventBit(0xE01U) == FALSE && this->unk31C == 1) {
            fopAcM_orderOtherEventId(this, mAppearEventIdx);
            eventInfo.onCondition(dEvtCnd_UNK2_e);
        } else if (dComIfGs_isEventBit(0xF40U) == FALSE && this->unk31C == 2) {
            fopAcM_orderOtherEventId(this, mDisappearEventIdx);
            eventInfo.onCondition(dEvtCnd_UNK2_e);
        }
    }
}

/* 00000C18-00000D90       .text appear_bridge__11daLbridge_cFv */
void daLbridge_c::appear_bridge() {
    dComIfG_Bgsp()->Regist(mpBgW, this);
    set_mtx();
    mpBgW->Move();

    cXyz pos1 = current.pos;
    cXyz pos2 = current.pos;

    pos1.z += 100.0f;
    pos2.z -= 100.0f;

    dComIfGp_particle_setProjection(dPa_name::ID_SCENE_8119, &pos1, &current.angle);
    dComIfGp_particle_setProjection(dPa_name::ID_SCENE_8119, &pos2, &current.angle);

    set_on_se();

    mBpkAnm.setFrame(0.0f);
    mBpkAnm.setPlaySpeed(1.0f);

    if (mpEmitter != NULL) {
        mpEmitter->playCreateParticle();
        mpEmitter->playDrawParticle();
    }

    mTimer = 31;
    unk31C = 1;
}

/* 00000D90-00000E30       .text disappear_bridge__11daLbridge_cFv */
void daLbridge_c::disappear_bridge() {
    dComIfG_Bgsp()->Release(mpBgW);

    if (mpEmitter != NULL) {
        mpEmitter->stopDrawParticle();
    }

    set_off_se();

    mBpkAnm.setFrame(mBpkAnm.getEndFrame());
    mBpkAnm.setPlaySpeed(-1.0f);

    unk31C = 2;
}

/* 00000E30-00000EA0       .text set_on_se__11daLbridge_cFv */
void daLbridge_c::set_on_se() {
    fopAcM_seStart(this, JA_SE_OBJ_L_BRIDGE_ON, 0);
}

/* 00000EA0-00000F10       .text set_off_se__11daLbridge_cFv */
void daLbridge_c::set_off_se() {
    fopAcM_seStart(this, JA_SE_OBJ_L_BRIDGE_OFF, 0);
}

/* 00000F10-00000FF8       .text _draw__11daLbridge_cFv */
bool daLbridge_c::_draw() {
    g_env_light.settingTevStruct(TEV_TYPE_BG0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mpModel, &tevStr);

    mBtkAnm.entry(mpModel->getModelData());
    mBpkAnm.entry(mpModel->getModelData());
    mBrkAnm.entry(mpModel->getModelData());

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();

    return TRUE;
}

bool daLbridge_c::_delete() {
    if (mpEmitter != NULL) {
        mpEmitter->becomeInvalidEmitter();
        mpEmitter = NULL;
    }

    bool isSw = fopAcM_isSwitch(this, mSwitchNo);

    if ((mSwitchNo == 0xFF || isSw == true) && heap != NULL) {
        dComIfG_Bgsp()->Release(mpBgW);
    }

    dComIfG_resDelete(&mPhs, m_arcname);

    return TRUE;
}

/* 00000FF8-00001018       .text daLbridge_Create__FPv */
static cPhs_State daLbridge_Create(void* i_this) {
    return static_cast<daLbridge_c*>(i_this)->_create();
}

/* 00001018-000010D4       .text daLbridge_Delete__FPv */
static BOOL daLbridge_Delete(void* i_this) {
    return static_cast<daLbridge_c*>(i_this)->_delete();
}

/* 000010D4-000010F8       .text daLbridge_Draw__FPv */
static BOOL daLbridge_Draw(void* i_this) {
    return static_cast<daLbridge_c*>(i_this)->_draw();
}

/* 000010F8-0000111C       .text daLbridge_Execute__FPv */
static BOOL daLbridge_Execute(void* i_this) {
    return static_cast<daLbridge_c*>(i_this)->_execute();
}

/* 0000111C-00001124       .text daLbridge_IsDelete__FPv */
static BOOL daLbridge_IsDelete(void*) {
    return TRUE;
}

static actor_method_class daLbridgeMethodTable = {
    (process_method_func)daLbridge_Create,
    (process_method_func)daLbridge_Delete,
    (process_method_func)daLbridge_Execute,
    (process_method_func)daLbridge_IsDelete,
    (process_method_func)daLbridge_Draw,
};

actor_process_profile_definition g_profile_LIGHTBRIDGE = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0007,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_LIGHTBRIDGE,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daLbridge_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_LIGHTBRIDGE,
    /* Actor SubMtd */ &daLbridgeMethodTable,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
