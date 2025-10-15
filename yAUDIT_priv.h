/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YENV_priv_hguard
#define YENV_priv_hguard loaded



/*===[[ ONE_LINERS ]]=========================================================*/
/*                      ´·········1·········2·········3·········4·········5·········6·········7*/
/*--------- 12345678901 ´123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/
/*········· ··········· ´·····························´········································*/
#define     P_FOCUS     "RS (run-time support)"
#define     P_NICHE     "de (debugging/logging control)"
#define     P_SUBJECT   "linux filesystem entry auditing"
#define     P_PURPOSE   "provide low-level linux filesystem and security auditing"
/*········· ··········· ´·····························´········································*/
#define     P_NAMESAKE  "tartarus-eeroeis (murky one)"
#define     P_PRONOUNCE "tar·tar·uhs"
#define     P_HERITAGE  "incorporeal protogenoi god of the deep abyss, then dungeon of torment"
#define     P_BRIEFLY   "master of the deep abyss"
#define     P_IMAGERY   "dreaded place of darkness and punishment reserved for the worst sinners"
#define     P_REASON    "tool for dealing with some of the dark secrets of linux filesystems"
/*········· ··········· ´·····························´········································*/
#define     P_ONELINE   P_NAMESAKE " " P_SUBJECT
/*········· ··········· ´·····························´········································*/
#define     P_BASENAME  "yAUDIT"
#define     P_HOMEDIR   "/home/system/yAUDIT.filesystem_entry_auditing"
#define     P_FULLPATH  "/usr/local/lib64/libyAUDIT.so"
#define     P_SUFFIX    "···"
#define     P_CONTENT   "···"
/*········· ··········· ´·····························´········································*/
#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_COMPILER  "gcc 5.3.0"
#define     P_CODESIZE  "small       (appoximately 1,000 slocl)"
/*········· ··········· ´·····························´········································*/
#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   "2025-06"
/*········· ··········· ´·····························´········································*/
#define     P_VERMAJOR  "1.--, production improvements"
#define     P_VERMINOR  "1.0-, porting from yENV into separate library"
#define     P_VERNUM    "1.0e"
#define     P_VERTXT    "updated yAUDIT with ySCORE improvements, still must unit test"
/*········· ··········· ´·····························´········································*/
#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"
#define     P_REMINDER  "there are many better options, but i *own* every byte of this one"
/*········· ··········· ´·····························´········································*/
/*--------- 12345678901 ´123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/



/*===[[ END_HEADER ]]=========================================================*/

/*345678901-12345678901-123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/

#include    <stdio.h>        /* C_ANSI : strcpy, strlen, strchr, strcmp, ...  */
#include    <string.h>       /* C_ANSI : printf, snprintf, fgets, fopen, ...  */
#include    <stdlib.h>       /* C_ANSI : exit                                 */
#include    <stdarg.h>       /* C_ANSI : va_args                              */
#include    <ctype.h>        /* C_ANSI : tolower, toupper, ...                */
#include    <sys/stat.h>          /* fstat, umask                                */
#include    <unistd.h>       /* stat(),crpyt()                      */
#include    <sys/mount.h>    /* C_ANSI : mount                                */
#include    <pwd.h>               /* getpwuid, getpwnam                       */
#include    <grp.h>               /* getgruid, getgrnam                       */
#include    <fcntl.h>             /* creat, open, fcntl                       */
#include    <sys/sysmacros.h>
#include    <openssl/sha.h>       /* openssl implementation of sha1           */
#include    <time.h>              /* time, localtime, strftime, ...              */
#include    <errno.h>             /* allow use of errno value                 */



#include    <yLOG.h>
#include    <yURG.h>
#include    <yENV.h>
#include    <ySCORE.h>
#include    <yDLST_solo.h>
#include    <yCOLOR_solo.h>  /* CUSTOM : heatherly color constants            */


#define     DEBUG_YENV   if (1) 

typedef struct stat      tSTAT;
typedef struct passwd    tPASSWD;
typedef struct group     tGROUP;
typedef struct tm        tTIME;


typedef struct cyAUDIT tyAUDIT;
struct cyAUDIT {
   /*---(general)-----------*/
   char      m_print       [LEN_RECD];
   /*---(ySCORE)------------*/
   void     *m_yscore;
   /*---(done)--------------*/
};
extern  tyAUDIT  myAUDIT;



/*===[[ REQUEST ]]===============*/

#define     MARK_ETYPE     0

#define     MARK_EFLAG     5

#define     MARK_EOWNER    7
#define     MARK_EGROUP    8
#define     MARK_EPERMS    9

#define     MARK_EMAJOR   11
#define     MARK_EMINOR   12

#define     MARK_ETTYPE   14
#define     MARK_ETARGET  15

#define     MARK_EEPOCH   17
#define     MARK_EBYTES   18
#define     MARK_EINODE   19
#define     MARK_EHASH    20

#define     MARK_REQUEST  22

#define     MARK_CONFC    26
#define     MARK_CONFF    27
#define     MARK_CONFX    28

/*===[[ CHECK ]]=================*/

#define     MARK_CTYPE    32

#define     MARK_COWNER   34
#define     MARK_CGROUP   35
#define     MARK_CPERMS   36

#define     MARK_CMAJOR   38
#define     MARK_CMINOR   39

#define     MARK_CTTYPE   41
#define     MARK_CTARGET  42

#define     MARK_CHECK    44

#define     MARK_FDEL     48
#define     MARK_FADD     49
#define     MARK_FUPD     50

/*===[[ UPDATES ]]===============*/

#define     MARK_REMOVE   54
#define     MARK_CREATE   55
#define     MARK_UPDATE   56

/*===[[ RECHECK ]]===============*/

#define     MARK_RTYPE    62

#define     MARK_ROWNER   64
#define     MARK_RGROUP   65
#define     MARK_RPERMS   66

#define     MARK_RMAJOR   68
#define     MARK_RMINOR   69

#define     MARK_RTTYPE   71
#define     MARK_RTARGET  72

#define     MARK_RECHECK  74

/*===[[ FINAL ]]=================*/

#define     MARK_FINAL    78

/*===[[ HACKING ]]===============*/

#define     MARK_AEPOCH   82
#define     MARK_ABYTES   83
#define     MARK_AINODE   84
#define     MARK_AHASH    85

#define     MARK_HACKED   87

/*===[[ DONE ]]==================*/



#define    IF_DEV         if (strchr ("bc", x_type) != NULL) 


extern const tSCORE_TABLE myAUDIT_table [LEN_FULL];


/*===[[ yENV_audit_beg.c ]]===================================================*/
/*········· ´······················ ´·········································*/
char        yaudit_fatal            (char a_label [LEN_LABEL], char a_msg [LEN_HUND]);
char        yaudit_prepare          (char a_type, char c_flag, char c_naming, char a_dir [LEN_PATH], char a_file [LEN_PATH], char r_tdesc [LEN_TERSE], char *r_check, char *r_force, char *r_fix);
char        yaudit_expect           (char a_type, char b_owner [LEN_USER], char b_group [LEN_USER], char b_perms [LEN_TERSE], int *r_uid, int *r_gid, int *r_prm, char r_disp [LEN_LABEL]);
char        yaudit_extra            (char a_type, int a_major, int a_minor, char a_ttype, char a_target [LEN_PATH], int a_epoch, long a_bytes, int a_inode, char a_hash [LEN_DESC]);
/*---(done)-----------------*/



/*===[[ yENV_audit_chg.c ]]===================================================*/
/*········· ´······················ ´·········································*/
char        yaudit_precheck         (char a_full [LEN_PATH], char a_etype, char a_eowner [LEN_USER], char a_egroup [LEN_USER], char a_eperms [LEN_TERSE], int a_emajor, int a_eminor, char a_ettype, char a_etarget [LEN_PATH], char *r_atype, char r_atdesc [LEN_TERSE], char *r_del, char *r_add, char *r_upd, char c_force, char c_fix);
char        yaudit_remove           (char a_full [LEN_PATH], char a_atype, char a_atdesc [LEN_TERSE], char c_force, char c_del);
char        yaudit_create           (char a_full [LEN_PATH], char a_etype, char a_etdesc [LEN_TERSE], char a_eowner [LEN_USER], char a_egroup [LEN_USER], char a_eperms [LEN_TERSE], int a_emajor, int a_eminor, char a_etarget [LEN_PATH], char c_force, char c_add);
char        yaudit_update           (char a_full [LEN_PATH], char a_atype, char a_atdesc [LEN_TERSE], char a_eowner [LEN_USER], char a_egroup [LEN_USER], char a_eperms [LEN_TERSE], int a_emajor, int a_eminor, char a_etarget [LEN_PATH], char c_fix, char c_upd);
/*---(done)-----------------*/



/*===[[ yENV_audit_end.c ]]===================================================*/
/*········· ´······················ ´·········································*/
char        yaudit_typing           (char a_etype, char a_etdesc [LEN_TERSE], char a_atype, char a_atdesc [LEN_TERSE]);
char        yaudit_final            (char a_full [LEN_PATH], char a_etype, char a_etdesc [LEN_TERSE], char a_eowner [LEN_USER], char a_egroup [LEN_USER], char a_eperms [LEN_TERSE], int a_euid, int a_egid, int a_eprm, char a_adisp [LEN_LABEL], int a_emajor, int a_eminor, char a_ettype, char a_etarget [LEN_PATH]);
char        yaudit_hacked           (char a_full [LEN_PATH], int a_epoch, long a_bytes, int a_inode, char a_hash [LEN_DESC]);
/*---(done)-----------------*/



/*===[[ yENV_name.c ]]========================================================*/
/*········· ´······················ ´·········································*/
char        yaudit_quality          (char a_type, char c_naming, char a_dir [LEN_PATH], char a_file [LEN_PATH], char *r_style, char r_full [LEN_PATH]);
char        yaudit_dots             (char c_naming, char a_file [LEN_PATH], char a_prefix [LEN_TERSE], char a_suffix [LEN_TERSE], short *r_beg, short *r_end);
char        yaudit__prefix          (char a_file [LEN_PATH], short a_beg, char a_prefix [LEN_TERSE], char r_prefix [LEN_TERSE]);
char        yaudit__suffix          (char a_file [LEN_PATH], short a_end, char a_suffix [LEN_TERSE]);
char        yaudit_standard         (char a_type, char c_naming, char c_style, char a_dir [LEN_PATH], char a_file [LEN_PATH], char a_prefix [LEN_TERSE], char a_suffix [LEN_TERSE]);
char        yaudit__local           (int a_ruid, char a_ruser [LEN_USER], char a_full [LEN_PATH], char b_owner [LEN_USER], char b_group [LEN_USER], char b_perms [LEN_TERSE]);
char        yaudit__central         (int a_ruid, char a_ruser [LEN_USER], char a_full [LEN_PATH], char b_owner [LEN_USER], char b_group [LEN_USER], char b_perms [LEN_TERSE]);
char        yaudit_name_location    (char a_type, char c_naming, char a_full [LEN_PATH], char b_owner [LEN_USER], char b_group [LEN_USER], char b_perms [LEN_TERSE]);
/*---(done)-----------------*/





#endif
