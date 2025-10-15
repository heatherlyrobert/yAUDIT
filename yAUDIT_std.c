/*===[[ START ]]==============================================================*/
#include    "yAUDIT.h"
#include    "yAUDIT_priv.h"



char
yaudit__prefix            (char a_file [LEN_PATH], short a_beg, char a_prefix [LEN_TERSE], char r_pre [LEN_TERSE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        t           [LEN_LABEL] = "";
   char        x_msg       [LEN_HUND]  = "";
   /*---(header)-------------------------*/
   DEBUG_YENV   yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_pre != NULL)  strlcpy (r_pre, "", LEN_TERSE);
   /*---(defense)------------------------*/
   DEBUG_YENV    yLOG_value   ("a_beg"     , a_beg);
   --rce;  if (a_beg <= 0) {
      DEBUG_YENV   yLOG_exit    (__FUNCTION__);
      return RC_ACK;
   }
   /*---(scoring)------------------------*/
   ySCORE_mark (myAUDIT.m_yscore, "NPREFIX" , '°');
   /*---(save-back)----------------------*/
   strlcpy (t, a_file, a_beg + 2);
   if (r_pre != NULL)  strlcpy (r_pre, t, LEN_TERSE);
   /*---(user name option)---------------*/
   DEBUG_YENV    yLOG_info    ("a_prefix"  , a_prefix);
   if (strcmp (a_prefix, "(USER).") == 0) {
      DEBUG_YENV    yLOG_note    ("using user name prefix handler");
      strlcpy (t, a_file, a_beg + 1);
      DEBUG_YENV    yLOG_info    ("t"         , t);
      rc = yENV_user_data ('n', t, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
      if (rc < 0) {
         sprintf (x_msg, "name prefix å%sæ does not match any system user (illegal)", t);
         yaudit_fatal     ("NAME"    , x_msg);
         DEBUG_YENV    yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      ySCORE_mark (myAUDIT.m_yscore, "NPREFIX" , 'u');
   }
   /*---(normal option)------------------*/
   else {
      DEBUG_YENV    yLOG_note    ("using normal prefix handler");
      strlcpy (t, a_file, a_beg + 2);
      DEBUG_YENV    yLOG_info    ("t"         , t);
      if (strcmp (t, a_prefix) != 0) {
         sprintf (x_msg, "name prefix å%sæ does not match given standard å%sæ (illegal)", t, a_prefix);
         yaudit_fatal     ("NAME"    , x_msg);
         DEBUG_YENV    yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      ySCORE_mark (myAUDIT.m_yscore, "NPREFIX" , 'p');
   }
   /*---(complete)-----------------------*/
   DEBUG_YENV   yLOG_exit    (__FUNCTION__);
   return RC_POSITIVE;
}

char
yaudit__suffix          (char a_file [LEN_PATH], short a_end, char a_suffix [LEN_TERSE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        t           [LEN_LABEL] = "";
   char        x_msg       [LEN_HUND]  = "";
   /*---(header)-------------------------*/
   DEBUG_YENV   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YENV    yLOG_value   ("a_end"     , a_end);
   --rce;  if (a_end < 0) {
      DEBUG_YENV   yLOG_exit    (__FUNCTION__);
      return RC_ACK;
   }
   /*---(scoring)------------------------*/
   ySCORE_mark (myAUDIT.m_yscore, "NSUFFIX" , '°');
   /*---(normal option)------------------*/
   DEBUG_YENV    yLOG_info    ("a_suffix"  , a_suffix);
   strlcpy (t, a_file + a_end, LEN_TERSE);
   DEBUG_YENV    yLOG_info    ("t"         , t);
   if (strcmp (t, a_suffix) != 0) {
      sprintf (x_msg, "name suffix å%sæ does not match given standard å%sæ (illegal)", t, a_suffix);
      yaudit_fatal     ("NAME"    , x_msg);
      DEBUG_YENV    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(scoring)------------------------*/
   ySCORE_mark (myAUDIT.m_yscore, "NSUFFIX" , 's');
   /*---(complete)-----------------------*/
   DEBUG_YENV   yLOG_exit    (__FUNCTION__);
   return RC_POSITIVE;
}

char
yaudit_standard         (char a_type, char c_naming, char c_style, char a_dir [LEN_PATH], char a_file [LEN_PATH], char a_prefix [LEN_TERSE], char a_suffix [LEN_TERSE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   short       x_beg       =   -1;
   short       x_end       =   -1;
   int         l           =    0;
   char        x_pre       [LEN_TERSE] = "";
   short       x_rem       =    0;
   char        x_msg       [LEN_HUND]  = "";
   char        x_type      =  '-';
   /*---(header)-------------------------*/
   DEBUG_YENV   yLOG_enter   (__FUNCTION__);
   /*---(quick-out)----------------------*/
   DEBUG_YENV   yLOG_char    ("a_type"    , a_type);
   if (a_type   != YENV_REG) {
      DEBUG_YENV   yLOG_note    ("standards only for regular files");
      DEBUG_YENV   yLOG_exit    (__FUNCTION__);
      return RC_ACK;
   }
   DEBUG_YENV   yLOG_char    ("c_naming"  , c_naming);
   if (c_naming == 0 || strchr ("cl", c_naming) == NULL) {
      DEBUG_YENV   yLOG_note    ("not using standards naming check");
      DEBUG_YENV   yLOG_exit    (__FUNCTION__);
      return RC_ACK;
   }
   DEBUG_YENV   yLOG_char    ("c_style"   , c_style);
   if (c_style  != 'b') {
      DEBUG_YENV   yLOG_note    ("full name is not both dir and file");
      DEBUG_YENV   yLOG_exit    (__FUNCTION__);
      return RC_ACK;
   }
   /*---(handle dots)--------------------*/
   rc = yaudit_dots         (c_naming, a_file, a_prefix, a_suffix, &x_beg, &x_end);
   DEBUG_YENV   yLOG_value   ("dots"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(handle prefix)------------------*/
   rc = yaudit__prefix      (a_file, x_beg, a_prefix, x_pre);
   DEBUG_YENV   yLOG_value   ("prefix"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(handle suffix)------------------*/
   rc = yaudit__suffix      (a_file, x_end, a_suffix);
   DEBUG_YENV   yLOG_value   ("suffix"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(set type)-----------------------*/
   if      (x_beg > 0 && x_end > 0)  x_type = 'b';
   else if (x_beg > 0)               x_type = 'p';
   else if (x_end > 0)               x_type = 's';
   else                              x_type = '-';
   DEBUG_YENV   yLOG_char    ("x_type"    , x_type);
   if (x_type  == '-') {
      DEBUG_YENV   yLOG_exit    (__FUNCTION__);
      return RC_ACK;
   }
   /*---(remaining description)----------*/
   ySCORE_mark (myAUDIT.m_yscore, "NDESC"   , '°');
   l = strlen (a_file);
   switch (x_type) {
   case 'b' :  x_rem = x_end - x_beg - 1;  break;
   case 'p' :  x_rem = l - x_beg - 1;      break;
   case 's' :  x_rem = x_end - 1;          break;
   }
   DEBUG_YENV   yLOG_value   ("x_rem"     , x_rem);
   --rce;  if (c_naming == YENV_LOCAL && x_rem < 3) {
      yaudit_fatal     ("NAME"    , "file description less than 3 chars (lazy)");
      DEBUG_YENV    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   ySCORE_mark (myAUDIT.m_yscore, "NDESC"   , 'd');
   /*---(done)---------------------------*/
   switch (x_type) {
   case 'b' :  yURG_msg ('-', "prefix å%sæ and suffix å%sæ match given standard", x_pre, a_suffix);  break;
   case 'p' :  yURG_msg ('-', "prefix å%sæ matches given standard", x_pre);                          break;
   case 's' :  yURG_msg ('-', "suffix å%sæ matches given standard", a_suffix);                       break;
   }
   /*---(complete)-----------------------*/
   DEBUG_YENV   yLOG_exit    (__FUNCTION__);
   return RC_POSITIVE;
}
