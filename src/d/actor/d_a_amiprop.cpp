//
// Generated by dtk
// Translation Unit: d_a_amiprop.cpp
//

#include "d/actor/d_a_amiprop.h"
#include "d/d_procname.h"
#include "d/d_priority.h"

/* 00000078-000000D4       .text _delete__11daAmiProp_cFv */
bool daAmiProp_c::_delete() {
    /* Nonmatching */
}

/* 000000D4-000000F4       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c*) {
    /* Nonmatching */
}

/* 000000F4-00000244       .text CreateHeap__11daAmiProp_cFv */
void daAmiProp_c::CreateHeap() {
    /* Nonmatching */
}

/* 00000244-00000318       .text CreateInit__11daAmiProp_cFv */
void daAmiProp_c::CreateInit() {
    /* Nonmatching */
}

/* 00000318-00000444       .text _create__11daAmiProp_cFv */
cPhs_State daAmiProp_c::_create() {
    /* Nonmatching */
}

/* 000005FC-00000694       .text set_mtx__11daAmiProp_cFv */
void daAmiProp_c::set_mtx() {
    /* Nonmatching */
}

/* 00000694-0000070C       .text setMoveBGMtx__11daAmiProp_cFv */
void daAmiProp_c::setMoveBGMtx() {
    /* Nonmatching */
}

/* 0000070C-00000884       .text _execute__11daAmiProp_cFv */
bool daAmiProp_c::_execute() {
    /* Nonmatching */
}

/* 00000884-00000924       .text _draw__11daAmiProp_cFv */
bool daAmiProp_c::_draw() {
    /* Nonmatching */
}

/* 00000924-00000944       .text daAmiProp_Create__FPv */
static cPhs_State daAmiProp_Create(void*) {
    /* Nonmatching */
}

/* 00000944-00000968       .text daAmiProp_Delete__FPv */
static BOOL daAmiProp_Delete(void*) {
    /* Nonmatching */
}

/* 00000968-0000098C       .text daAmiProp_Draw__FPv */
static BOOL daAmiProp_Draw(void*) {
    /* Nonmatching */
}

/* 0000098C-000009B0       .text daAmiProp_Execute__FPv */
static BOOL daAmiProp_Execute(void*) {
    /* Nonmatching */
}

/* 000009B0-000009B8       .text daAmiProp_IsDelete__FPv */
static BOOL daAmiProp_IsDelete(void*) {
    /* Nonmatching */
}

static actor_method_class daAmiPropMethodTable = {
    (process_method_func)daAmiProp_Create,
    (process_method_func)daAmiProp_Delete,
    (process_method_func)daAmiProp_Execute,
    (process_method_func)daAmiProp_IsDelete,
    (process_method_func)daAmiProp_Draw,
};

actor_process_profile_definition g_profile_AMI_PROP = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0003,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_AMI_PROP,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daAmiProp_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_AMI_PROP,
    /* Actor SubMtd */ &daAmiPropMethodTable,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
