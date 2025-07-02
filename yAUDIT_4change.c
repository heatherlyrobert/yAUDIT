/*===[[ START ]]==============================================================*/
#include    "yAUDIT.h"
#include    "yAUDIT_priv.h"



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
      yaudit_fatal     ("FIXES"   , x_msg);
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(critical removals)--------------*/
   if (c_del == '!') {
      if (c_force == '!') {
         rc = yENV_removier (a_atype, a_full);
         --rce;  if (rc != YENV_NONE) {
            sprintf (x_msg, "%s (%c) conflicting entry removal (%c) failed (%d/%c)" , a_atdesc, a_atype, c_del, rc, rc);
            yaudit_fatal     ("FIXES"   , x_msg);
            DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
      } else {
         ySCORE_mark ("REMOVE"  , '!');
         sprintf (x_msg, "%s (%c) conflicting entry removal (%c) requires critical force (!), but used (%c)" , a_atdesc, a_atype, c_del, c_force);
         yaudit_fatal     ("FIXES"   , x_msg);
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
            yaudit_fatal     ("FIXES"   , x_msg);
            DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
      } else {
         ySCORE_mark ("REMOVE"  , '!');
         sprintf (x_msg, "%s (%c) conflicting entry removal (%c) requires any force (!y), but used (%c)" , a_atdesc, a_atype, c_del, c_force);
         yaudit_fatal     ("FIXES"   , x_msg);
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
      yaudit_fatal     ("FIXES"   , x_msg);
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(additions)----------------------*/
   if (c_force != '-') {
      rc = yENV_touchier (a_etype, a_full, a_eowner, a_egroup, a_eperms, a_emajor, a_eminor, a_etarget);
      DEBUG_YENV   yLOG_complex ("touch"     , "%d/%c", rc, rc);
      --rce;  if (rc < 0 || rc == YENV_NONE) {
         sprintf (x_msg, "%s (%c) requested creation (%c) failed (%d/%c)" , a_etdesc, a_etype, c_add, rc, rc);
         yaudit_fatal     ("FIXES"   , x_msg);
         DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   } else {
      ySCORE_mark ("CREATE"  , '!');
      sprintf (x_msg, "%s (%c) requested creation (%c) requires any force (!y), but used (%c)" , a_etdesc, a_etype, c_add, c_force);
      yaudit_fatal     ("FIXES"   , x_msg);
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
      yaudit_fatal     ("FIXES"   , x_msg);
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update)-------------------------*/
   if (c_fix != '-') {
      rc = yENV_touchier (a_atype, a_full, a_eowner, a_egroup, a_eperms, a_emajor, a_eminor, a_etarget);
      DEBUG_YENV   yLOG_complex ("touch"     , "%d/%c", rc, rc);
      --rce;  if (rc < 0 || rc == YENV_NONE) {
         sprintf (x_msg, "%s (%c) requested update (%c) failed (%d/%c)" , a_atdesc, a_atype, c_upd, rc, rc);
         yaudit_fatal     ("FIXES"   , x_msg);
         DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   } else {
      ySCORE_mark ("UPDATE"  , '!');
      sprintf (x_msg, "%s (%c) requested update (%c) requires fix (y), but used (%c)" , a_atdesc, a_atype, c_upd, c_fix);
      yaudit_fatal     ("FIXES"   , x_msg);
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



