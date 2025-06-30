/*===[[ START ]]==============================================================*/
#include    "yAUDIT.h"
#include    "yAUDIT_priv.h"



char
yaudit_typing           (char a_etype, char a_etdesc [LEN_TERSE], char a_atype, char a_atdesc [LEN_TERSE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_msg       [LEN_HUND]  = "";
   /*---(defense)----------+-----+-----+-*/
   --rce;  if (a_etype  == 0)     return rce;
   --rce;  if (a_etdesc == NULL)  return rce;
   --rce;  if (a_atype  == 0)     return rce;
   --rce;  if (a_atdesc == NULL)  return rce;
   /*---(prepare)------------------------*/
   ySCORE_mark ("RTYPE"   , '°');
   /*---(non-existance)------------------*/
   if (a_atype == YENV_NONE) {
      if (a_etype == YENV_NONE) {
         ySCORE_mark ("RTYPE"   , a_atype);
         yURG_msg ('-', "non-existance of filesystem entry confirmed");
         return RC_POSITIVE;
      } else {
         ySCORE_mark ("RTYPE"   , '?');
         sprintf (x_msg, "%s (%c) entry does not exist, AND not in FORCE mode"     , a_etdesc, a_etype);
         yenv_audit_fatal ("RECHECK" , x_msg);
         return RC_FAILED;
      }
   }
   /*---(match)--------------------------*/
   if (a_atype == a_etype) {
      ySCORE_mark ("RTYPE"   , a_atype);
      switch (a_atype) {
      case YENV_SYM  :
         yURG_msg ('-', "%s (%c) entry existance confirmed, AND not a normal/hardlink"  , a_etdesc, a_etype);
         return RC_ACK;
         break;
      case YENV_HARD :
         yURG_msg ('-', "%s (%c) entry existance confirmed, AND not a normal/symlink"   , a_etdesc, a_etype);
         return RC_ACK;
         break;
      default        :
         yURG_msg ('-', "%s (%c) entry existance confirmed, AND not a symlink/hardlink", a_etdesc, a_etype);
         return RC_ACK;
         break;
      }
   }
   /*---(not-matched)--------------------*/
   sprintf (x_msg, "wanted %s (%c) entry ACTUALLY refers to a %s (%c)", a_etdesc, a_etype, a_atdesc, a_atype);
   yenv_audit_fatal ("RECHECK" , x_msg);
   return RC_FAILED;
}

char
yaudit_final            (char a_full [LEN_PATH], char a_etype, char a_etdesc [LEN_TERSE], char a_eowner [LEN_USER], char a_egroup [LEN_USER], char a_eperms [LEN_TERSE], int a_euid, int a_egid, int a_eprm, char a_adisp [LEN_LABEL], int a_emajor, int a_eminor, char a_ettype, char a_etarget [LEN_PATH])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        rc_final    = RC_POSITIVE;
   char        x_atype     = YENV_NONE;
   char        x_atdesc    [LEN_TERSE] = "";
   char        x_aowner    [LEN_USER]  = "";
   char        x_agroup    [LEN_USER]  = "";
   char        x_aperms    [LEN_TERSE] = "";
   int         x_auid      =    0;
   int         x_agid      =    0;
   uint        x_aprm      =    0;
   char        x_adisp     [LEN_LABEL] = "";
   int         x_amajor    =   -1;
   int         x_aminor    =   -1;
   char        x_atarget     [LEN_PATH]  = "";
   int         x_inode     =   -1;
   int         n           =   -1;
   char        x_msg       [LEN_HUND]  = "";
   /*---(header)-------------------------*/
   DEBUG_YENV   yLOG_enter   (__FUNCTION__);
   /*---(re-gather data)-----------------*/
   DEBUG_YENV   yLOG_note    ("re-gathering data");
   x_atype = yENV_detail (a_full, x_atdesc, &x_auid, x_aowner, &x_agid, x_agroup, &x_aprm, x_aperms, x_adisp, NULL, NULL, &x_amajor, &x_aminor, x_atarget, NULL, &x_inode, NULL);
   DEBUG_YENV   yLOG_value   ("detail"    , x_atype);
   --rce;  if (x_atype <= 0) {
      yenv_audit_fatal ("RTYPE"   , '°');
      sprintf (x_msg, "file check generated a hard error (%d)", x_atype);
      yenv_audit_fatal ("RECHECK" , x_msg);
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YENV   yLOG_char    ("detail"    , x_atype);
   /*---(existance check)----------------*/
   DEBUG_YENV   yLOG_note    ("conducting final-check");
   rc = yaudit_typing (a_etype, a_etdesc, x_atype, x_atdesc);
   DEBUG_YENV   yLOG_value   ("typing"    , rc);
   if (rc == RC_POSITIVE) {
      ySCORE_mark ("RECHECK" , '´');
      ySCORE_mark ("FINAL"   , 'Ï');
      DEBUG_YENV   yLOG_exit    (__FUNCTION__);
      return RC_POSITIVE;
   }
   if (rc == RC_FAILED) {
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(major/minor)--------------------*/
   DEBUG_YENV   yLOG_complex ("device"    , "act %d:%d, exp %d:%d", x_amajor, x_aminor, a_emajor, a_eminor);
   --rce;  if (strchr ("bc", x_atype) != NULL) {
      ySCORE_mark ("RMAJOR"  , 'j');
      if (x_amajor != a_emajor) {
         ySCORE_mark ("RMAJOR"  , '°');
         yURG_err ('f', "device id (%d:%d); BUT expected (%d:%d)", x_amajor, x_aminor, a_emajor, a_eminor);
         rc_final = rce;
      }
      ySCORE_mark ("RMINOR"  , 'n');
      if (x_aminor != a_eminor) {
         ySCORE_mark ("RMINOR"  , '°');
         yURG_err ('f', "device id (%d:%d); BUT expected (%d:%d)", x_amajor, x_aminor, a_emajor, a_eminor);
         rc_final = rce;
      }
   }
   /*---(symlink)------------------------*/
   DEBUG_YENV   yLOG_info    ("target act", x_atarget);
   DEBUG_YENV   yLOG_info    ("target exp", a_etarget);
   --rce;  if (x_atype == YENV_SYM) { 
      ySCORE_mark ("RTTYPE"  , '°');
      ySCORE_mark ("RTARGET" , '°');
      if (x_atarget != NULL && a_etarget != NULL) {
         /*---(type/exist)---------------*/
         rc = yENV_detail (x_atarget, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
         if (rc == YENV_NONE) {
            yURG_err ('f', "symlink target does not exist");
            rc_final = rce;
         } else if (rc != a_ettype) {
            yURG_err ('f', "symlink target actual type (%c) does not match expected (%c)", rc, a_ettype);
            rc_final = rce;
         } else {
            ySCORE_mark ("RTTYPE"  , rc);
         }
         /*---(target)-------------------*/
         if (strcmp (x_atarget, a_etarget) == 0) {
            ySCORE_mark ("RTARGET" , 't');
         } else {
            yURG_err ('f', "symlink %2då%-30.30s]; BUT expected %2då%-30.30sæ", strlen (x_atarget), x_atarget, strlen (a_etarget), a_etarget);
            rc_final = rce;
         }
         /*---(done)---------------------*/
      }
   }
   /*---(hardlink)-----------------------*/
   --rce;  if (x_atype == YENV_HARD) { 
      ySCORE_mark ("RTTYPE"  , '°');
      ySCORE_mark ("RTARGET" , '°');
      if (x_atarget != NULL && a_etarget != NULL) {
         rc = yENV_detail (x_atarget, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
         /*---(type/exist)---------------*/
         rc = yENV_detail (x_atarget, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
         if (rc == YENV_NONE) {
            yURG_err ('f', "hardlink target does not exist");
            rc_final = rce;
         } else if (rc != a_ettype) {
            yURG_err ('f', "hardlink target actual type (%c) does not match expected (%c)", rc, a_ettype);
            rc_final = rce;
         } else {
            if (a_ettype == YENV_REG && rc == YENV_HARD)  ySCORE_mark ("RTTYPE"  , YENV_REG);
            else                                          ySCORE_mark ("RTTYPE"  , rc);
         }
         /*---(target)-------------------*/
         rc = yENV_detail (a_etarget, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &n, NULL);
         if (x_inode == n) {
            ySCORE_mark ("RTARGET" , 't');
         } else {
            yURG_err ('f', "hardlink %2då%-30.30s] idode is (%d); BUT actually points at inode (%d)", strlen (a_etarget), a_etarget, n, x_inode);
            rc_final = rce;
         }
      }
   }
   /*---(owner)--------------------------*/
   DEBUG_YENV   yLOG_complex ("owner"     , "act %s, exp %s", x_aowner, a_eowner);
   ySCORE_mark ("ROWNER"  , '°');
   --rce;  if (strcmp (a_eowner, "") == 0) {
      ySCORE_mark ("ROWNER"  , '-');
   } else {
      if (strcmp (x_aowner, a_eowner) != 0) {
         yURG_err ('f', "owned by å%sæ (%d); BUT expected å%sæ (%d)", x_aowner, x_auid, a_eowner, a_euid);
         rc_final = rce;
      } else {
         ySCORE_mark ("ROWNER"  , 'o');
      }
   }
   /*---(group)--------------------------*/
   DEBUG_YENV   yLOG_complex ("group"     , "act %s, ext %s", x_agroup, a_egroup);
   ySCORE_mark ("RGROUP"  , '°');
   --rce;  if (strcmp (a_egroup, "") == 0) {
      ySCORE_mark ("RGROUP"  , '-');
   } else {
      if (strcmp (x_agroup, a_egroup) != 0) {
         yURG_err ('f', "grouped in å%sæ (%d); BUT expected å%sæ (%d)", x_agroup, x_agid, a_egroup, a_egid);
         rc_final = rce;
      } else {
         ySCORE_mark ("RGROUP"  , 'g');
      }
   }
   /*---(owner/group summary)------------*/
   if (ySCORE_value ("ROWNER"  ) == 'o' && ySCORE_value ("RGROUP"  ) == 'g') {
      yURG_msg ('-', "ownership confirmed, owned by å%sæ (%d) and in group å%sæ (%d)", a_eowner, a_euid, a_egroup, a_egid);
   } else if (ySCORE_value ("ROWNER"  ) == '-' && ySCORE_value ("RGROUP"  ) == '-') {
      yURG_msg ('-', "ownership check å-æ SKIPPED, actual owner å%sæ (%d) and group å%sæ (%d)", x_aowner, x_auid, x_agroup, x_agid);
   }
   /*---(perms)--------------------------*/
   DEBUG_YENV   yLOG_complex ("perms"     , "act %s, ext %s", x_aperms, a_eperms);
   ySCORE_mark ("RPERMS"  , '°');
   --rce;  if (strcmp (a_eperms, "") == 0) {
      ySCORE_mark ("RPERMS"  , '-');
      yURG_msg ('-', "permissions check å-æ SKIPPED, actuals å%sæ, %04o, disp å%sæ", x_aperms, x_aprm, x_adisp);
   } else {
      if (strcmp (x_aperms, a_eperms) != 0) {
         yURG_err ('f', "permissions å%sæ, %04o, disp å%sæ; BUT expected å%sæ, %04o, disp å%sæ", x_aperms, x_aprm, x_adisp, a_eperms, a_eprm, a_adisp);
         rc_final = rce;
      } else {
         ySCORE_mark ("RPERMS"  , 'p');
         yURG_msg ('-', "permissions confirmed, å%sæ, %04o, disp å%sæ", a_eperms, a_eprm, a_adisp);
      }
   }
   /*---(finalize)-----------------------*/
   if (rc_final < 0) {
      yenv_audit_fatal ("RECHECK"  , "");
   } else {
      ySCORE_mark ("RECHECK" , '´');
      ySCORE_mark ("FINAL"   , 'Ï');
   }
   /*---(complete)-----------------------*/
   DEBUG_YENV    yLOG_exit    (__FUNCTION__);
   return rc_final;
}

char
yaudit_hacked           (char a_full [LEN_PATH], int a_epoch, long a_bytes, int a_inode, char a_hash [LEN_DESC])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        rc_final    = RC_POSITIVE;
   char        x_atype     = YENV_NONE;
   int         x_epoch     =   -1;
   int         x_bytes     =   -1;
   int         x_inode     =   -1;
   char        x_hash      [LEN_DESC]  = "";
   char        c           =    0;
   /*---(header)-------------------------*/
   DEBUG_YENV   yLOG_enter   (__FUNCTION__);
   /*---(re-gather data)-----------------*/
   DEBUG_YENV   yLOG_note    ("re-gathering data");
   x_atype = yENV_detail (a_full, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &x_epoch, &x_bytes, NULL, NULL, NULL, NULL, &x_inode, x_hash);
   DEBUG_YENV   yLOG_value   ("detail"    , x_atype);
   --rce;  if (x_atype <= 0) {
      ySCORE_mark ("HACKED"  , '°');
      yURG_err ('f', "file check generated a hard error (%d)", x_atype);
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YENV   yLOG_char    ("detail"    , x_atype);
   /*---(epoch)--------------------------*/
   DEBUG_YENV   yLOG_complex ("epoch"     , "act %d, vs exp %d", x_epoch, a_epoch);
   --rce;  if (a_epoch > 0) {
      ++c;
      ySCORE_mark ("AEPOCH"  , 'e');
      if (strchr ("rh", x_atype) != NULL) {
         if (x_epoch != a_epoch) {
            ySCORE_mark ("AEPOCH"  , '°');
            yURG_err ('f', "epoch (%d); BUT expected (%d)", x_epoch, a_epoch);
            rc_final = rce;
         }
      } else {
         ySCORE_mark ("AEPOCH"  , '-');
      }
   }
   /*---(bytes)--------------------------*/
   DEBUG_YENV   yLOG_complex ("bytes"     , "act %d, vs exp %d", x_bytes, a_bytes);
   --rce;  if (a_bytes > 0) {
      ++c;
      ySCORE_mark ("ABYTES"  , 'b');
      if (strchr ("rh", x_atype) != NULL) {
         if (x_bytes != a_bytes) {
            ySCORE_mark ("ABYTES"  , '°');
            yURG_err ('f', "bytes (%d); BUT expected (%d)", x_bytes, a_bytes);
            rc_final = rce;
         }
      } else {
         ySCORE_mark ("ABYTES"  , '-');
      }
   }
   /*---(inode)--------------------------*/
   DEBUG_YENV   yLOG_complex ("inode"     , "act %d, vs exp %d", x_inode, a_inode);
   --rce;  if (a_inode > 0) {
      ++c;
      ySCORE_mark ("AINODE"  , 'i');
      if (x_inode != a_inode) {
         ySCORE_mark ("AINODE"  , '°');
         yURG_err ('f', "inode (%d); BUT expected (%d)", x_inode, a_inode);
         rc_final = rce;
      }
   }
   /*---(hash)---------------------------*/
   --rce;  if (a_hash != NULL && strcmp (a_hash, "") != 0) {
      ++c;
      ySCORE_mark ("AHASH"   , 'h');
      if (strchr ("rh", x_atype) != NULL) {
         if (strcmp (x_hash, a_hash) != 0) {
            ySCORE_mark ("AHASH"   , '°');
            yURG_err ('f', "hash å%sæ; BUT expected å%sæ", x_hash, a_hash);
            rc_final = rce;
         }
      } else {
         ySCORE_mark ("AHASH"   , '-');
      }
   }
   /*---(finalize)-----------------------*/
   if (rc_final < 0) {
      ySCORE_mark ("HACKED"  , '°');
   } else if (c > 0) {
      ySCORE_mark ("HACKED"  , '´');
   }
   /*---(complete)-----------------------*/
   DEBUG_YENV    yLOG_exit    (__FUNCTION__);
   return rc_final;
}



