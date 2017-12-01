/*
* Copyright (c) 2010,GridNT,Inc
*All rights reserved.
*
* 文件名称：amlConst.h
* 摘要：aml常量头文件
*/

#ifndef AMLCONST_H
#define AMLCONST_H

#include <string>
#include <stdio.h>

#define ROLE_MASTER	"master"
#define ROLE_SLAVE	"slave"
#define ROLE_UNKNOW	"unknow"

#define TYPE_DATASRV	1
#define TYPE_FEP		2
#define TYPE_AMS		3
#define TYPE_ICESTORM	4
#define TYPE_CLIENT		5

#define ICE_TIMEOUT		3000
#define FAULT_TIME		12

#define AMS_ADAPTER			"amsAdapter"
#define DATASRV_ADAPTER		"datasrvAdapter"
#define FEP_ADAPTER			"fepAdapter"
#define ICESTORM_INSTANCE	"server-icestorm/TopicManager : "

#define TRANSFER_BUFFER_SIZE	BUFSIZ

static void getType(char *strType, int type)
{
	memset(strType, 0, 5);
	sprintf(strType, "%d", type);
}

#endif

