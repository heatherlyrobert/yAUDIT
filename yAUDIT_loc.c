/*===[[ START ]]==============================================================*/
#include    "yAUDIT.h"
#include    "yAUDIT_priv.h"



char
yaudit__local           (int a_ruid, char a_ruser [LEN_USER], char a_full [LEN_PATH], char b_owner [LEN_USER], char b_group [LEN_USER], char b_perms [LEN_TERSE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_root      [LEN_LABEL] = "";
   char        x_home      [LEN_LABEL] = "";
   char        t           [LEN_PATH]  = "";
   char       *p           = NULL;
   int         x_fuid      =   -1;
   char        x_msg       [LEN_HUND]  = "";
   /*---(header)-------------------------*/
   DEBUG_YENV   yLOG_enter   (__FUNCTION__);
   /*---(scoring)------------------------*/
   ySCORE_mark ("NLOC"    , '°');
   /*---(set location)-------------------*/
   if (strncmp (a_full, "/tmp", 4) == 0) {
      strcpy (x_root , "/tmp/root/");
      strcpy (x_home , "/tmp/home/");
   } else {
      strcpy (x_root , "/root/");
      strcpy (x_home , "/home/");
   }
   DEBUG_YENV   yLOG_info    ("x_root"    , x_root);
   DEBUG_YENV   yLOG_info    ("x_home"    , x_home);
   /*---(check root)---------------------*/
   if (strncmp (a_full, x_root, strlen (x_root)) == 0) {
      ySCORE_mark ("NLOC"    , 'r');
      ySCORE_mark ("NUSE"    , '°');
      DEBUG_YENV   yLOG_note    ("file located under root");
      if (a_ruid != 0) {
         sprintf (x_msg, "running as å%sæ, uid (%d); BUT, file in å%sæ (illegal)", a_ruser, a_ruid, x_root);
         yaudit_fatal     ("NAME"    , x_msg);
         DEBUG_YENV    yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      yURG_msg ('-', "good, running as å%sæ, uid (%d) and file is located in or below å%sæ", a_ruser, a_ruid, x_root);
      ySCORE_mark ("NUSE"    , 'R');
      strcpy (t, "root");
      /*---(done)------------------------*/
   }
   /*---(check home)---------------------*/
   else if (strncmp (a_full, x_home, strlen (x_home)) == 0) {
      ySCORE_mark ("NLOC"    , 'h');
      strcpy (t, a_full + strlen (x_home));
      ySCORE_mark ("NUSE"    , '°');
      DEBUG_YENV   yLOG_info    ("t"         , t);
      p = strchr (t, '/');
      DEBUG_YENV   yLOG_point   ("p"         , p);
      if (p != NULL)  p [0] = '\0';
      else            strcpy (t, "");
      DEBUG_YENV   yLOG_info    ("t"         , t);
      rc = yENV_user_data ('n', t, &x_fuid, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
      if (rc < 0) {
         sprintf (x_msg, "could not find å%sæ as a legal user subdirectory under å%sæ (illegal)", t, x_home);
         yaudit_fatal     ("NAME"    , x_msg);
         DEBUG_YENV    yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      if      (a_ruid == 0) ;
      else if (strcmp (a_ruser, t) == 0) ;
      else {
         sprintf (x_msg, "running as å%sæ, uid (%d); BUT, file in å%sæ (illegal)", a_ruser, a_ruid, t);
         yaudit_fatal     ("NAME"    , x_msg);
         DEBUG_YENV    yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      yURG_msg ('-', "good, running as å%sæ, uid (%d) and file is located in or below å%sæ", a_ruser, a_ruid, t);
      ySCORE_mark ("NUSE"    , 'H');
      /*---(done)------------------------*/
   }
   /*---(elsewhere)----------------------*/
   else {
      sprintf (x_msg, "file not in å%sæ or under å%sæ and so can not be handled", x_root, x_home);
      yaudit_fatal     ("NAME"    , x_msg);
      DEBUG_YENV    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)----------------------*/
   if (b_owner != NULL)   strlcpy (b_owner, t        , LEN_USER);
   if (b_group != NULL)   strlcpy (b_group, t        , LEN_USER);
   if (b_perms != NULL)   strlcpy (b_perms, "f_tight", LEN_TERSE);
   /*---(complete)-----------------------*/
   DEBUG_YENV   yLOG_exit    (__FUNCTION__);
   return RC_POSITIVE;
}

char
yaudit__central         (int a_ruid, char a_ruser [LEN_USER], char a_full [LEN_PATH], char b_owner [LEN_USER], char b_group [LEN_USER], char b_perms [LEN_TERSE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_etc       [LEN_LABEL] = "";
   char        x_spool     [LEN_LABEL] = "";
   char        t           [LEN_PATH]  = "";
   char       *p           = NULL;
   char        x_msg       [LEN_HUND]  = "";
   /*---(header)-------------------------*/
   DEBUG_YENV   yLOG_enter   (__FUNCTION__);
   /*---(scoring)------------------------*/
   ySCORE_mark ("NLOC"    , '°');
   /*---(set location)-------------------*/
   if (strncmp (a_full, "/tmp", 4) == 0) {
      strcpy (x_etc  , "/tmp/etc/");
      strcpy (x_spool, "/tmp/spool/");
   } else {
      strcpy (x_etc  , "/etc/");
      strcpy (x_spool, "/var/spool/");
   }
   DEBUG_YENV   yLOG_info    ("x_spool"   , x_spool);
   DEBUG_YENV   yLOG_info    ("x_etc"     , x_etc);
   /*---(check etc)----------------------*/
   if (strncmp (a_full, x_etc, strlen (x_etc)) == 0) {
      ySCORE_mark ("NLOC"    , 'e');
      ySCORE_mark ("NUSE"    , '°');
      DEBUG_YENV   yLOG_note    ("file located under root");
      if (a_ruid != 0) {
         sprintf (x_msg, "running as å%sæ, uid (%d); BUT, file in å%sæ (illegal)", a_ruser, a_ruid, x_etc);
         yaudit_fatal     ("NAME"    , x_msg);
         DEBUG_YENV    yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      yURG_msg ('-', "good, running as å%sæ, uid (%d) and file is located in or below å%sæ", a_ruser, a_ruid, x_etc);
      ySCORE_mark ("NUSE"    , 'E');
   }
   /*---(check spool)--------------------*/
   else if (strncmp (a_full, x_spool, strlen (x_spool)) == 0) {
      ySCORE_mark ("NLOC"    , 's');
      ySCORE_mark ("NUSE"    , '°');
      p = strrchr (a_full, '/');
      DEBUG_YENV   yLOG_point   ("p"         , p);
      if (p != NULL)  strcpy (t, p + 1);
      else            strcpy (t, "");
      DEBUG_YENV   yLOG_info    ("t"         , t);
      p = strchr  (t, '.');
      if (p != NULL)  p [0] = '\0';
      else            strcpy (t, "");
      DEBUG_YENV   yLOG_info    ("t"         , t);
      rc = yENV_user_data ('n', t, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
      if (rc < 0) {
         sprintf (x_msg, "could not find å%sæ as a legal user (illegal)", t, x_spool);
         yaudit_fatal     ("NAME"    , x_msg);
         DEBUG_YENV    yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      if (a_ruid == 0 || strcmp (a_ruser, t) == 0) {
         yURG_msg ('-', "good, running as å%sæ, uid (%d) so can handle file prefixed as å%sæ", a_ruser, a_ruid, t);
      } else {
         sprintf (x_msg, "running as å%sæ, uid (%d); BUT, file prefixed as å%sæ (illegal)", a_ruser, a_ruid, t);
         yaudit_fatal     ("NAME"    , x_msg);
         DEBUG_YENV    yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      ySCORE_mark ("NUSE"    , 'S');
   }
   /*---(elsewhere)----------------------*/
   else {
      sprintf (x_msg, "file not in å%sæ or under å%sæ and so can not be handled", x_etc, x_spool);
      yaudit_fatal     ("NAME"    , x_msg);
      DEBUG_YENV    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)----------------------*/
   if (b_owner != NULL)   strlcpy (b_owner, "root"   , LEN_USER);
   if (b_group != NULL)   strlcpy (b_group, "root"   , LEN_USER);
   if (b_perms != NULL)   strlcpy (b_perms, "f_tight", LEN_TERSE);
   /*---(complete)-----------------------*/
   DEBUG_YENV   yLOG_exit    (__FUNCTION__);
   return RC_POSITIVE;
}

char
yaudit_name_location    (char a_type, char c_naming, char a_full [LEN_PATH], char b_owner [LEN_USER], char b_group [LEN_USER], char b_perms [LEN_TERSE])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_ruid      =   -1;
   char        x_ruser     [LEN_USER]  = "";
   /*---(header)-------------------------*/
   DEBUG_YENV   yLOG_enter   (__FUNCTION__);
   /*---(config mark(--------------------*/
   --rce;  if (c_naming == 0 || strchr (YENV_NAMING, c_naming) == NULL) {
      DEBUG_YENV   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(quick-out)----------------------*/
   DEBUG_YENV   yLOG_char    ("a_type"    , a_type);
   if (a_type   != YENV_REG) {
      DEBUG_YENV   yLOG_note    ("standards only for regular files");
      DEBUG_YENV   yLOG_exit    (__FUNCTION__);
      return RC_ACK;
   }
   DEBUG_YENV   yLOG_char    ("c_naming"  , c_naming);
   if (c_naming != 0 && strchr ("cl", c_naming) == NULL) {
      DEBUG_YENV   yLOG_note    ("not using yjods central/local standards, skipping");
      DEBUG_YENV   yLOG_exit    (__FUNCTION__);
      return RC_ACK;
   }
   /*---(set uid)------------------------*/
   ySCORE_mark ("NLOC"    , 'I');
   rc = yENV_whoami (NULL, NULL, &x_ruid, NULL, NULL, x_ruser, NULL, NULL, NULL, NULL);
   DEBUG_YENV   yLOG_value   ("whoami"    , rc);
   --rce;  if (rc < 0) {
      ySCORE_mark ("NLOC"    , '°');
      yaudit_fatal     ("NAME"    , "could not identify current user  (scary)");
      DEBUG_YENV    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YENV   yLOG_info    ("x_ruser"   , x_ruser);
   /*---(handle locals)------------------*/
   if (c_naming == YENV_LOCAL) {
      DEBUG_YENV   yLOG_note    ("handling local");
      rc = yaudit__local       (x_ruid, x_ruser, a_full, b_owner, b_group, b_perms);
      if (rc < 0) {
         DEBUG_YENV    yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(check centrals)-----------------*/
   if (c_naming == YENV_CENTRAL) {
      DEBUG_YENV   yLOG_note    ("handling central");
      rc = yaudit__central     (x_ruid, x_ruser, a_full, b_owner, b_group, b_perms);
      if (rc < 0) {
         DEBUG_YENV    yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_YENV   yLOG_exit    (__FUNCTION__);
   return RC_POSITIVE;
}
