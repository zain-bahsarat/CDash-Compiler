/* 
 * File:   DashScanner.h
 * Author: zain basharat
 *
 * Created on April 6, 2013, 8:04 PM
 */

#ifndef DASHSCANNER_H
#define	DASHSCANNER_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    #include "DashGlobals.h"    
 
    //getToken returns the current token to the parser.	
	extern Token* getToken();
    
    //returns the end of file flag.  
       extern int EndOfFile();
     

#ifdef	__cplusplus
}
#endif

#endif	/* DASHSCANNER_H */

