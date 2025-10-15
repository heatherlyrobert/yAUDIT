/*===[[ START ]]==============================================================*/
#include    "yAUDIT.h"
#include    "yAUDIT_priv.h"



tyAUDIT  myAUDIT;



/*====================------------------------------------====================*/
/*===----                            program                           ----===*/
/*====================------------------------------------====================*/
void  o___PROGRAM_________o () { return; }

static char s_version    [LEN_HUND];

char*        /*--> return versioning information ---------[ ------ [ ------ ]-*/
yAUDIT_version     (void)
{
   char        t [LEN_LABEL] = "";
#if    __TINYC__ > 0
   strncpy (t, "[tcc built  ]", LEN_LABEL);
#elif  __GNUC__  > 0
   strncpy (t, "[gnu gcc    ]", LEN_LABEL);
#elif  __HEPH__  > 0
   strncpy (t, "[hephaestus ]", LEN_LABEL);
#else
   strncpy (t, "[unknown    ]", LEN_LABEL);
#endif
   snprintf (s_version, LEN_HUND, "%s   %s : %s", t, P_VERNUM, P_VERTXT);
   return s_version;
}



