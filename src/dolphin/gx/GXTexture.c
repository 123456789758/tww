#include "dolphin/gx/GXTexture.h"
#include "dolphin/gx/GX.h"
#include "string.h"

#define GET_TILE_COUNT(a, b) (((a) + (1 << (b)) - 1) >> (b))

inline void __GXGetTexTileShift(GXTexFmt format, u32* widthTiles, u32* heightTiles) {
    switch (format) {
    case GX_TF_I4:
    case GX_TF_C4:
    case GX_TF_CMPR:
    case GX_CTF_R4:
    case GX_CTF_Z4:
        *widthTiles = 3;
        *heightTiles = 3;
        break;

    case GX_TF_I8:
    case GX_TF_IA4:
    case GX_TF_C8:
    case GX_TF_Z8:
    case GX_CTF_RA4:
    case GX_CTF_R8:
    case GX_CTF_G8:
    case GX_CTF_B8:
    case GX_CTF_RG8:
    case GX_CTF_Z8M:
    case GX_CTF_Z8L:
        *widthTiles = 3;
        *heightTiles = 2;
        break;

    case GX_TF_IA8:
    case GX_TF_RGB565:
    case GX_TF_RGB5A3:
    case GX_TF_RGBA8:
    case GX_TF_C14X2:
    case GX_TF_Z16:
    case GX_TF_Z24X8:
    case GX_CTF_RA8:
    case GX_CTF_GB8:
    case 44:
    case GX_CTF_Z16L:
        *widthTiles = 2;
        *heightTiles = 2;
        break;

    default:
        *heightTiles = 0;
        *widthTiles = 0;
        break;
    }
}

u32 GXGetTexBufferSize(u16 width, u16 height, u32 format, GXBool mipmap, u8 max_lod) {
    u32 widthTiles, heightTiles, tileSize, bufferSize, numX, numY, i;

    __GXGetTexTileShift(format, &widthTiles, &heightTiles);

    if (format == GX_TF_RGBA8 || format == GX_TF_Z24X8) {
        tileSize = 0x40;
    } else {
        tileSize = 0x20;
    }

    if (mipmap == GX_TRUE) {
        bufferSize = 0;

        for (i = 0; i < max_lod; i++) {
            numX = GET_TILE_COUNT(width, widthTiles);
            numY = GET_TILE_COUNT(height, heightTiles);

            bufferSize += numX * numY * tileSize;
            if (width == 1 && height == 1) {
                break;
            }

            width = (width > 1) ? (width >> 1) : 1;
            height = (height > 1) ? (height >> 1) : 1;
        }

    } else {
        numX = GET_TILE_COUNT(width, widthTiles);
        numY = GET_TILE_COUNT(height, heightTiles);
        bufferSize = numX * numY * tileSize;
    }

    return bufferSize;
}

void __GetImageTileCount(GXTexFmt format, u16 width, u16 height, u32* a, u32* b, u32* c) {
    u32 widthTiles, heightTiles;

    __GXGetTexTileShift(format, &widthTiles, &heightTiles);

    if (width <= 0) {
        width = 1;
    }

    if (height <= 0) {
        height = 1;
    }

    *a = GET_TILE_COUNT(width, widthTiles);
    *b = GET_TILE_COUNT(height, heightTiles);
    *c = (format == GX_TF_RGBA8 || format == GX_TF_Z24X8) ? 2 : 1;
}

void GXInitTexObj(GXTexObj* obj, void* imagePtr, u16 width, u16 height, GXTexFmt format,
                  GXTexWrapMode sWrap, GXTexWrapMode tWrap, GXBool useMIPmap) {
    u32 imageBase;
    u16 a, b;
    u32 c, d;

    GXTexObj* internal = (GXTexObj*)obj;
    memset(internal, 0, sizeof(*internal));

    GX_SET_REG(internal->texture_filter, sWrap, 30, 31);
    GX_SET_REG(internal->texture_filter, tWrap, 28, 29);
    GX_SET_REG(internal->texture_filter, GX_TRUE, 27, 27);

    if (useMIPmap) {
        u32 maxDimSize;
        internal->texture_flags |= 1;
        if (format == 8 || format == 9 || format == 10) {
            GX_SET_REG(internal->texture_filter, 5, 24, 26);
        } else {
            GX_SET_REG(internal->texture_filter, 6, 24, 26);
        }

        maxDimSize = width > height ? 31 - __cntlzw(width) : 31 - __cntlzw(height);

        GX_SET_REG(internal->texture_lod, (u8)((maxDimSize) * 16.f), 16, 23);
    } else {
        GX_SET_REG(internal->texture_filter, 4, 24, 26);
    }

    internal->texture_format = format;
    GX_SET_REG(internal->texture_size, width - 1, 22, 31);
    GX_SET_REG(internal->texture_size, height - 1, 12, 21);

    GX_SET_REG(internal->texture_size, format & 0xf, 8, 11);
    imageBase = (u32)imagePtr >> 5;
    GX_SET_REG(internal->texture_address, imageBase, 11, 31);

    switch (format & 0xf) {
    case 0:
    case 8:
        internal->texture_tile_type = 1;
        a = 3;
        b = 3;
        break;
    case 1:
    case 2:
    case 9:
        internal->texture_tile_type = 2;
        a = 3;
        b = 2;
        break;
    case 3:
    case 4:
    case 5:
    case 10:
        internal->texture_tile_type = 2;
        a = 2;
        b = 2;
        break;
    case 6:
        internal->texture_tile_type = 3;
        a = 2;
        b = 2;
        break;
    case 0xe:
        internal->texture_tile_type = 0;
        a = 3;
        b = 3;
        break;
    default:
        internal->texture_tile_type = 2;
        a = 2;
        b = 2;
        break;
    }

    internal->texture_time_count = (GET_TILE_COUNT(width, a) * GET_TILE_COUNT(height, b)) & 0x7fff;

    internal->texture_flags |= 2;
}

void GXInitTexObjCI(GXTexObj* obj, void* imagePtr, u16 width, u16 height, GXCITexFmt format,
                    GXTexWrapMode sWrap, GXTexWrapMode tWrap, GXBool useMIPmap, u32 tlut_name) {
    GXTexObj* internal = (GXTexObj*)obj;

    GXInitTexObj(obj, imagePtr, width, height, format, sWrap, tWrap, useMIPmap);

    internal->texture_flags &= ~2;
    internal->tlut_name = tlut_name;
}

u8 GXTexMode0Ids[8] = {0x80, 0x81, 0x82, 0x83, 0xA0, 0xA1, 0xA2, 0xA3};

u8 GXTexMode1Ids[8] = {0x84, 0x85, 0x86, 0x87, 0xA4, 0xA5, 0xA6, 0xA7};

u8 GXTexImage0Ids[8] = {0x88, 0x89, 0x8a, 0x8b, 0xA8, 0xA9, 0xAa, 0xAb};

u8 GXTexImage1Ids[8] = {0x8c, 0x8d, 0x8e, 0x8f, 0xAc, 0xAd, 0xAe, 0xAf};

u8 GXTexImage2Ids[8] = {0x90, 0x91, 0x92, 0x93, 0xB0, 0xB1, 0xB2, 0xB3};

u8 GXTexImage3Ids[8] = {0x94, 0x95, 0x96, 0x97, 0xB4, 0xB5, 0xB6, 0xB7};

u8 GXTexTlutIds[8] = {0x98, 0x99, 0x9a, 0x9b, 0xB8, 0xB9, 0xBa, 0xBb};

u8 GX2HWFiltConv[6] = {0x00, 0x04, 0x01, 0x05, 0x02, 0x06};

void GXInitTexObjLOD(GXTexObj* obj, GXTexFilter minFilter, GXTexFilter maxFilter, f32 minLOD,
                     f32 maxLOD, f32 lodBias, GXBool doBiasClamp, GXBool doEdgeLOD,
                     GXAnisotropy maxAniso) {
    GXTexObj* internal = (GXTexObj*)obj;
    u8 reg1, reg2;
    if (lodBias < -4.0f) {
        lodBias = -4.0f;
    } else if (lodBias >= 4.0f) {
        lodBias = 3.99f;
    }

    GX_SET_REG(internal->texture_filter, (u8)(lodBias * 32.0f), 15, 22);
    GX_SET_REG(internal->texture_filter, maxFilter == 1 ? 1 : 0, 27, 27);
    GX_SET_REG(internal->texture_filter, GX2HWFiltConv[minFilter], 24, 26);
    GX_SET_REG(internal->texture_filter, doEdgeLOD ? 0 : 1, 23, 23);
    GX_SET_REG(internal->texture_filter, 0, 14, 14);
    GX_SET_REG(internal->texture_filter, 0, 13, 13);
    GX_SET_REG(internal->texture_filter, maxAniso, 11, 12);
    GX_SET_REG(internal->texture_filter, doBiasClamp, 10, 10);

    if (minLOD < 0.0f) {
        minLOD = 0.0f;
    } else if (minLOD > 10.0f) {
        minLOD = 10.0f;
    }
    reg1 = minLOD * 16.0f;

    if (maxLOD < 0.0f) {
        maxLOD = 0.0f;
    } else if (maxLOD > 10.0f) {
        maxLOD = 10.0f;
    }
    reg2 = maxLOD * 16.0f;

    GX_SET_REG(internal->texture_lod, reg1, 24, 31);
    GX_SET_REG(internal->texture_lod, reg2, 16, 23);
}

u16 GXGetTexObjWidth(GXTexObj* obj) {
    return (obj->texture_size & 0x3ff) + 1;
}

u16 GXGetTexObjHeight(GXTexObj* obj) {
    return ((obj->texture_size >> 10) & 0x3ff) + 1;
}

GXTexFmt GXGetTexObjFmt(const GXTexObj* obj) {
    return obj->texture_format;
}

GXTexWrapMode GXGetTexObjWrapS(GXTexObj* obj) {
    return obj->texture_filter & 0x3;
}

GXTexWrapMode GXGetTexObjWrapT(GXTexObj* obj) {
    return (obj->texture_filter & 0xc) >> 2;
}

u32 GXGetTexObjTlut(GXTexObj* obj) {
    return obj->tlut_name;
}

void GXLoadTexObjPreLoaded(GXTexObj* obj, GXTexRegion* region, GXTexMapID map) {
    u8 stackManipulation[0x18];

    GXTexObj* internalObj = (GXTexObj*)obj;
    GXTexRegion* internalRegion = (GXTexRegion*)region;

    GX_SET_REG(internalObj->texture_filter, GXTexMode0Ids[map], 0, 7);
    GX_SET_REG(internalObj->texture_lod, GXTexMode1Ids[map], 0, 7);
    GX_SET_REG(internalObj->texture_size, GXTexImage0Ids[map], 0, 7);

    GX_SET_REG(internalRegion->unk0, GXTexImage1Ids[map], 0, 7);
    GX_SET_REG(internalRegion->unk4, GXTexImage2Ids[map], 0, 7);

    GX_SET_REG(internalObj->texture_address, GXTexImage3Ids[map], 0, 7);

    GX_BP_LOAD_REG(internalObj->texture_filter);
    GX_BP_LOAD_REG(internalObj->texture_lod);
    GX_BP_LOAD_REG(internalObj->texture_size);
    GX_BP_LOAD_REG(internalRegion->unk0);
    GX_BP_LOAD_REG(internalRegion->unk4);
    GX_BP_LOAD_REG(internalObj->texture_address);

    if ((internalObj->texture_flags & 2) == 0) {
        GXTlutObj* tlut = (GXTlutObj*)gx->tlutRegionCallback(internalObj->tlut_name);
        GX_SET_REG(tlut->address, GXTexTlutIds[map], 0, 7);

        GX_BP_LOAD_REG(tlut->address);
    }

    gx->tImage0[map] = internalObj->texture_size;
    gx->tMode0[map] = internalObj->texture_filter;

    gx->dirtyState |= GX_DIRTY_SU_TEX;
    gx->bpSentNot = GX_FALSE;
}

void GXLoadTexObj(GXTexObj* obj, GXTexMapID map) {
    GXTexRegion* ret = (GXTexRegion*)gx->texRegionCallback(obj, map);

    GXLoadTexObjPreLoaded(obj, ret, map);
}

void GXInitTlutObj(GXTlutObj* obj, void* table, GXTlutFmt format, u16 numEntries) {
    GXTlutObj* internal = (GXTlutObj*)obj;

    internal->format = 0;

    GX_SET_REG(internal->format, format, 20, 21);
    GX_SET_REG(internal->address, (u32)table >> 5, 11, 31);
    GX_SET_REG(internal->address, 100, 0, 7);

    internal->numEntries = numEntries;
}

void GXLoadTlut(GXTlutObj* obj, u32 tlut_name) {
    GXTlutObj* internal = (GXTlutObj*)obj;
    GXTlutRegion* ret = (GXTlutRegion*)gx->tlutRegionCallback(tlut_name);
    u32 reg;

    __GXFlushTextureState();

    GX_BP_LOAD_REG(internal->address);
    GX_BP_LOAD_REG(ret->unk0);

    __GXFlushTextureState();

    reg = ret->unk0;
    GX_SET_REG(internal->format, reg, 22, 31);

    ret->tlutObj = *internal;
}

void GXInitTexCacheRegion(GXTexRegion* region, GXBool is32bMIPmap, u32 memEven,
                          GXTexCacheSize sizeEven, u32 memOdd, GXTexCacheSize sizeOdd) {
    GXTexRegion* internal = (GXTexRegion*)region;

    u32 reg;
    switch (sizeEven) {
    case 0:
        reg = 3;
        break;
    case 1:
        reg = 4;
        break;
    case 2:
        reg = 5;
        break;
    }

    internal->unk0 = 0;

    GX_SET_REG(internal->unk0, memEven >> 5, 17, 31);
    GX_SET_REG(internal->unk0, reg, 14, 16);
    GX_SET_REG(internal->unk0, reg, 11, 13);
    GX_SET_REG(internal->unk0, 0, 10, 10);

    switch (sizeOdd) {
    case 0:
        reg = 3;
        break;
    case 1:
        reg = 4;
        break;
    case 2:
        reg = 5;
        break;
    case 3:
        reg = 0;
        break;
    }

    internal->unk4 = 0;
    GX_SET_REG(internal->unk4, memOdd >> 5, 17, 31);
    GX_SET_REG(internal->unk4, reg, 14, 16);
    GX_SET_REG(internal->unk4, reg, 11, 13);

    internal->unkC = is32bMIPmap;
    internal->unkD = 1;
}

void GXInitTlutRegion(GXTlutRegion* region, u32 memAddr, GXTlutSize tlutSize) {
    GXTlutRegion* internal = (GXTlutRegion*)region;

    internal->unk0 = 0;
    GX_SET_REG(internal->unk0, (memAddr - 0x80000) >> 9, 22, 31);
    GX_SET_REG(internal->unk0, tlutSize, 11, 21);
    GX_SET_REG(internal->unk0, 0x65, 0, 7);
}

void GXInvalidateTexAll(void) {
    __GXFlushTextureState();
    GXFIFO.u8 = 0x61;
    GXFIFO.u32 = 0x66001000;
    GXFIFO.u8 = 0x61;
    GXFIFO.u32 = 0x66001100;
    __GXFlushTextureState();
}

GXTexRegionCallback GXSetTexRegionCallback(GXTexRegionCallback callback) {
    GXTexRegionCallback prev = gx->texRegionCallback;
    gx->texRegionCallback = callback;
    return prev;
}

GXTlutRegionCallback GXSetTlutRegionCallback(GXTlutRegionCallback callback) {
    GXTlutRegionCallback prev = gx->tlutRegionCallback;
    gx->tlutRegionCallback = callback;
    return prev;
}

void GXSetTexCoordScaleManually(GXTexCoordID coord, GXBool enable, u16 s_scale, u16 t_scale) {
    /* Nonmatching */
}

void __SetSURegs(u32 texImgIndex, u32 setUpRegIndex) {
    u16 a1, a2;
    GXBool b, c;

    a1 = GX_GET_REG(gx->tImage0[texImgIndex], 22, 31);
    a2 = (gx->tImage0[texImgIndex] & (0x3ff << 10)) >> 10;

    GX_SET_REG(gx->suTs0[setUpRegIndex], a1, 16, 31);
    GX_SET_REG(gx->suTs1[setUpRegIndex], a2, 16, 31);

    b = GX_GET_REG(gx->tMode0[texImgIndex], 30, 31) == 1;
    c = GX_GET_REG(gx->tMode0[texImgIndex], 28, 29) == 1;

    GX_SET_REG(gx->suTs0[setUpRegIndex], b, 15, 15);
    GX_SET_REG(gx->suTs1[setUpRegIndex], c, 15, 15);

    GX_BP_LOAD_REG(gx->suTs0[setUpRegIndex]);
    GX_BP_LOAD_REG(gx->suTs1[setUpRegIndex]);

    gx->bpSentNot = GX_FALSE;
}

#pragma dont_inline on
void __GXSetSUTexRegs(void) {
    u32 i;
    u32 b;
    u32 a;
    u32 c;
    u32 d;
    u32 stackFiller;
    if (gx->tcsManEnab != 0xff) {
        a = GX_GET_REG(gx->genMode, 18, 21) + 1;
        b = GX_GET_REG(gx->genMode, 13, 15);
        for (i = 0; i < b; i++) {
            switch (i) {
            case 0:
                c = GX_GET_REG(gx->iref, 29, 31);
                d = GX_GET_REG(gx->iref, 26, 28);
                break;
            case 1:
                c = GX_GET_REG(gx->iref, 23, 25);
                d = GX_GET_REG(gx->iref, 20, 22);
                break;
            case 2:
                c = GX_GET_REG(gx->iref, 17, 19);
                d = GX_GET_REG(gx->iref, 14, 16);
                break;
            case 3:
                c = GX_GET_REG(gx->iref, 11, 13);
                d = GX_GET_REG(gx->iref, 8, 10);
                break;
            }

            if (!(gx->tcsManEnab & (1 << d))) {
                __SetSURegs(c, d);
            }
        }

        for (i = 0; i < a; i++) {
            u32* g = &gx->tref[i / 2];

            c = gx->texmapId[i] & ~0x100;

            if (i & 1) {
                d = GX_GET_REG(*g, 14, 16);
            } else {
                d = GX_GET_REG(*g, 26, 28);
            }

            if (c != 0xff && !(gx->tcsManEnab & (1 << d)) && gx->tevTcEnab & (1 << i)) {
                __SetSURegs(c, d);
            }
        }
    }
}
#pragma dont_inline reset

void __GXSetTmemConfig(u32 config) {
    switch (config) {
    case 1:
        GX_BP_LOAD_REG(0x8c0d8000);
        GX_BP_LOAD_REG(0x900dc000);

        GX_BP_LOAD_REG(0x8d0d8800);
        GX_BP_LOAD_REG(0x910dc800);

        GX_BP_LOAD_REG(0x8e0d9000);
        GX_BP_LOAD_REG(0x920dd000);

        GX_BP_LOAD_REG(0x8f0d9800);
        GX_BP_LOAD_REG(0x930dd800);

        GX_BP_LOAD_REG(0xac0da000);
        GX_BP_LOAD_REG(0xb00de000);

        GX_BP_LOAD_REG(0xad0da800);
        GX_BP_LOAD_REG(0xb10de800);

        GX_BP_LOAD_REG(0xae0db000);
        GX_BP_LOAD_REG(0xb20df000);

        GX_BP_LOAD_REG(0xaf0db800);
        GX_BP_LOAD_REG(0xb30df800);

        break;
    case 0:
    default:
        GX_BP_LOAD_REG(0x8c0d8000);
        GX_BP_LOAD_REG(0x900dc000);

        GX_BP_LOAD_REG(0x8d0d8400);
        GX_BP_LOAD_REG(0x910dc400);

        GX_BP_LOAD_REG(0x8e0d8800);
        GX_BP_LOAD_REG(0x920dc800);

        GX_BP_LOAD_REG(0x8f0d8c00);
        GX_BP_LOAD_REG(0x930dcc00);

        GX_BP_LOAD_REG(0xac0d9000);
        GX_BP_LOAD_REG(0xb00dd000);

        GX_BP_LOAD_REG(0xad0d9400);
        GX_BP_LOAD_REG(0xb10dd400);

        GX_BP_LOAD_REG(0xae0d9800);
        GX_BP_LOAD_REG(0xb20dd800);

        GX_BP_LOAD_REG(0xaf0d9c00);
        GX_BP_LOAD_REG(0xb30ddc00);

        break;
    }
}
