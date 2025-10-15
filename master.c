


char       /*----: set up programgents/debugging -----------------------------*/
yaudit_unit_quiet    (void)
{
   char       *x_args [1]  = { "yAUDIT" };
   ySCORE_init (myAUDIT_table, '-', &(myAUDIT.m_yscore));
   return 0;
}

char       /*----: set up programgents/debugging -----------------------------*/
yaudit_unit_loud     (void)
{
   char       *x_args [3]  = { "yAUDIT_unit","@@kitchen", "@@yaudit" };
   yURG_logger (3, x_args);
   yURG_urgs   (3, x_args);
   ySCORE_init (myAUDIT_table, '-', &(myAUDIT.m_yscore));
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
yaudit_unit_end      (void)
{
   ySCORE_wrap (&(myAUDIT.m_yscore));
   yLOGS_end     ();
   return 0;
}


