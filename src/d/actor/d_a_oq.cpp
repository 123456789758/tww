/**
 * d_a_oq.cpp
 * Enemy - Octorok
 */

#include "d/actor/d_a_oq.h"
#include "m_Do/m_Do_ext.h"
#include "d/d_procname.h"
#include "d/d_priority.h"

/* 000000EC-00000208       .text nodeCallBack__FP7J3DNodei */
static BOOL nodeCallBack(J3DNode*, int) {
    /* Nonmatching */
}

/* 00000208-00000310       .text draw_SUB__FP8oq_class */
void draw_SUB(oq_class*) {
    /* Nonmatching */
}

/* 00000310-000004DC       .text daOQ_Draw__FP8oq_class */
static BOOL daOQ_Draw(oq_class*) {
    /* Nonmatching */
}

/* 000004DC-00000608       .text anm_init__FP8oq_classifUcfi */
void anm_init(oq_class*, int, float, unsigned char, float, int) {
    /* Nonmatching */
}

/* 00000608-00000D50       .text body_atari_check__FP8oq_class */
void body_atari_check(oq_class*) {
    /* Nonmatching */
}

/* 00000D8C-00000EC4       .text BG_check__FP8oq_class */
void BG_check(oq_class*) {
    /* Nonmatching */
}

/* 00000EC4-000011D8       .text Line_check__FP8oq_class4cXyz */
void Line_check(oq_class*, cXyz) {
    /* Nonmatching */
}

/* 00001610-000016AC       .text shibuki_set__FP8oq_class4cXyzf */
void shibuki_set(oq_class*, cXyz, float) {
    /* Nonmatching */
}

/* 000016AC-00001930       .text sea_water_check__FP8oq_class */
void sea_water_check(oq_class*) {
    /* Nonmatching */
}

/* 00001930-0000198C       .text search_y_check__FP8oq_classs */
void search_y_check(oq_class*, short) {
    /* Nonmatching */
}

/* 0000198C-00001B24       .text moguru_check__FP8oq_class */
void moguru_check(oq_class*) {
    /* Nonmatching */
}

/* 00001B24-00002270       .text action_dousa__FP8oq_class */
void action_dousa(oq_class*) {
    /* Nonmatching */
}

/* 00002270-00002BEC       .text action_kougeki__FP8oq_class */
void action_kougeki(oq_class*) {
    /* Nonmatching */
}

/* 00002BEC-000032A4       .text action_tama_shoot__FP8oq_class */
void action_tama_shoot(oq_class*) {
    /* Nonmatching */
}

/* 000032A4-0000369C       .text action_itai__FP8oq_class */
void action_itai(oq_class*) {
    /* Nonmatching */
}

/* 0000369C-00003BC4       .text action_wakidasi__FP8oq_class */
void action_wakidasi(oq_class*) {
    /* Nonmatching */
}

/* 00003BC4-00004274       .text daOQ_Execute__FP8oq_class */
static BOOL daOQ_Execute(oq_class*) {
    /* Nonmatching */
}

/* 00004274-0000427C       .text daOQ_IsDelete__FP8oq_class */
static BOOL daOQ_IsDelete(oq_class*) {
    /* Nonmatching */
}

/* 0000427C-00004300       .text daOQ_Delete__FP8oq_class */
static BOOL daOQ_Delete(oq_class*) {
    /* Nonmatching */
}

/* 00004300-000046C0       .text useHeapInit__FP10fopAc_ac_c */
static BOOL useHeapInit(fopAc_ac_c*) {
    /* Nonmatching */
}

/* 00004708-00004DBC       .text daOQ_Create__FP10fopAc_ac_c */
static cPhs_State daOQ_Create(fopAc_ac_c*) {
    /* Nonmatching */
}

static actor_method_class l_daOQ_Method = {
    (process_method_func)daOQ_Create,
    (process_method_func)daOQ_Delete,
    (process_method_func)daOQ_Execute,
    (process_method_func)daOQ_IsDelete,
    (process_method_func)daOQ_Draw,
};

actor_process_profile_definition g_profile_OQ = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0007,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_OQ,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(oq_class),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_OQ,
    /* Actor SubMtd */ &l_daOQ_Method,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ENEMY_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
