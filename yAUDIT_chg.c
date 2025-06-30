/*===[[ START ]]==============================================================*/
#include    "yAUDIT.h"
#include    "yAUDIT_priv.h"



char
yaudit_precheck         (char a_full [LEN_PATH], char a_etype, char a_eowner [LEN_USER], char a_egroup [LEN_USER], char a_eperms [LEN_TERSE], int a_emajor, int a_eminor, char a_ettype, char a_etarget [LEN_PATH], char *r_atype, char r_atdesc [LEN_TERSE], char *r_del, char *r_add, char *r_upd, char c_force, char c_fix)
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
   char        x_miss      [LEN_HUND]  = "";
   int         x_amajor    =   -1;
   int         x_aminor    =   -1;
   char        x_alink     [LEN_PATH]  = "";
   int         x_inode     =   -1;
   int         n           =   -1;
   char        x_del       =  '-';
   char        x_add       =  '-';
   char        x_upd       =  '-';
   int         l           =    0;
   int         i           =    0;
   /*---(quick-out)----------------------*/
   if (c_force == '-' && c_fix == '-') {
      DEBUG_YENV   yLOG_senter  (__FUNCTION__);
      DEBUG_YENV   yLOG_snote   ("no precheck requested");
      DEBUG_YENV   yLOG_sexit   (__FUNCTION__);
      return RC_ACK;
   }
   /*---(header)-------------------------*/
   DEBUG_YENV   yLOG_enter   (__FUNCTION__);
   /*---(defaults))----------------------*/
   if (r_atype  != NULL)   *r_atype = '-';
   if (r_atdesc != NULL)   strcpy (r_atdesc, "");
   if (r_del    != NULL)   *r_del   = '-';
   if (r_add    != NULL)   *r_add   = '-';
   if (r_upd    != NULL)   *r_upd   = '-';
   /*---(scores)-------------------------*/
   ySCORE_mark ("FDEL"    , '-');
   ySCORE_mark ("FADD"    , '-');
   ySCORE_mark ("FUPD"    , '-');
   /*---(get data)-----------------------*/
   x_atype = yENV_detail (a_full, x_atdesc, NULL, x_aowner, NULL, x_agroup, NULL, x_aperms, NULL, NULL, NULL, &x_amajor, &x_aminor, x_alink, NULL, &x_inode, NULL);
   DEBUG_YENV   yLOG_value   ("detail"    , x_atype);
   --rce;  if (x_atype <= 0) {
      yURG_err ('f', "file check generated a hard error (%d)", x_atype);
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   ySCORE_mark ("CTYPE"   , x_atype);
   /*---(quick out)----------------------*/
   DEBUG_YENV   yLOG_char    ("x_atype"   , x_atype);
   if (x_atype == YENV_NONE && a_etype == YENV_NONE) {
      yURG_msg ('-', "non-existance of filesystem entry confirmed");
      return RC_POSITIVE;
   }
   /*---(existance)----------------------       ----del----; ----add----; ----upd----;*/
   if      (x_atype  == YENV_NONE)            {            ; x_add = 'y';            ;  strcat (x_miss, "non-existance, "); }
   else if (a_etype  == YENV_NONE)            { x_del = 'y';            ;            ;  strcat (x_miss, "existance, "); }
   /*---(type changes)-------------------       ----del----; ----add----; ----upd----;*/
   else if (x_atype  != a_etype) {
      ySCORE_mark ("CTYPE"   , toupper (x_atype));
      if (strchr ("hs", a_etype) != NULL &&
            strchr ("hs", x_atype) != NULL)   { x_del = 'y'; x_add = 'y';            ;  strcat (x_miss, "type, "     ); }
      else                                    { x_del = '!'; x_add = 'y';            ;  strcat (x_miss, "type, "     ); }
   }
   /*---(entry details)------------------       ----del----; ----add----; ----upd----;*/
   if (x_atype == a_etype) {
      switch (a_etype) {
      case YENV_BLOCK : case YENV_CHAR  :
         ySCORE_mark ("CMAJOR"  , 'j');
         ySCORE_mark ("CMINOR"  , 'n');
         if (x_amajor != a_emajor)            { x_del = 'y'; x_add = 'y';            ;  strcat (x_miss, "maj, "      );  ySCORE_mark ("CMAJOR"  , 'J'); }
         if (x_aminor != a_eminor)            { x_del = 'y'; x_add = 'y';            ;  strcat (x_miss, "min, "      );  ySCORE_mark ("CMINOR"  , 'N'); }
         break;
      case YENV_SYM   :
         rc = yENV_detail (a_etarget, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
         ySCORE_mark ("CTTYPE"  , rc);
         if (rc != a_ettype)  ySCORE_mark ("CTTYPE"  , toupper (rc));
         ySCORE_mark ("CTARGET" , 't');
         if (strcmp (x_alink, a_etarget) != 0) { x_del = 'y'; x_add = 'y';            ;  strcat (x_miss, "target, "   );  ySCORE_mark ("CTARGET" , 'T'); }

         break;
      case YENV_HARD  :
         rc = yENV_detail (a_etarget, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &n, NULL);
         ySCORE_mark ("CTTYPE"  , rc);
         if (rc != a_ettype) {
            if (a_ettype == YENV_REG && rc == YENV_HARD)  ySCORE_mark ("CTTYPE"  , YENV_REG);
            else                                          ySCORE_mark ("CTTYPE"  , toupper (rc));
         }
         ySCORE_mark ("CTARGET" , 't');
         if (x_inode != n)                    { x_del = 'y'; x_add = 'y';            ;  strcat (x_miss, "target, "   );  ySCORE_mark ("CTARGET" , 'T'); }
         break;
      }
   }
   /*---(small fixest)-------------------       ----del----; ----add----; ----upd----;*/
   if (x_atype == a_etype && x_add != 'y' && a_etype != YENV_SYM) {
      ySCORE_mark ("COWNER"  , 'o');
      ySCORE_mark ("CGROUP"  , 'g');
      ySCORE_mark ("CPERMS"  , 'p');
      if (strcmp (x_aowner, a_eowner) != 0)   {            ;            ; x_upd = 'y';  strcat (x_miss, "owner, "    );  ySCORE_mark ("COWNER"  , 'O'); }
      if (strcmp (x_agroup, a_egroup) != 0)   {            ;            ; x_upd = 'y';  strcat (x_miss, "group, "    );  ySCORE_mark ("CGROUP"  , 'G'); }
      if (strcmp (x_aperms, a_eperms) != 0)   {            ;            ; x_upd = 'y';  strcat (x_miss, "perms, "    );  ySCORE_mark ("CPERMS"  , 'P'); }
   }
   /*---(entry fixes)--------------------       ----del----; ----add----; ----upd----;*/
   l = strlen (x_miss);
   if (l > 0) {
      ySCORE_mark ("CHECK"   , 'Þ');
      if (l > 2)  x_miss [l - 2] = '\0';
      yURG_err ('w', "troubles with entry %s", x_miss);
      rc_final = RC_POSITIVE;
   } else {
      ySCORE_mark ("CHECK"   , '´');
   }
   /*---(finalize)-----------------------*/
   if (x_del == '!')  ySCORE_mark ("FDEL"    , '!');
   if (x_del == 'y')  ySCORE_mark ("FDEL"    , 'r');
   if (x_add != '-')  ySCORE_mark ("FADD"    , 'c');
   if (x_upd != '-')  ySCORE_mark ("FUPD"    , 'u');
   /*---(save-back)----------------------*/
   if (r_atype  != NULL)   *r_atype = x_atype;
   if (r_atdesc != NULL)   strlcpy (r_atdesc, x_atdesc, LEN_TERSE);
   if (r_del    != NULL)   *r_del   = x_del;
   if (r_add    != NULL)   *r_add   = x_add;
   if (r_upd    != NULL)   *r_upd   = x_upd;
   /*---(complete)-----------------------*/
   DEBUG_YENV    yLOG_exit    (__FUNCTION__);
   return rc_final;
}

char
yaudit_remove           (char a_full [LEN_PATH], char a_atype, char a_atdesc [LEN_TERSE], char c_force, char c_del)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_msg       [LEN_HUND]  = "";
   /*---(quick-out)----------------------*/
   if (a_atype == YENV_NONE) {
      DEBUG_YENV   yLOG_senter  (__FUNCTION__);
      DEBUG_YENV   yLOG_snote   ("does not exist, removal not required");
      DEBUG_YENV   yLOG_sexit   (__FUNCTION__);
      return RC_ACK;
   }
   if (c_del == '-') {
      DEBUG_YENV   yLOG_senter  (__FUNCTION__);
      DEBUG_YENV   yLOG_snote   ("no removal necessary");
      DEBUG_YENV   yLOG_sexit   (__FUNCTION__);
      return RC_ACK;
   }
   /*---(header)-------------------------*/
   DEBUG_YENV   yLOG_enter   (__FUNCTION__);
   ySCORE_mark ("REMOVE"  , '°');
   /*---(does not exist)-----------------*/
   rc = yENV_exists (a_full);
   DEBUG_YENV   yLOG_complex ("exists"    , "%d/%c", rc, rc);
   --rce;  if (rc < 0 || rc == YENV_NONE) {
      ySCORE_mark ("REMOVE"  , '?');
      sprintf (x_msg, "%s (%c) removal impossible, entry does not exist" , a_atdesc, a_atype);
      yenv_audit_fatal ("FIXES"   , x_msg);
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(critical removals)--------------*/
   if (c_del == '!') {
      if (c_force == '!') {
         rc = yENV_removier (a_atype, a_full);
         --rce;  if (rc != YENV_NONE) {
            sprintf (x_msg, "%s (%c) conflicting entry removal (%c) failed (%d/%c)" , a_atdesc, a_atype, c_del, rc, rc);
            yenv_audit_fatal ("FIXES"   , x_msg);
            DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
      } else {
         ySCORE_mark ("REMOVE"  , '!');
         sprintf (x_msg, "%s (%c) conflicting entry removal (%c) requires critical force (!), but used (%c)" , a_atdesc, a_atype, c_del, c_force);
         yenv_audit_fatal ("FIXES"   , x_msg);
         DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(normal removals)----------------*/
   else {
      if (c_force != '-') {
         rc = yENV_removier (a_atype, a_full);
         --rce;  if (rc != YENV_NONE) {
            sprintf (x_msg, "%s (%c) conflicting entry removal (%c) failed (%d/%c)" , a_atdesc, a_atype, c_del, rc, rc);
            yenv_audit_fatal ("FIXES"   , x_msg);
            DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
      } else {
         ySCORE_mark ("REMOVE"  , '!');
         sprintf (x_msg, "%s (%c) conflicting entry removal (%c) requires any force (!y), but used (%c)" , a_atdesc, a_atype, c_del, c_force);
         yenv_audit_fatal ("FIXES"   , x_msg);
         DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(message)------------------------*/
   ySCORE_mark ("REMOVE"  , 'R');
   yURG_msg ('-', "%s (%c) conflicting entry removed successfully because of force flag (%c)", a_atdesc, a_atype, c_force);
   /*---(complete)-----------------------*/
   DEBUG_YENV    yLOG_exit    (__FUNCTION__);
   return RC_REPAIR;
}

char
yaudit_create           (char a_full [LEN_PATH], char a_etype, char a_etdesc [LEN_TERSE], char a_eowner [LEN_USER], char a_egroup [LEN_USER], char a_eperms [LEN_TERSE], int a_emajor, int a_eminor, char a_etarget [LEN_PATH], char c_force, char c_add)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_tdesc     [LEN_TERSE] = "";
   char        x_msg       [LEN_HUND]  = "";
   /*---(quick-out)----------------------*/
   if (a_etype == YENV_NONE) {
      DEBUG_YENV   yLOG_senter  (__FUNCTION__);
      DEBUG_YENV   yLOG_snote   ("requested non-exist, creation not required");
      DEBUG_YENV   yLOG_sexit   (__FUNCTION__);
      return RC_ACK;
   }
   if (c_add == '-') {
      DEBUG_YENV   yLOG_senter  (__FUNCTION__);
      DEBUG_YENV   yLOG_snote   ("no creation necessary");
      DEBUG_YENV   yLOG_sexit   (__FUNCTION__);
      return RC_ACK;
   }
   /*---(header)-------------------------*/
   DEBUG_YENV   yLOG_enter   (__FUNCTION__);
   ySCORE_mark ("CREATE"  , '°');
   /*---(already exists)-----------------*/
   rc = yENV_exists (a_full);
   DEBUG_YENV   yLOG_complex ("exists"    , "%d/%c", rc, rc);
   --rce;  if (rc != YENV_NONE) {
      ySCORE_mark ("CREATE"  , '?');
      strlcpy (x_tdesc, yENV_typedesc (rc), LEN_TERSE);
      sprintf (x_msg, "%s (%c) creation stopped, FOUND it as %s (%c) already" , a_etdesc, a_etype, x_tdesc, rc);
      yenv_audit_fatal ("FIXES"   , x_msg);
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(additions)----------------------*/
   if (c_force != '-') {
      rc = yENV_touchier (a_etype, a_full, a_eowner, a_egroup, a_eperms, a_emajor, a_eminor, a_etarget);
      DEBUG_YENV   yLOG_complex ("touch"     , "%d/%c", rc, rc);
      --rce;  if (rc < 0 || rc == YENV_NONE) {
         sprintf (x_msg, "%s (%c) requested creation (%c) failed (%d/%c)" , a_etdesc, a_etype, c_add, rc, rc);
         yenv_audit_fatal ("FIXES"   , x_msg);
         DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   } else {
      ySCORE_mark ("CREATE"  , '!');
      sprintf (x_msg, "%s (%c) requested creation (%c) requires any force (!y), but used (%c)" , a_etdesc, a_etype, c_add, c_force);
      yenv_audit_fatal ("FIXES"   , x_msg);
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(message)------------------------*/
   ySCORE_mark ("CREATE"  , 'C');
   yURG_msg ('-', "%s (%c) entry created successfully because of force flag (%c)", a_etdesc, a_etype, c_force);
   /*---(complete)-----------------------*/
   DEBUG_YENV    yLOG_exit    (__FUNCTION__);
   return RC_REPAIR;
}

char
yaudit_update           (char a_full [LEN_PATH], char a_atype, char a_atdesc [LEN_TERSE], char a_eowner [LEN_USER], char a_egroup [LEN_USER], char a_eperms [LEN_TERSE], int a_emajor, int a_eminor, char a_etarget [LEN_PATH], char c_fix, char c_upd)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_msg       [LEN_HUND]  = "";
   /*---(quick-out)----------------------*/
   if (a_atype == YENV_NONE) {
      DEBUG_YENV   yLOG_senter  (__FUNCTION__);
      DEBUG_YENV   yLOG_snote   ("requested non-exist, update not required");
      DEBUG_YENV   yLOG_sexit   (__FUNCTION__);
      return RC_ACK;
   }
   if (c_upd == '-') {
      DEBUG_YENV   yLOG_senter  (__FUNCTION__);
      DEBUG_YENV   yLOG_snote   ("no update necessary");
      DEBUG_YENV   yLOG_sexit   (__FUNCTION__);
      return RC_ACK;
   }
   /*---(header)-------------------------*/
   DEBUG_YENV   yLOG_enter   (__FUNCTION__);
   ySCORE_mark ("UPDATE"  , '°');
   /*---(does not exist)-----------------*/
   rc = yENV_exists (a_full);
   DEBUG_YENV   yLOG_complex ("exists"    , "%d/%c", rc, rc);
   --rce;  if (rc < 0 || rc == YENV_NONE) {
      ySCORE_mark ("UPDATE"  , '?');
      sprintf (x_msg, "%s (%c) update stopped, does not exist" , a_atdesc, a_atype);
      yenv_audit_fatal ("FIXES"   , x_msg);
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update)-------------------------*/
   if (c_fix != '-') {
      rc = yENV_touchier (a_atype, a_full, a_eowner, a_egroup, a_eperms, a_emajor, a_eminor, a_etarget);
      DEBUG_YENV   yLOG_complex ("touch"     , "%d/%c", rc, rc);
      --rce;  if (rc < 0 || rc == YENV_NONE) {
         sprintf (x_msg, "%s (%c) requested update (%c) failed (%d/%c)" , a_atdesc, a_atype, c_upd, rc, rc);
         yenv_audit_fatal ("FIXES"   , x_msg);
         DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   } else {
      ySCORE_mark ("UPDATE"  , '!');
      sprintf (x_msg, "%s (%c) requested update (%c) requires fix (y), but used (%c)" , a_atdesc, a_atype, c_upd, c_fix);
      yenv_audit_fatal ("FIXES"   , x_msg);
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(message)------------------------*/
   ySCORE_mark ("UPDATE"  , 'U');
   yURG_msg ('-', "%s (%c) entry updated successfully because of fix flag (%c)", a_atdesc, a_atype, c_fix);
   /*---(complete)-----------------------*/
   DEBUG_YENV    yLOG_exit    (__FUNCTION__);
   return RC_REPAIR;
}



