/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YAUDIT_HGUARD
#define YAUDIT_HGUARD loaded



#include    <stdio.h>        /* C_ANSI : strcpy, strlen, strchr, strcmp, ...  */
#include    <ySTR_solo.h>



/*---(standard recturn codes)-------------------*/
#define     RC_ACK          0
#define     RC_POSITIVE     1
#define     RC_OVERRIDE     2
#define     RC_REPAIR       3
#define     RC_WARNING      4
#define     RC_FAILED       5
#define     RC_FATAL        5   /* depricated */
/*---(c_type)-------------------------*/
#define     YENV_BLOCK       'b'
#define     YENV_CHAR        'c'
#define     YENV_DIR         'd'
#define     YENV_HARD        'h'
#define     YENV_IPSOC       'i'
#define     YENV_PIPE        'p'
#define     YENV_REG         'r'
#define     YENV_SYM         's'
#define     YENV_NONE        '-'
#define     YENV_WTF         '?'
#define     YENV_ANY         '!'
/*---(defense)------------------------*/
#define     YENV_REAL        "bcdhiprs"
#define     YENV_TYPES       "bcdhiprs-"
/*---(c_style)------------------------*/
#define     YENV_BOTH        'b'
/*---(c_naming)-----------------------*/
#define     YENV_WILD        '-'
#define     YENV_NORMAL      'n'
#define     YENV_STANDARD    's'
#define     YENV_LOCAL       'l'
#define     YENV_CENTRAL     'c'
/*---(defense)------------------------*/
#define     YENV_NAMING      "-nslc"
/*---(c_flag)-------------------------*/
#define     YENV_FIX         'f'
#define     YENV_FORCE       'F'
#define     YENV_HAMMER      '!'
/*---(done)---------------------------*/

#define     YENV_QUALITY     'y'
#define     YENV_TOOSHORT    '<'
#define     YENV_TOOLONG     '>'
#define     YENV_BADCHAR     '#'

#define     YENV_LOGIN       'y'
#define     YENV_NOLOGIN     '·'

#define     YENV_ACTIVE      'y'
#define     YENV_LOCKED      '!'
#define     YENV_UNSET       '*'
#define     YENV_NOPASS      '?'



/*===[[ yENV_base.c ]]========================================================*/
/*········· ´······················ ´·········································*/
char*       yAUDIT_version          (void);
/*---(done)-----------------*/



/*===[[ yENV_audit.c ]]=======================================================*/
/*········· ´······················ ´·········································*/
/*---(driver)---------------*/
char        yAUDIT_full             (char a_type, char c_flag, char c_naming, char a_dir [LEN_PATH], char a_file [LEN_PATH], char a_owner [LEN_USER], char a_group [LEN_USER], char a_perms [LEN_TERSE], int a_major, int a_minor, char a_ttype, char a_target [LEN_PATH], char a_prefix [LEN_TERSE], char a_suffix [LEN_TERSE], int a_epoch, long a_bytes, int a_inode, char a_hash [LEN_DESC], char r_full [LEN_PATH], int *r_fuid, char r_fuser [LEN_USER]);
/*---(simplifiers)----------*/
char        yAUDIT_base             (char a_type, char c_flag, char c_naming, char a_dir [LEN_PATH], char a_file [LEN_PATH], char a_owner [LEN_USER], char a_group [LEN_USER], char a_perms [LEN_TERSE], int a_major, int a_minor, char a_ttype, char a_target [LEN_PATH]);
char        yAUDIT_del              (char c_flag, char a_dir [LEN_PATH], char a_file [LEN_PATH]);
char        yAUDIT_reg              (char c_flag, char c_naming, char a_dir [LEN_PATH], char a_file [LEN_PATH], char a_owner [LEN_USER], char a_group [LEN_USER], char a_perms [LEN_TERSE]);
char        yAUDIT_local            (char a_dir [LEN_PATH], char a_file [LEN_PATH], char a_prefix [LEN_TERSE], char a_suffix [LEN_TERSE], char r_full [LEN_PATH], int *r_fuid, char r_fuser [LEN_USER]);
char        yAUDIT_localdir         (char a_dir [LEN_PATH], char r_full [LEN_PATH], int *r_fuid, char r_fuser [LEN_USER]);
char        yAUDIT_central          (char c_flag, char a_dir [LEN_PATH], char a_file [LEN_PATH], char a_prefix [LEN_TERSE], char a_suffix [LEN_TERSE], char r_full [LEN_PATH], int *r_fuid, char r_fuser [LEN_USER]);
char        yAUDIT_centraldir       (char c_flag, char a_dir [LEN_PATH], char a_perms [LEN_TERSE]);
/*---(done)-----------------*/





#endif
/*===[[ END ]]================================================================*/
