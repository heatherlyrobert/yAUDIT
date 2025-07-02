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
