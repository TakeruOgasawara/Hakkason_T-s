//==================================================================================================
//
//pointMessage.h
//Author:���R����
//
//==================================================================================================

#ifndef _PLOG_H_
#define _PLOG_H_

#include "main.h"
#include "point.h"

//=====================================
// �v���g�^�C�v�錾
//=====================================
void InitPointLog(void);
void UninitPointLog(void);
void UpdatePointLog(void);
void DrawPointLog(void);
void SetPointLog(Point *pPoint, LOGTYPE type,float fScale);

PointLog *GetPointLog(void);

#endif
