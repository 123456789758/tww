//
// Generated by dtk
// Translation Unit: d_a_boss_item.cpp
//

#include "d/actor/d_a_boss_item.h"
#include "f_op/f_op_actor.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "d/d_com_inf_game.h"

/* 00000078-00000080       .text daBossItem_IsDelete__FP14bossitem_class */
static BOOL daBossItem_IsDelete(bossitem_class* i_this) {
    return TRUE;
}

/* 00000080-00000088       .text daBossItem_Delete__FP14bossitem_class */
static BOOL daBossItem_Delete(bossitem_class* i_this) {
    return TRUE;
}

/* 00000088-00000128       .text daBossItem_Create__FP10fopAc_ac_c */
static cPhs_State daBossItem_Create(fopAc_ac_c* i_this) {
    bossitem_class* a_this = (bossitem_class*)i_this;
    fopAcM_SetupActor(i_this, bossitem_class);

    int stageNo = daBossItem_prm::getStage(a_this);
    BOOL isStageBossDead = dComIfGs_isStageBossEnemy(stageNo);

    if (isStageBossDead && !dComIfGs_isStageLife(stageNo)) {
        fopAcM_createItemForBoss(
            &i_this->current.pos, 1, fopAcM_GetRoomNo(i_this),
            &i_this->current.angle, NULL, 1
        );
    }

    return cPhs_ERROR_e;
}

static actor_method_class daBossItem_METHODS = {
    (process_method_func)daBossItem_Create,
    (process_method_func)daBossItem_Delete,
    (process_method_func)NULL,
    (process_method_func)daBossItem_IsDelete,
    (process_method_func)NULL,
};

actor_process_profile_definition g_profile_BOSSITEM = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0007,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_BOSSITEM,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(bossitem_class),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_BOSSITEM,
    /* Actor SubMtd */ &daBossItem_METHODS,
    /* Status       */ fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_0_e,
};
