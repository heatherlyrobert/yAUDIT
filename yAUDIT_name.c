/*===[[ START ]]==============================================================*/
#include    "yAUDIT.h"
#include    "yAUDIT_priv.h"


char
yaudit_quality          (char a_type, char c_naming, char a_dir [LEN_PATH], char a_file [LEN_PATH], char *r_style, char r_full [LEN_PATH])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         ld          =    0;
   int         lf          =    0;
   char        x_valid     [LEN_FULL]  = "";
   char        x_full      [LEN_PATH]  = "";
   int         l           =    0;
   char        x_style     =  '-';
   int         i           =    0;
   uchar      *p           = NULL;
   int         c           =    0;
   char        x_msg       [LEN_HUND]  = "";
   /*---(header)-------------------------*/
   DEBUG_YENV   yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_style != NULL)  *r_style = '-';
   if (r_full  != NULL)  strcpy (r_full , "");
   ySCORE_mark (myAUDIT.m_yscore, "NFULL"   , '°');
   /*---(create full)--------------------*/
   rc = yENV_name_full (a_dir, a_file, &x_style, x_full);
   DEBUG_YENV   yLOG_value   ("full"      , rc);
   --rce;  if (rc < 0)  {
      yaudit_fatal     ("NAME"    , "trouble with creating full name");
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   l = strlen (x_full);
   yURG_msg ('-', "full name      %3då%sæ", l, x_full);
   DEBUG_YENV    yLOG_complex ("x_full"    , "%3då%sæ", l, x_full);
   /*---(hidden file)--------------------*/
   if (c_naming != YENV_WILD) {
      ySCORE_mark (myAUDIT.m_yscore, "NFULL"   , '´');
      DEBUG_YENV    yLOG_char    ("first char", a_file [0]);
      --rce;  if (a_file [0] == '.') {
         sprintf (x_msg, "file can not be hidden, lead period, in (%c) mode (security risk)", c_naming);
         yaudit_fatal     ("NAME"    , x_msg);
         DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(check name quality)-------------*/
   ySCORE_mark (myAUDIT.m_yscore, "NFULL"   , '¢');
   switch (c_naming) {
   case YENV_WILD :  strcpy (x_valid, YSTR_EXTEN);  break;
   default        :  strcpy (x_valid, YSTR_FILES);  break;
   }
   l = strlen (x_full);
   --rce;  for (i = 0; i < l; ++i) {
      if (strchr (x_valid, x_full [i]) == NULL) {
         sprintf (x_msg, "full name has an illegal character (%c) at position %d in (%c) mode", x_full [i], i, c_naming);
         yaudit_fatal     ("NAME"    , x_msg);
         DEBUG_YENV    yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(success)------------------------*/
   ySCORE_mark (myAUDIT.m_yscore, "NFULL"   , 'n');
   switch (c_naming) {
   case YENV_WILD :  yURG_msg ('-', "name (%c/%c) is fully qualified and uses only legal characters", x_style, c_naming);                    break;
   default        :  yURG_msg ('-', "name (%c/%c) is fully qualified, not hidden/temp, and uses only legal characters", x_style, c_naming);  break;
   }
   ySCORE_mark (myAUDIT.m_yscore, "NSTYLE"  , x_style);
   /*---(save-back)----------------------*/
   if (r_style != NULL)  *r_style = x_style;
   if (r_full  != NULL)  strlcpy (r_full , x_full, LEN_PATH);
   /*---(complete)-----------------------*/
   DEBUG_YENV   yLOG_exit    (__FUNCTION__);
   return RC_POSITIVE;
}

char
yaudit_dots             (char c_naming, char a_file [LEN_PATH], char a_prefix [LEN_TERSE], char a_suffix [LEN_TERSE], short *r_beg, short *r_end)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_min       =    0;
   int         i           =    0;
   int         c           =    0;
   int         l           =    0;
   char        x_msg       [LEN_HUND]  = "";
   int         x_beg       =   -1;
   int         x_end       =   -1;
   /*---(header)-------------------------*/
   DEBUG_YENV   yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_beg != NULL)  *r_beg = -1;
   if (r_end != NULL)  *r_end = -1;
   /*---(defense)------------------------*/
   DEBUG_YENV   yLOG_point   ("a_prefix"  , a_prefix);
   --rce;  if (a_prefix == NULL) {
      ySCORE_mark (myAUDIT.m_yscore, "NPREFIX" , '°');
      yaudit_fatal     ("NAME"    , "prefix standard can not be null (blatant error)");
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YENV   yLOG_info    ("a_prefix"  , a_prefix);
   DEBUG_YENV   yLOG_point   ("a_suffix"  , a_suffix);
   --rce;  if (a_suffix == NULL) {
      ySCORE_mark (myAUDIT.m_yscore, "NSUFFIX" , '°');
      yaudit_fatal     ("NAME"    , "suffix standard can not be null (blatant error)");
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YENV   yLOG_info    ("a_suffix"  , a_suffix);
   /*---(initial score)------------------*/
   ySCORE_mark (myAUDIT.m_yscore, "NDOTS"   , '°');
   /*---(figure minimum)-----------------*/
   if (strcmp (a_prefix, "") != 0)    ++x_min;
   if (strcmp (a_suffix, "") != 0)    ++x_min;
   if (c_naming == 'c' && x_min > 0)  x_min -= 1;
   DEBUG_YENV   yLOG_value   ("x_min"     , x_min);
   /*---(check request)------------------*/
   if (x_min == 0) {
      ySCORE_mark (myAUDIT.m_yscore, "NDOTS"   , '·');
      DEBUG_YENV   yLOG_note    ("no prefix/suffix provided, nothing to check");
      DEBUG_YENV   yLOG_exit    (__FUNCTION__);
      return RC_ACK;
   }
   /*---(count separators)---------------*/
   l = strlen (a_file);
   DEBUG_YENV   yLOG_value   ("l"         , l);
   for (i = 0; i < l; ++i) {
      if (a_file [i] == '.') {
         if (x_beg < 0)  x_beg = i;
         x_end = i;
         ++c;
      }
   }
   if (strcmp (a_prefix, "") == 0)  x_beg = -1;
   if (strcmp (a_suffix, "") == 0)  x_end = -1;
   DEBUG_YENV   yLOG_complex ("counts"    , "%3db, %3de, %3d#", x_beg, x_end, c);
   /*---(check seperators)---------------*/
   --rce;  if (x_min > c) {
      if      (x_min == 2)                     sprintf (x_msg, "need both prefix and suffix, i.e., at least 2 dot (.) separators, but only provided %d (illegal name)", c);
      else if (x_min == 1 && c_naming == 'c')  sprintf (x_msg, "need both prefix and suffix, i.e., at least 1 dot (.) separator, but only provided %d (illegal name)", c);
      else if (strcmp (a_prefix, "") != 0)     sprintf (x_msg, "need prefix, i.e. at least 1 dot (.) separator, but provided none (illegal name)");
      else                                     sprintf (x_msg, "need suffix, i.e. at least 1 dot (.) separator, but provided none (illegal name)");
      ySCORE_mark (myAUDIT.m_yscore, "NDOTS"   , '#');
      yaudit_fatal     ("NAME"    , x_msg);
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(scoring)------------------------*/
   ySCORE_mark (myAUDIT.m_yscore, "NDOTS"   , 'n');
   /*---(save-back)----------------------*/
   if (r_beg != NULL)  *r_beg = x_beg;
   if (r_end != NULL)  *r_end = x_end;
   /*---(complete)-----------------------*/
   DEBUG_YENV   yLOG_exit    (__FUNCTION__);
   return RC_POSITIVE;
}




