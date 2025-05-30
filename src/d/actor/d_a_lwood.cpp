/**
 * d_a_lwood.cpp
 * Object - Normal tree
 */

#include "d/actor/d_a_lwood.h"
#include "d/d_a_obj.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "d/res/res_lwood.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_graphic.h"

const char daLwood_c::m_arcname[6] = "Lwood";

/* 00000078-00000098       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daLwood_c*)i_this)->CreateHeap();
}

/* 00000098-00000194       .text CreateHeap__9daLwood_cFv */
BOOL daLwood_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(m_arcname, LWOOD_BDL_ALWD);
    JUT_ASSERT(0xb9, modelData != NULL);
    mModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000022);
    if (mModel == NULL)
        return FALSE;

    mModel->setUserArea((u32)this);
    setMoveBGMtx();
    cBgD_t* bgp = (cBgD_t*)dComIfG_getObjectRes(m_arcname, LWOOD_DZB_ALWD);
    mpBgW = dBgW_NewSet(bgp, dBgW::MOVE_BG_e, &mtx);
    if (mpBgW == NULL)
        return FALSE;

    return TRUE;
}

static BOOL nodeCallBack(J3DNode*, int);

/* 00000194-000002E0       .text CreateInit__9daLwood_cFv */
void daLwood_c::CreateInit() {
    fopAcM_SetMtx(this, mModel->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, -600.0f, -0.0f, -600.0f, 600.0f, 900.0f, 600.0f);
    fopAcM_setCullSizeFar(this, 2.37f);
    mTimer = cM_rndF(32768.0f);
    mScale = cM_rndF(0.4f) + 0.8f;
    JUTNameTab* jointName = mModel->getModelData()->getJointName();
    for (u16 i = 0; i < mModel->getModelData()->getJointNum(); i++) {
        if (strcmp("J_Alwd_ha", jointName->getName(i)) == 0) {
            mModel->getModelData()->getJointNodePointer(i)->setCallBack(&nodeCallBack);
            break;
        }
    }

    mModel->calc();
    dComIfG_Bgsp()->Regist(mpBgW, this);
    set_mtx();
    mpBgW->Move();
}

/* 000002E0-000004D8       .text nodeCallBack__FP7J3DNodei */
static BOOL nodeCallBack(J3DNode* joint, int calcTiming) {
    if (mDoGph_gInf_c::isMonotone())
        return TRUE;

    if (calcTiming == J3DNodeCBCalcTiming_In) {
        u32 jntNo = ((J3DJoint*)joint)->getJntNo();
        J3DModel* model = j3dSys.getModel();
        daLwood_c* i_this = (daLwood_c*)model->getUserArea();
        if (i_this != NULL) {
            cXyz windSpeed = daObj::get_wind_spd(i_this, 100.0f);
            f32 sy = cM_ssin(i_this->getYureTimer() * 300);
            s16 r2 = windSpeed.x * sy * 10.0f;
            f32 cy = cM_scos(i_this->getYureTimer() * 300);
            s16 r0 = windSpeed.z * cy * 10.0f;

            // Fakematch: In order for the compiler to put the conversion in the right order, this
            // needs to be a double assignment for some reason. An unused temp variable is enough.
            s16 faketemp;
            s16 r1 = faketemp = fabs(sy + 1.0f) * 250.0f;

            s16 p1 = i_this->getYureScale() * r2;
            s16 p2 = i_this->getYureScale() * r0;
            s16 p0 = i_this->getYureScale() * r1;

            mDoMtx_stack_c::copy(model->getAnmMtx(jntNo));
            mDoMtx_stack_c::ZXYrotM(p0, p1, p2);
            model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
            mDoMtx_copy(mDoMtx_stack_c::get(), j3dSys.mCurrentMtx);
        }
    }

    return TRUE;
}

/* 000004D8-00000560       .text set_mtx__9daLwood_cFv */
void daLwood_c::set_mtx() {
    mModel->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(current.angle);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

/* 00000560-000005D8       .text setMoveBGMtx__9daLwood_cFv */
void daLwood_c::setMoveBGMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(current.angle);
    mDoMtx_stack_c::scaleM(scale);
    mDoMtx_copy(mDoMtx_stack_c::get(), mtx);
}

cPhs_State daLwood_c::_create() {
    fopAcM_SetupActor(this, daLwood_c);

    cPhs_State ret = dComIfG_resLoad(&mPhs, m_arcname);

    if (ret == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, CheckCreateHeap, 0x0e40) == 0) {
            ret = cPhs_ERROR_e;
        } else {
            CreateInit();
        }
    }

    return ret;
}

bool daLwood_c::_delete() {
    if (heap != NULL)
        dComIfG_Bgsp()->Release(mpBgW);

    dComIfG_resDelete(&mPhs, m_arcname);
    return TRUE;
}

bool daLwood_c::_execute() {
    mTimer++;
    return TRUE;
}

bool daLwood_c::_draw() {
    g_env_light.settingTevStruct(TEV_TYPE_BG0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mModel);
    dComIfGd_setList();
    return TRUE;
}

/* 000005D8-00000678       .text daLwood_Create__FPv */
static cPhs_State daLwood_Create(void* i_this) {
    return ((daLwood_c*)i_this)->_create();
}

/* 00000678-000006D4       .text daLwood_Delete__FPv */
static BOOL daLwood_Delete(void* i_this) {
    return ((daLwood_c*)i_this)->_delete();
}

/* 000006D4-00000778       .text daLwood_Draw__FPv */
static BOOL daLwood_Draw(void* i_this) {
    return ((daLwood_c*)i_this)->_draw();
}

/* 00000778-0000078C       .text daLwood_Execute__FPv */
static BOOL daLwood_Execute(void* i_this) {
    return ((daLwood_c*)i_this)->_execute();
}

/* 0000078C-00000794       .text daLwood_IsDelete__FPv */
static BOOL daLwood_IsDelete(void* i_this) {
    return TRUE;
}

static actor_method_class daLwoodMethodTable = {
    (process_method_func)daLwood_Create,
    (process_method_func)daLwood_Delete,
    (process_method_func)daLwood_Execute,
    (process_method_func)daLwood_IsDelete,
    (process_method_func)daLwood_Draw,
};

actor_process_profile_definition g_profile_Lwood = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0007,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_Lwood,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daLwood_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_Lwood,
    /* Actor SubMtd */ &daLwoodMethodTable,
    /* Status       */ fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
