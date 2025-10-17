/*===[[ START ]]==============================================================*/
#include    "yAUDIT.h"
#include    "yAUDIT_priv.h"



char
yAUDIT_full             (char a_type, char c_flag, char c_naming, char a_dir [LEN_PATH], char a_file [LEN_PATH], char a_owner [LEN_USER], char a_group [LEN_USER], char a_perms [LEN_TERSE], int a_major, int a_minor, char a_ttype, char a_target [LEN_PATH], char a_prefix [LEN_TERSE], char a_suffix [LEN_TERSE], int a_epoch, long a_bytes, int a_inode, char a_hash [LEN_DESC], char r_full [LEN_PATH], int *r_fuid, char r_fuser [LEN_USER])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        rc_final    = RC_POSITIVE;
   char        x_full      [LEN_PATH]  = "";
   char        x_atype     =  '-';
   char        x_atdesc    [LEN_TERSE] = "";
   char        x_etdesc    [LEN_TERSE] = "";
   char        x_eowner    [LEN_USER]  = "";
   char        x_egroup    [LEN_USER]  = "";
   char        x_eperms    [LEN_TERSE] = "";
   int         x_euid      =   -1;
   int         x_egid      =   -1;
   int         x_eprm      =   -1;
   char        x_edisp      [LEN_LABEL] = "";
   char        c_check     =  '-';
   char        c_force     =  '-';
   char        c_fix       =  '-';
   char        x_del       =  '-';
   char        x_add       =  '-';
   char        x_upd       =  '-';
   char        x_style     =  '-';
   /*---(header)-------------------------*/
   DEBUG_YENV   yLOG_enter   (__FUNCTION__);
   /*---(defaults)-----------------------*/
   if (a_owner != NULL)  strlcpy (x_eowner, a_owner, LEN_USER);
   if (a_group != NULL)  strlcpy (x_egroup, a_group, LEN_USER);
   if (a_perms != NULL)  strlcpy (x_eperms, a_perms, LEN_TERSE);
   if (r_full  != NULL)  strcpy  (r_full  , "");
   if (r_fuid  != NULL)  *r_fuid  = -1;
   if (r_fuser != NULL)  strcpy  (r_fuser , "");
   /*---(prepare)------------------------*/
   rc = yaudit_prepare     (a_type, c_flag, c_naming, a_dir, a_file, x_etdesc, &c_check, &c_force, &c_fix);
   if (rc > rc_final)  rc_final = rc;
   DEBUG_YENV   yLOG_complex ("prepare"   , "%4d rc, %4d final", rc, rc_final);
   --rce;  if (rc < 0) {
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(name quality)-------------------*/
   rc = yaudit_quality       (a_type, c_naming, a_dir, a_file, &x_style, x_full);
   if (rc > rc_final)  rc_final = rc;
   DEBUG_YENV   yLOG_complex ("quality"   , "%4d rc, %4d final", rc, rc_final);
   --rce;  if (rc < 0) {
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = yaudit_standard      (a_type, c_naming, x_style, a_dir, a_file, a_prefix, a_suffix);
   if (rc > rc_final)  rc_final = rc;
   DEBUG_YENV   yLOG_complex ("standard"  , "%4d rc, %4d final", rc, rc_final);
   --rce;  if (rc < 0) {
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(location)-----------------------*/
   rc = yaudit_name_location (a_type, c_naming, x_full, x_eowner, x_egroup, x_eperms);
   if (rc > rc_final)  rc_final = rc;
   DEBUG_YENV   yLOG_complex ("location"  , "%4d rc, %4d final", rc, rc_final);
   --rce;  if (rc < 0) {
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(name sign-off)------------------*/
   ySCORE_mark (myAUDIT.m_yscore, "NAME"    , '´');
   /*---(expected)-----------------------*/
   rc = yaudit_expect  (a_type, x_eowner, x_egroup, x_eperms, &x_euid, &x_egid, &x_eprm, x_edisp);
   if (rc > rc_final)  rc_final = rc;
   DEBUG_YENV   yLOG_complex ("expect"    , "%4d rc, %4d final", rc, rc_final);
   --rce;  if (rc < 0) {
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(extra)--------------------------*/
   rc = yaudit_extra   (a_type, a_major, a_minor, a_ttype, a_target, a_epoch, a_bytes, a_inode, a_hash);
   if (rc > rc_final)  rc_final = rc;
   DEBUG_YENV   yLOG_complex ("extra"     , "%4d rc, %4d final", rc, rc_final);
   --rce;  if (rc < 0) {
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(expect sign-off)----------------*/
   ySCORE_mark (myAUDIT.m_yscore, "EXPECT"  , '´');
   /*---(precheck)-----------------------*/
   rc = yaudit_precheck (x_full, a_type, x_eowner, x_egroup, x_eperms, a_major, a_minor, a_ttype, a_target, &x_atype, x_atdesc, &x_del, &x_add, &x_upd, c_force, c_fix);
   if (rc > rc_final)  rc_final = rc;
   DEBUG_YENV   yLOG_complex ("check"     , "%4d rc, %4d final", rc, rc_final);
   --rce;  if (rc < 0) {
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(remove)-------------------------*/
   rc = yaudit_remove   (x_full, x_atype, x_atdesc, c_force, x_del);
   if (rc > rc_final)  rc_final = rc;
   DEBUG_YENV   yLOG_complex ("remove"    , "%4d rc, %4d final", rc, rc_final);
   --rce;  if (rc < 0) {
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create)-------------------------*/
   rc = yaudit_create   (x_full, a_type, x_etdesc, x_eowner, x_egroup, x_eperms, a_major, a_minor, a_target, c_force, x_add);
   if (rc > rc_final)  rc_final = rc;
   DEBUG_YENV   yLOG_complex ("create"    , "%4d rc, %4d final", rc, rc_final);
   --rce;  if (rc < 0) {
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update)-------------------------*/
   rc = yaudit_update   (x_full, a_type, x_etdesc, x_eowner, x_egroup, x_eperms, a_major, a_minor, a_target, c_fix, x_upd);
   if (rc > rc_final)  rc_final = rc;
   DEBUG_YENV   yLOG_complex ("update"    , "%4d rc, %4d final", rc, rc_final);
   --rce;  if (rc < 0) {
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(expect sign-off)----------------*/
   ySCORE_mark (myAUDIT.m_yscore, "FIXES"   , '´');
   /*---(final check)--------------------*/
   rc = yaudit_final    (x_full, a_type, x_etdesc, x_eowner, x_egroup, x_eperms, x_euid, x_egid, x_eprm, x_edisp, a_major, a_minor, a_ttype, a_target);
   if (rc > rc_final)  rc_final = rc;
   DEBUG_YENV   yLOG_complex ("final"     , "%4d rc, %4d final", rc, rc_final);
   --rce;  if (rc < 0) {
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(hacker check)-------------------*/
   rc = yaudit_hacked       (x_full, a_epoch, a_bytes, a_inode, a_hash);
   if (rc > rc_final)  rc_final = rc;
   DEBUG_YENV   yLOG_complex ("hacked"    , "%4d rc, %4d final", rc, rc_final);
   --rce;  if (rc < 0) {
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)----------------------*/
   if (r_full  != NULL)  strlcpy (r_full , x_full  , LEN_PATH);
   if (r_fuid  != NULL)  *r_fuid  = x_euid;
   if (r_fuser != NULL)  strlcpy (r_fuser, x_eowner, LEN_USER);
   /*---(complete)-----------------------*/
   DEBUG_YENV    yLOG_exit    (__FUNCTION__);
   return rc_final;
}

char
yAUDIT_base             (char a_type, char c_flag, char c_naming, char a_dir [LEN_PATH], char a_file [LEN_PATH], char a_owner [LEN_USER], char a_group [LEN_USER], char a_perms [LEN_TERSE], int a_major, int a_minor, char a_ttype, char a_target [LEN_PATH])
{
   char        rc          =    0;
   DEBUG_YENV    yLOG_enter   (__FUNCTION__);
   rc = yAUDIT_full     (a_type, c_flag, c_naming, a_dir, a_file, a_owner, a_group, a_perms, a_major, a_minor, a_ttype, a_target, "", "", -1, -1, -1, "", NULL, NULL, NULL);
   DEBUG_YENV    yLOG_exit    (__FUNCTION__);
   return rc;
}

char
yAUDIT_del              (char c_flag, char a_dir [LEN_PATH], char a_file [LEN_PATH])
{
   char        rc          =    0;
   DEBUG_YENV    yLOG_enter   (__FUNCTION__);
   rc = yAUDIT_full     (YENV_NONE , c_flag, 'n', a_dir, a_file, "", "", "", -1, -1, YENV_NONE, "", "", "", -1, -1, -1, NULL, NULL, NULL, NULL);
   DEBUG_YENV    yLOG_exit    (__FUNCTION__);
   return rc;
}

char
yAUDIT_reg              (char c_flag, char c_naming, char a_dir [LEN_PATH], char a_file [LEN_PATH], char a_owner [LEN_USER], char a_group [LEN_USER], char a_perms [LEN_TERSE])
{
   char        rc          =    0;
   DEBUG_YENV    yLOG_enter   (__FUNCTION__);
   rc = yAUDIT_full     (YENV_REG  , c_flag, c_naming, a_dir, a_file, a_owner, a_group, a_perms, -1, -1, YENV_NONE, "", "", "", -1, -1, -1, NULL, NULL, NULL, NULL);
   DEBUG_YENV    yLOG_exit    (__FUNCTION__);
   return rc;
}

char
yAUDIT_local            (char a_dir [LEN_PATH], char a_file [LEN_PATH], char a_prefix [LEN_TERSE], char a_suffix [LEN_TERSE], char r_full [LEN_PATH], int *r_fuid, char r_fuser [LEN_USER])
{
   char        rc          =    0;
   DEBUG_YENV    yLOG_enter   (__FUNCTION__);
   rc = yAUDIT_full     (YENV_REG  , '-', YENV_LOCAL  , a_dir, a_file, "-", "-", "-", -1, -1, YENV_NONE, "", a_prefix, a_suffix, -1, -1, -1, NULL, r_full, r_fuid, r_fuser);
   DEBUG_YENV    yLOG_exit    (__FUNCTION__);
   return rc;
}

char
yAUDIT_localdir         (char a_dir [LEN_PATH], char r_full [LEN_PATH], int *r_fuid, char r_fuser [LEN_USER])
{
   char        rc          =    0;
   DEBUG_YENV    yLOG_enter   (__FUNCTION__);
   rc = yAUDIT_full     (YENV_DIR  , '-', YENV_STANDARD, a_dir, "", "-", "-", "-", -1, -1, YENV_NONE, "", "", "", -1, -1, -1, NULL, r_full, r_fuid, r_fuser);
   DEBUG_YENV    yLOG_exit    (__FUNCTION__);
   return rc;
}

char
yAUDIT_central          (char c_flag, char a_dir [LEN_PATH], char a_file [LEN_PATH], char a_prefix [LEN_TERSE], char a_suffix [LEN_TERSE], char r_full [LEN_PATH], int *r_fuid, char r_fuser [LEN_USER])
{
   char        rc          =    0;
   DEBUG_YENV    yLOG_enter   (__FUNCTION__);
   rc = yAUDIT_full     (YENV_REG  , c_flag, YENV_CENTRAL, a_dir, a_file, "-", "-", "-", -1, -1, YENV_NONE, "", a_prefix, a_suffix, -1, -1, -1, NULL, r_full, r_fuid, r_fuser);
   DEBUG_YENV    yLOG_exit    (__FUNCTION__);
   return rc;
}

char
yAUDIT_centraldir       (char c_flag, char a_dir [LEN_PATH], char a_perms [LEN_TERSE])
{
   char        rc          =    0;
   DEBUG_YENV    yLOG_enter   (__FUNCTION__);
   rc = yAUDIT_full     (YENV_DIR  , c_flag, YENV_CENTRAL, a_dir, "", "root", "root", a_perms, -1, -1, YENV_NONE, "", "", "", -1, -1, -1, NULL, NULL, NULL, NULL);
   DEBUG_YENV    yLOG_exit    (__FUNCTION__);
   return rc;
}



