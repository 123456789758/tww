/**
 * d_a_npc_kg1.cpp
 * NPC - Salvatore (Squid-Hunt/"Sploosh Kaboom" Minigame)
 */

#include "d/actor/d_a_npc_kg1.h"
#include "d/d_lib.h"
#include "d/d_procname.h"
#include "d/d_priority.h"

/* 000000EC-000001E0       .text __ct__15daNpc_Kg1_HIO_cFv */
daNpc_Kg1_HIO_c::daNpc_Kg1_HIO_c() {
    /* Nonmatching */
}

/* 000001E0-00000428       .text daNpc_Kg1_nodeCallBack__FP7J3DNodei */
static BOOL daNpc_Kg1_nodeCallBack(J3DNode*, int) {
    /* Nonmatching */
}

/* 00000464-00000688       .text lookBack__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::lookBack() {
    /* Nonmatching */
}

/* 00000688-000007D8       .text chkAttention__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::chkAttention() {
    /* Nonmatching */
}

/* 000007D8-000008D8       .text initTexPatternAnm__11daNpc_Kg1_cFib */
void daNpc_Kg1_c::initTexPatternAnm(int, bool) {
    /* Nonmatching */
}

/* 000008D8-00000944       .text playTexPatternAnm__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::playTexPatternAnm() {
    /* Nonmatching */
}

/* 00000944-000009E8       .text set_mtx__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::set_mtx() {
    /* Nonmatching */
}

/* 000009E8-00000A08       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c*) {
    /* Nonmatching */
}

/* 00000A08-00000D34       .text CreateHeap__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::CreateHeap() {
    /* Nonmatching */
}

/* 00000D34-00000EF0       .text CreateInit__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::CreateInit() {
    /* Nonmatching */
}

/* 00000EF0-00000FE8       .text eventOrder__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::eventOrder() {
    /* Nonmatching */
}

/* 00000FE8-000010C4       .text checkOrder__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::checkOrder() {
    /* Nonmatching */
}

/* 000010C4-00001188       .text kg1_talk_camera__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::kg1_talk_camera() {
    /* Nonmatching */
}

/* 00001188-000011D4       .text wait_action_init__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::wait_action_init() {
    /* Nonmatching */
}

/* 000011D4-00001858       .text wait_action__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::wait_action() {
    /* Nonmatching */
}

/* 00001858-00001878       .text clr_seq_flag__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::clr_seq_flag() {
    /* Nonmatching */
}

/* 00001878-000019B4       .text getMsg__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::getMsg() {
    /* Nonmatching */
}

/* 000019B4-00001C7C       .text next_msgStatus__11daNpc_Kg1_cFPUl */
void daNpc_Kg1_c::next_msgStatus(unsigned long*) {
    /* Nonmatching */
}

/* 00001C7C-00001CB8       .text anmAtr__11daNpc_Kg1_cFUs */
void daNpc_Kg1_c::anmAtr(unsigned short) {
    /* Nonmatching */
}

/* 00001CB8-00001E60       .text daNpc_Kg1_bcks_setAnm__FPCcP14mDoExt_McaMorfPScPScPScPCiPC14dLib_anm_prm_c */
void daNpc_Kg1_bcks_setAnm(const char*, mDoExt_McaMorf*, signed char*, signed char*, signed char*, const int*, const dLib_anm_prm_c*) {
    /* Nonmatching */
}

/* 00001E60-00001F8C       .text setAnm__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::setAnm() {
    /* Nonmatching */
}

/* 00001F8C-00001FAC       .text daNpc_Kg1Create__FPv */
static s32 daNpc_Kg1Create(void*) {
    /* Nonmatching */
}

/* 00001FAC-0000203C       .text _create__11daNpc_Kg1_cFv */
cPhs_State daNpc_Kg1_c::_create() {
    /* Nonmatching */
}

/* 00002480-0000250C       .text daNpc_Kg1Delete__FPv */
static BOOL daNpc_Kg1Delete(void*) {
    /* Nonmatching */
}

/* 0000250C-00002648       .text daNpc_Kg1Execute__FPv */
static BOOL daNpc_Kg1Execute(void*) {
    /* Nonmatching */
}

/* 00002648-000027CC       .text daNpc_Kg1Draw__FPv */
static BOOL daNpc_Kg1Draw(void*) {
    /* Nonmatching */
}

/* 000027CC-000027D4       .text daNpc_Kg1IsDelete__FPv */
static BOOL daNpc_Kg1IsDelete(void*) {
    /* Nonmatching */
}

static actor_method_class daNpc_Kg1MethodTable = {
    (process_method_func)daNpc_Kg1Create,
    (process_method_func)daNpc_Kg1Delete,
    (process_method_func)daNpc_Kg1Execute,
    (process_method_func)daNpc_Kg1IsDelete,
    (process_method_func)daNpc_Kg1Draw,
};

actor_process_profile_definition g_profile_NPC_KG1 = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0007,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_NPC_KG1,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Kg1_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_NPC_KG1,
    /* Actor SubMtd */ &daNpc_Kg1MethodTable,
    /* Status       */ 0x07 | fopAcStts_SHOWMAP_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_0_e,
};
