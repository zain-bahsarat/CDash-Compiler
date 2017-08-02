/* 
 * File:   DashUtil.h
 * Author: zain basharat
 *
 * Created on April 6, 2013, 7:51 PM
 */

#ifndef DASHUTIL_H
#define	DASHUTIL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "DashGlobals.h"

//it is used for debugging purposes .It prints the information about the token.  
    void print_tokens(Token* );
    
    void print_tree(node* r);


#ifdef	__cplusplus
}
#endif

#endif	/* DASHUTIL_H */

