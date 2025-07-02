/*===[[ START ]]==============================================================*/
#include    "yAUDIT.h"
#include    "yAUDIT_priv.h"



char
yaudit_fatal            (char a_label [LEN_LABEL], char a_msg [LEN_HUND])
{
   /*---(locals)-----------+-----+-----+-*/
   int          i;
   short        bn, en;
   short        bt, et;
   short        br, er;
   char         x_beg      [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_YENV   yLOG_enter   (__FUNCTION__);
   /*---(message)------------------------*/
   DEBUG_YENV   yLOG_point   ("a_msg"     , a_msg);
   if (a_msg != NULL && strcmp (a_msg, "") != 0) {
      DEBUG_YENV   yLOG_info    ("a_msg"     , a_msg);
      yURG_err ('f', a_msg);
   }
   /*---(failure marks)------------------*/
   DEBUG_YENV   yLOG_info    ("a_label"   , a_label);
   ySCORE_mark (a_label   , '°');
   ySCORE_mark ("FINAL"   , '°');
   /*---(find mask beginning)------------*/
   switch (a_label [0]) {
   case 'N' : strcpy (x_beg, "EXá ");  break;
   case 'E' : strcpy (x_beg, "PRá ");  break;
   case 'P' : strcpy (x_beg, "FXá ");  break;
   case 'F' : strcpy (x_beg, "REá ");  break;
   case 'R' : strcpy (x_beg, "");      break;
   case 'J' : return 0;                break;
   case 'H' : return 0;                break;
   default  : return 0;                break;
   }
   DEBUG_YENV   yLOG_info    ("x_beg"     , x_beg);
   /*---(masking for readability)--------*/
   if (strcmp (x_beg, "") != 0)   ySCORE_mask (x_beg, "RECHECK");
   ySCORE_nohacked     ();
   /*---(complete)-----------------------*/
   DEBUG_YENV    yLOG_exit    (__FUNCTION__);
   return RC_POSITIVE;
}

char
yaudit_prepare          (char a_type, char c_flag, char c_naming, char a_dir [LEN_PATH], char a_file [LEN_PATH], char r_tdesc [LEN_TERSE], char *r_check, char *r_force, char *r_fix)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_tdesc     [LEN_TERSE] = "";
   char        x_check     =  '-';
   char        x_force     =  '-';
   char        x_fix       =  '-';
   char        x_msg       [LEN_HUND]  = "";
   /*---(header)-------------------------*/
   DEBUG_YENV   yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   /*> yenv_score_clear ();                                                           <*/
   ySCORE_init (myAUDIT_table);
   /*---(default)------------------------*/
   if (r_tdesc != NULL)  strcpy (r_tdesc, "");
   if (r_check != NULL)  *r_check = '-';
   if (r_force != NULL)  *r_force = '-';
   if (r_fix   != NULL)  *r_fix   = '-';
   /*---(check a_type)-------------------*/
   ySCORE_mark ("ETYPE"   , '°');
   DEBUG_YENV   yLOG_char    ("a_type"    , a_type);
   --rce;  if (a_type == 0 || strchr (YENV_TYPES, a_type) == NULL) {
      ySCORE_mask ("NMá ", "NAME");
      sprintf (x_msg, "a_type (%3d) must be one of å%sæ (blatant error)", a_type, YENV_TYPES);
      yaudit_fatal     ("EXPECT"  , x_msg);
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(type description)---------------*/
   strlcpy (x_tdesc, yENV_typedesc (a_type), LEN_TERSE);
   if (strcmp (x_tdesc, "WTF") != 0)   ySCORE_mark ("ETYPE"   , a_type);
   /*---(check c_naming)-----------------*/
   ySCORE_mark ("NCONF"   , '°');
   DEBUG_YENV   yLOG_char    ("c_naming"  , c_naming);
   --rce;  if (c_naming == 0 || strchr (YENV_NAMING, c_naming) == NULL) {
      sprintf (x_msg, "c_naming flag (%c) must be one of å%sæ (blatant error)", c_naming, YENV_NAMING);
      yaudit_fatal     ("NAME"    , x_msg);
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   ySCORE_mark ("NCONF"   , c_naming);
   /*---(check a_dir)--------------------*/
   ySCORE_mark ("NDIR"    , '°');
   DEBUG_YENV   yLOG_point   ("a_dir"     , a_dir);
   --rce;  if (a_dir  == NULL) {
      yaudit_fatal     ("NAME"    , "directory name must not be NULL (blatant error)");
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   ySCORE_mark ("NDIR"    , '£');
   DEBUG_YENV   yLOG_info    ("a_dir"     , a_dir);
   --rce;  if (a_dir  [0] == '\0') {
      yaudit_fatal     ("NAME"    , "directory name must not be empty");
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   ySCORE_mark ("NDIR"    , '/');
   --rce;  if (a_dir [0] != '/') {
      yaudit_fatal     ("NAME"    , "directory name must be absolute path");
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   ySCORE_mark ("NDIR"   , 'D');
   /*---(message)------------------------*/
   yURG_msg ('-', "requested dir  %3då%sæ", strlen (a_dir), a_dir);
   /*---(check a_file)-------------------*/
   ySCORE_mark ("NFILE"   , '°');
   DEBUG_YENV   yLOG_point   ("a_file"     , a_file);
   --rce;  if (a_file == NULL) {
      yaudit_fatal     ("NAME"    , "file name must not be NULL (blatant error)");
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   ySCORE_mark ("NFILE"   , '£');
   DEBUG_YENV   yLOG_info    ("a_file"    , a_file);
   --rce;  if (a_type != YENV_NONE && a_type != YENV_DIR) {
      if (a_file [0] == '\0') {
         yaudit_fatal     ("NAME"    , "file name must not be empty (non-dir types)");
         DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      ySCORE_mark ("NFILE"   , '/');
   }
   ySCORE_mark ("NFILE"   , '*');
   --rce;  if (a_type == YENV_DIR) {
      if (a_file [0] != '\0') {
         yaudit_fatal     ("NAME"    , "file name MUST be empty (dir types)");
         DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      ySCORE_mark ("NFILE"   , '-');
   }
   if (a_type == YENV_NONE) {
      if (a_file [0] == '£')    ySCORE_mark ("NFILE"   , '-');
   }
   if (ySCORE_value ("NFILE") != '-')  ySCORE_mark ("NFILE"   , '/');
   --rce;  if (a_file [0] != '\0' && strchr (a_file, '/') != NULL) {
      yaudit_fatal     ("NAME"    , "file name must not have a path (/)");
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (ySCORE_value ("NFILE") != '-')  ySCORE_mark ("NFILE"   , 'F');
   /*---(message)------------------------*/
   yURG_msg ('-', "requested file %3då%sæ", strlen (a_file), a_file);
   /*---(flags)--------------------------*/
   ySCORE_mark ("EFLAG"   , '°');
   --rce;  switch (c_flag) {
   case '!' :  x_check = 'y';  x_force = '!';  x_fix   = 'y';  break;
   case 'F' :  x_check = 'y';  x_force = 'y';  x_fix   = 'y';  break;
   case 'f' :  x_check = 'y';  x_force = '-';  x_fix   = 'y';  break;
   case '-' :  x_check = '-';  x_force = '-';  x_fix   = '-';  break;
   default  :
               if (c_flag < 32 || c_flag >= 127)  sprintf (x_msg, "configuration flag (%d) is not legal (!Ff-) (blatant error)", c_flag);
               else                               sprintf (x_msg, "configuration flag (%c) is not legal (!Ff-) (blatant error)", c_flag);
               yaudit_fatal     ("FIXES"   , x_msg);
               DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
               return rce;
   }
   ySCORE_mark ("EFLAG"   , c_flag);
   ySCORE_mark ("CONFC"   , x_check);
   ySCORE_mark ("CONFF"   , x_force);
   ySCORE_mark ("CONFX"   , x_fix);
   /*---(clear some marks)---------------*/
   if (x_check == '-') ySCORE_nocheck ();
   /*---(save-back)----------------------*/
   if (r_tdesc != NULL)  snprintf (r_tdesc, LEN_TERSE, "%s", x_tdesc);
   if (r_check != NULL)  *r_check = x_check;
   if (r_force != NULL)  *r_force = x_force;
   if (r_fix   != NULL)  *r_fix   = x_fix;
   /*---(complete)-----------------------*/
   DEBUG_YENV    yLOG_exit    (__FUNCTION__);
   return RC_POSITIVE;
}
