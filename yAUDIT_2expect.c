/*===[[ START ]]==============================================================*/
#include    "yAUDIT.h"
#include    "yAUDIT_priv.h"



char
yaudit_expect           (char a_type, char b_owner [LEN_USER], char b_group [LEN_USER], char b_perms [LEN_TERSE], int *r_uid, int *r_gid, int *r_prm, char r_disp [LEN_LABEL])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        rc_final    = RC_POSITIVE;
   int         x_uid       =   -1;
   int         x_gid       =   -1;
   uint        x           =    0;
   int         x_prm       =   -1;
   char        x_owner     [LEN_USER]  = "";
   char        x_group     [LEN_USER]  = "";
   char        x_perms     [LEN_TERSE] = "";
   char        x_disp      [LEN_LABEL] = "";
   char        x_handle    [LEN_LABEL] = "";
   char        t           [LEN_TERSE] = "";
   /*---(header)-------------------------*/
   DEBUG_YENV   yLOG_enter   (__FUNCTION__);
   /*---(quick-out)----------------------*/
   DEBUG_YENV   yLOG_char    ("a_type"    , a_type);
   if (a_type == YENV_NONE) {
      DEBUG_YENV    yLOG_note    ("nothing to do");
      DEBUG_YENV    yLOG_exit    (__FUNCTION__);
      return RC_ACK;
   }
   /*---(default)------------------------*/
   if (r_uid   != NULL)  *r_uid = -1;
   if (b_owner != NULL)  { snprintf (x_owner, LEN_USER , "%s", b_owner); strcpy (b_owner, ""); }
   if (r_gid   != NULL)  *r_gid = -1;
   if (b_group != NULL)  { snprintf (x_group, LEN_USER , "%s", b_group); strcpy (b_group, ""); }
   if (r_prm   != NULL)  *r_prm = -1;
   if (b_perms != NULL)  { snprintf (x_perms, LEN_TERSE, "%s", b_perms); strcpy (b_perms, ""); }
   if (r_disp  != NULL)  strcpy (r_disp , "");
   /*---(user ownership)-----------------*/
   ySCORE_mark ("EOWNER"  , '-');
   --rce;  if (b_owner != NULL && strcmp (x_owner, "-") != 0) {
      DEBUG_YENV   yLOG_info    ("x_owner"   , x_owner);
      ySCORE_mark ("EOWNER"  , '°');
      strlcpy (t, x_owner, LEN_USER);
      rc = yENV_user_full  (a_type, t, x_owner, &x_uid, NULL, NULL, NULL, x_handle, NULL, NULL, NULL, NULL, NULL, NULL);
      if (rc >= 0) {
         x = atoi (t);
         if      (strcmp (t      , "@"    ) == 0)  ySCORE_mark ("EOWNER"  , 'O');  /* default  */
         else if (strcmp (t      , "0"    ) == 0)  ySCORE_mark ("EOWNER"  , 'ö');  /* specific */
         else if (x > 0)                           ySCORE_mark ("EOWNER"  , 'ö');  /* specific */
         else                                      ySCORE_mark ("EOWNER"  , 'o');  /* standard */
      } else {
         yURG_err ('f', "could not interpret owner å%sæ using %s", t, x_handle);
         rc_final = rce;
      }
   }
   /*---(group ownership)----------------*/
   ySCORE_mark ("EGROUP"  , '-');
   --rce;  if (b_group != NULL && strcmp (x_group, "-") != 0) {
      DEBUG_YENV   yLOG_info    ("x_group"   , x_group);
      ySCORE_mark ("EGROUP"  , '°');
      strlcpy (t, x_group, LEN_USER);
      rc = yENV_group_full (a_type, t, x_group, &x_gid, x_handle, NULL, NULL, NULL, NULL, NULL);
      if (rc >= 0) {
         x = atoi (t);
         if      (strcmp (t      , "@"    ) == 0)  ySCORE_mark ("EGROUP"  , 'G');  /* default  */
         else if (strcmp (t      , "0"    ) == 0)  ySCORE_mark ("EGROUP"  , 'ê');  /* specific */
         else if (x > 0)                           ySCORE_mark ("EGROUP"  , 'ê');  /* specific */
         else                                      ySCORE_mark ("EGROUP"  , 'g');  /* standard */
      } else {
         yURG_err ('f', "could not interpret group å%sæ using %s", t, x_handle);
         rc_final = rce;
      }
   }
   /*---(permissions)--------------------*/
   ySCORE_mark ("EPERMS"  , '-');
   --rce;  if (b_perms != NULL && strcmp (x_perms, "-") != 0) {
      ySCORE_mark ("EPERMS"  , '°');
      DEBUG_YENV   yLOG_info    ("x_perms"   , x_perms);
      strlcpy (t, x_perms, LEN_TERSE);
      rc = yENV_perms_full (a_type, t, x_perms, &x_prm, x_disp, NULL, x_handle);
      if (rc >= 0) {
         if      (strcmp (t      , "@"    ) == 0)  ySCORE_mark ("EPERMS"  , 'P');  /* default  */
         else if (strcmp (x_perms, "(n/a)") == 0)  ySCORE_mark ("EPERMS"  , '÷');  /* specific */
         else                                      ySCORE_mark ("EPERMS"  , 'p');  /* standard */
      } else {
         yURG_err ('f', "could not interpret permissions å%sæ using %s", t, x_handle);
         rc_final = rce;
      }
   }
   /*---(save-back)----------------------*/
   if (strchr ("Oöo", ySCORE_value ("EOWNER"  )) != NULL) {
      if (r_uid   != NULL)  *r_uid = x_uid;
      if (b_owner != NULL)  snprintf (b_owner, LEN_USER , "%s", x_owner);
   }
   if (strchr ("Gêg", ySCORE_value ("EGROUP"  )) != NULL) {
      if (r_gid   != NULL)  *r_gid = x_gid;
      if (b_group != NULL)  snprintf (b_group, LEN_USER , "%s", x_group);
   }
   if (strchr ("P÷p", ySCORE_value ("EPERMS"  )) != NULL) {
      if (r_prm   != NULL)  *r_prm = x_prm;
      if (b_perms != NULL)  snprintf (b_perms, LEN_TERSE, "%s", x_perms);
      if (r_disp  != NULL)  snprintf (r_disp , LEN_LABEL, "%s", x_disp );
   }
   /*---(finalize)-----------------------*/
   if (rc_final < 0)   yaudit_fatal     ("EXPECT"  , "");
   /*---(complete)-----------------------*/
   DEBUG_YENV    yLOG_exit    (__FUNCTION__);
   return rc_final;
}

char
yaudit_extra            (char a_type, int a_major, int a_minor, char a_ttype, char a_target [LEN_PATH], int a_epoch, long a_bytes, int a_inode, char a_hash [LEN_DESC])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        rc_final    = RC_POSITIVE;
   char        x_tdesc     [LEN_TERSE] = "";
   int         l           =   0;
   int         i           =   0;
   char        x_bad       = '-';
   char        x_hack      = '-';
   /*---(header)-------------------------*/
   DEBUG_YENV   yLOG_enter   (__FUNCTION__);
   /*---(device nums)--------------------*/
   --rce;  if (strchr ("bc", a_type) != NULL) {
      if (a_major >= 0 && a_major <= 255)    ySCORE_mark ("EMAJOR"  , 'j');
      else {
         ySCORE_mark ("EMAJOR"  , '°');
         yURG_err ('f', "attempting to use illegal major device number (%d), expeced (0-255)", a_major);
         rc_final = rce;
      }
   } else if (a_major != -1){
      yURG_err ('w', "included superfluous major device number (%d)", a_major);
   }
   --rce;  if (strchr ("bc", a_type) != NULL) {
      if (a_minor >= 0 && a_minor <= 255)    ySCORE_mark ("EMINOR"  , 'n');
      else {
         ySCORE_mark ("EMINOR"  , '°');
         yURG_err ('f', "attempting to use illegal minor device number (%d), expeced (0-255)", a_minor);
         rc_final = rce;
      }
   } else if (a_minor != -1){
      yURG_err ('w', "included superfluous minor device number (%d)", a_minor);
   }
   /*---(target type)--------------------*/
   --rce;  if (strchr ("sh", a_type) != NULL) {
      strlcpy (x_tdesc, yENV_typedesc (a_ttype), LEN_TERSE);
      if (strcmp (x_tdesc, "WTF") != 0)   ySCORE_mark ("ETTYPE"  , a_ttype);
      else {
         ySCORE_mark ("ETTYPE"  , '°');
         yURG_err ('f', "attempting to use illegal target type %s (%c)", x_tdesc, a_ttype);
         rc_final = rce;
      }
   } else if (a_ttype != YENV_NONE){
      yURG_err ('w', "included superfluous target type (%d)", a_ttype);
   }
   --rce;  if (a_type == YENV_HARD) {
      if (a_ttype == 0 || strchr ("rh", a_ttype) == NULL) {
         strlcpy (x_tdesc, yENV_typedesc (a_ttype), LEN_TERSE);
         ySCORE_mark ("ETTYPE"  , '°');
         yURG_err ('f', "attempting to use hardlink to illegal type %s (%c), only allow (rh)", x_tdesc, a_ttype);
         rc_final = rce;
      }
   }
   /*---(target name)--------------------*/
   --rce;  if (strchr ("sh", a_type) != NULL) {
      ySCORE_mark ("ETARGET" , '°');
      if (a_target != NULL && strcmp (a_target, "") != 0) {
         ySCORE_mark ("ETARGET" , '/');
         DEBUG_YENV    yLOG_char    ("a_target", a_target [0]);
         if (a_target [0] != '/') {
            yURG_err ('f', "target must be absolute reference, i.e., start with '/'");
            rc_final = rce;
         } else {
            l = strlen (a_target);
            ySCORE_mark ("ETARGET" , '#');
            x_bad = '-';
            --rce;  for (i = 0; i < l; ++i) {
               if (strchr (YSTR_FILES, a_target [i]) == NULL) {
                  yURG_err ('f', "target name has an illegal character (%c) at position %d", a_target [i], i);
                  rc_final = rce;
                  x_bad = 'y';
               }
            }
            if (x_bad != 'y')   ySCORE_mark ("ETARGET" , 't');
         }
      }
   } else  if (a_target != NULL && strcmp (a_target, "") != 0) {
      yURG_err ('w', "included superfluous target name å%sæ", a_target);
   }
   /*---(specialty)----------------------*/
   if (a_epoch > 0)  { ySCORE_mark ("EEPOCH"  , 'e');  x_hack = 'y';  }
   if (a_bytes > 0)  { ySCORE_mark ("EBYTES"  , 'b');  x_hack = 'y';  }
   if (a_inode > 0)  { ySCORE_mark ("EINODE"  , 'i');  x_hack = 'y';  }
   --rce;  if (a_hash != NULL && strcmp (a_hash, "") != 0) {
      ySCORE_mark ("EHASH"   , '°');
      l = strlen (a_hash);
      if (l == 40) {
         ySCORE_mark ("EHASH"   , '#');
         x_bad = '-';
         for (i = 0; i < l; ++i) {
            if (strchr (YSTR_HEXUP, a_hash [i]) == NULL) {
               yURG_err ('f', "hash code has an illegal character (%c) at position %d", a_hash [i], i);
               rc_final = rce;
               x_bad = 'y';
            }
         }
         if (x_bad != 'y')  {  ySCORE_mark ("EHASH"   , 'h'); x_hack = 'y'; }

      } else {
         yURG_err ('f', "hash code is the wrong length (%d) vs standard (40)", l);
         rc_final = rce;
      }
   }
   /*---(clear some marks)---------------*/
   if (x_hack  == '-') ySCORE_nohacked     ();
   /*---(finalize)-----------------------*/
   if (rc_final < 0)   yaudit_fatal     ("EXPECT"  , "");
   /*---(complete)-----------------------*/
   DEBUG_YENV    yLOG_exit    (__FUNCTION__);
   return rc_final;
}



