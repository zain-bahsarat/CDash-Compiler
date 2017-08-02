/* 
 * File:   DashCGen.h
 * Author: zain
 *
 * Created on May 12, 2013, 11:49 AM
 */

#ifndef DASHCGEN_H
#define	DASHCGEN_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "DashGlobals.h"
#include "DashUtil.h"
    
    void initData();
    void initCode();
    void endCode();
    void generate(StatKind sk,node* statement);


#ifdef	__cplusplus
}
#endif

#endif	/* DASHCGEN_H */

