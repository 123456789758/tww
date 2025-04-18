//
// Generated by dtk
// Translation Unit: f_pc_load.cpp
//

#include "f_pc/f_pc_load.h"
#include "c/c_dylink.h"
#include "SSystem/SComponent/c_phase.h"

/* 8003E230-8003E280       .text fpcLd_Use__Fs */
BOOL fpcLd_Use(s16 i_procName) {
    if (fpcLd_IsLoaded(i_procName) == TRUE && fpcLd_Load(i_procName) == cPhs_COMPLEATE_e)
        return TRUE;
    return FALSE;
}

/* 8003E280-8003E2A4       .text fpcLd_IsLoaded__Fs */
BOOL fpcLd_IsLoaded(s16 i_procName) {
    return cDyl_IsLinked((int)i_procName);
}

/* 8003E2A4-8003E2C8       .text fpcLd_Free__Fs */
void fpcLd_Free(s16 i_procName) {
    cDyl_Unlink((int)i_procName);
}

/* 8003E2C8-8003E318       .text fpcLd_Load__Fs */
cPhs_State fpcLd_Load(s16 i_procName) {
    cPhs_State phase = cDyl_LinkASync((int)i_procName);

    switch (phase) {
    case cPhs_COMPLEATE_e:
        return cPhs_COMPLEATE_e;
    case cPhs_INIT_e:
        return cPhs_INIT_e;
    default:
        return cPhs_ERROR_e;
    }
}
