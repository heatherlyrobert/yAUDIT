/*===[[ START ]]==============================================================*/
#include    "yAUDIT.h"
#include    "yAUDIT_priv.h"



char ySCORE_nocheck    (void) { return ySCORE_mask (myAUDIT.m_yscore, "PRá "      , "FIXES"     ); }
char ySCORE_nohacked   (void) { return ySCORE_mask (myAUDIT.m_yscore, "HKá "      , "HACKED"    ); }



const tSCORE_TABLE myAUDIT_table [LEN_FULL] = {

   /*===[[ NAME ]]==========================================================*/
   { "NMá "     , ' ' ,  0  , "NAME"     , ""                                 , ""                    , ""                                                                                },
   /*===[[ REQUEST TYPE ]]==================================================*/
   { "NCONF"    , '·' , 'c' , "conf"     , "naming²standard²applied"          , "·°-nslc"             , "°) illegal   -) wild      n) normal    s) standard  l) local     c) central"     }, 
   { "NDIR"     , '·' , 'd' , "dir"      , "directory²requested"              , "·°£/D"               , "°) null      £) empty     /) pathless  D) passed"                                },
   { "NFILE"    , '·' , 'f' , "file"     , "file²name²requested"              , "·°£/*F-"             , "°) null      £) empty     /) pathed    F) passed    -) skipped"                  },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   /*===[[ NAMING ]]========================================================*/
   { "NFULL"    , '·' , 'n' , "full"     , "name²quality²checking"            , "·°/´¢n"              , "°) null      /) not abs   ´) hidden    ¢) bad char  n) passed"                   },
   { "NSTYLE"   , '·' , 'b' , "style"    , "naming²style"                     , "·°dfb"               , "d) dir       f) file      b) both"                                               },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   /*===[[ STANDARDS ]]=====================================================*/
   { "NDOTS"    , '·' , 'n' , "ndots"    , "file²name²separators"             , "·°<>#n"              , "·) skipped   °) none      <) too few   >) too many  n) passed"                   },
   { "NPREFIX"  , '·' , 'p' , "prefix"   , "required²prefix"                  , "·°up"                , "·) skipped   °) failed    u) user      p) passed"                                },
   { "NSUFFIX"  , '·' , 's' , "suffix"   , "required²suffix"                  , "·°s"                 , "·) skipped   °) failed    s) passed"                                             },
   { "NDESC"    , '·' , 'd' , "desc"     , "remaining²description"            , "·°d"                 , "·) skipped   °) failed    d) passed"                                             },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   /*===[[ LOCATION ]]======================================================*/
   { "NLOC"     , '·' , 'h' , "loc"      , "file²location²type"               , "·°rhesI"             , "·) skipped   °) failed    r) root      h) home      e) etc       s) spool"       },
   { "NUSE"     , '·' , 'H' , "usable"   , "file²location²usable"             , "·°RHES"              , "·) skipped   °) failed    R) root      H) home      E) etc       S) spool"       },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   /*===[[ REQUEST ]]=======================================================*/
   { "NAME"     , '-' , '´' , "ŒŒŒŒŒŒ"   , "[[²name²judgement²]]"             , "-°´"                 , "-) unknown   °) failed    ´) passed"                                             },
   { "   "      , ' ' ,  3  , ""         , ""                                 , ""                    , ""                                                                                },

   /*===[[ REQUEST ]]=======================================================*/
   { "EXá "     , ' ' ,  0  , "EXPECT"   , ""                                 , ""                    , ""                                                                                },
   /*===[[ REQUEST TYPE ]]==================================================*/
   { "ETYPE"    , '·' , 'r' , "type"     , "expected file type"               , "·°-drshcbpi?"        , "d) dir   r) reg   s) sym   h) hard  c) char  b) block p) pipe  i) ipsoc ?) WTF"  },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   /*===[[ REQUEST CHARS ]]=================================================*/
   { "EOWNER"   , '·' , 'o' , "owner"    , "expected owner"                   , "·°oöO-"              , "o) standard   ö) numeric    O) default    -) skipped"                            },
   { "EGROUP"   , '·' , 'g' , "group"    , "expected group"                   , "·°gêG-"              , "g) standard   ê) numeric    G) default    -) skipped"                            },
   { "EPERMS"   , '·' , 'p' , "perms"    , "expected permissions"             , "·°p÷P-"              , "p) standard   ÷) non-std    P) default    -) skipped"                            },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   /*===[[ REQUEST DEVICE ]]================================================*/
   { "EMAJOR"   , '·' , 'j' , "major"    , "expected owner"                   , "·°j-"                , "j) in-range   -) skipped"                                                        },
   { "EMINOR"   , '·' , 'n' , "minor"    , "expected group"                   , "·°n-"                , "n) in-range   -) skipped"                                                        },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   /*===[[ REQUEST LINK ]]==================================================*/
   { "ETTYPE"   , '·' , 'r' , "ttype"    , "expected link target type"        , "·°-drshcbpi?"        , "d) dir   r) reg   s) sym   h) hard  c) char  b) block p) pipe  i) ipsoc ?) WTF"  },
   { "ETARGET"  , '·' , 't' , "target"   , "expected link target"             , "·°/#t-"              , "/) not-abs    #) bad chars  t) accepted   -) skipped"                            },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   /*===[[ REQUEST EXTENDED ]]==============================================*/
   { "EEPOCH"   , '·' , 'e' , "epoch"    , "expected update time"             , "·°e-"                , "e) accepted   -) skipped"                                                        },
   { "EBYTES"   , '·' , 'b' , "bytes"    , "expected size in bytes"           , "·°b-"                , "b) accepted   -) skipped"                                                        },
   { "EINODE"   , '·' , 'i' , "inode"    , "expected inode"                   , "·°i-"                , "i) accepted   -) skipped"                                                        },
   { "EHASH"    , '·' , 'h' , "hash"     , "expected file SHA hash"           , "·°#h-"               , "h) accepted   -) skipped"                                                        },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   /*===[[ REQUEST ]]=======================================================*/
   { "EXPECT"   , '-' , '´' , "ŒŒŒŒŒŒ"   , "judgement on request"             , "-°´"                 , "´) passed"                                                                       },
   { "   "      , ' ' ,  3  , ""         , ""                                 , ""                    , ""                                                                                },

   /*===[[ PRECHECK ]]======================================================*/
   { "PRá "     , ' ' ,  0  , "CHECK"    , ""                                 , ""                         , ""                                                                                },
   /*===[[ CHECK TYPE ]]====================================================*/
   { "CTYPE"    , '·' , 'r' , "type"     , "checked file type"                , "·°-drshcbpiDRSHCBPI?", "same as expected type, lower case if matching, upper case if not-match"          },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   /*===[[ CHECK CHARS ]]===================================================*/
   { "COWNER"   , '·' , 'o' , "owner"    , "checked owner"                    , "·°oO"                , "o) match      O) not-match"                                                      },
   { "CGROUP"   , '·' , 'g' , "group"    , "checked group"                    , "·°gG"                , "g) match      G) not-match"                                                      },
   { "CPERMS"   , '·' , 'p' , "perms"    , "checked permissions"              , "·°pP"                , "p) match      P) not-match"                                                      },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   /*===[[ CHECK DEVICE ]]==================================================*/
   { "CMAJOR"   , '·' , 'j' , "major"    , "checked owner"                    , "·°jJ"                , "j) match      J) not-match"                                                      },
   { "CMINOR"   , '·' , 'n' , "minor"    , "checked group"                    , "·°nN"                , "n) match      N) not-match"                                                      },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   /*===[[ CHECK LINK ]]====================================================*/
   { "CTTYPE"   , '·' , 'r' , "ttype"    , "checked link target type"         , "·°-drshcbpi?"        , "t) match      T) not-match"                                                      },
   { "CTARGET"  , '·' , 't' , "target"   , "checked link target"              , "·°tT"                , "t) match      T) not-match"                                                      },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   /*===[[ CHECK JUDGE ]]===================================================*/
   { "CHECK"    , '-' , '´' , "ŒŒŒŒŒŒ"   , "judgement on check"               , "-°Þ´"                , "-) skipped    ´) passed"                                                         },
   { "   "      , ' ' ,  3  , ""         , ""                                 , ""                    , ""                                                                                },

   /*===[[ FIXES ]]=========================================================*/
   { "FXá "     , ' ' ,  0  , "FIXES"    , ""                                 , ""                    , ""                                                                                },
   /*===[[ REQUEST ]]=======================================================*/
   { "EFLAG"    , '·' , 'F' , "flag"     , "fix/forcing flag interpretation"  , "·-°fF!"              , "-) none allowed  f) allow fix     F) allow create  !) can even remove"           },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   /*===[[ CONFIGURATION ]]=================================================*/
   { "CONFC"    , '·' , 'y' , "check"    , "judgement on request"             , "·°-y"                , "y) precheck    -) not-requested"                                                 },
   { "CONFF"    , '·' , 'y' , "force"    , "judgement on request"             , "·°y!-"               , "y) create, if must   !) remove, if must   -) not-requested"                      },
   { "CONFX"    , '·' , 'y' , "fix"      , "judgement on request"             , "·°y-"                , "y) fix, if must      -) not-requested"                                           },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   /*===[[ FIX/FORCES ]]====================================================*/
   { "FDEL"     , '·' , 'r' , "remove"   , "need forced deletion"             , "·°!r-"               , "r) requires deletion   -) not needed"                                            },
   { "FADD"     , '·' , 'c' , "create"   , "need forced creation"             , "·°!c-"               , "c) requires creation   -) not needed"                                            },
   { "FUPD"     , '·' , 'u' , "update"   , "need forced updates"              , "·°!u-"               , "u) requires updates    -) not needed"                                            },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   /*===[[ ACTUAL ]]========================================================*/
   { "REMOVE"   , '·' , 'R' , "REMOVE"   , "actual deletion"                  , "·°R?!-"              , "R) really removed    ?) non-existant      !) not enough force  -) not needed"    },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   { "CREATE"   , '·' , 'C' , "CREATE"   , "actual creation"                  , "·°C?!-"              , "C) really created    ?) already exists    !) not enough force  -) not needed"    },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   { "UPDATE"   , '·' , 'U' , "UPDATE"   , "actual updates"                   , "·°U?!-"              , "U) really updated    !) not enough force  -) not needed"                         },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   /*===[[ RE-CHECK JUDGE ]]================================================*/
   { "FIXES"    , '-' , '´' , "ŒŒŒŒŒŒ"   , "judgement on fixes"               , "-°´"                 , "-) skipped    ´) passed"                                                         },
   { "   "      , ' ' ,  3  , ""         , ""                                 , ""                    , ""                                                                                },

   /*===[[ RECHECK ]]=======================================================*/
   { "REá "     , ' ' ,  0  , "RECHECK"  , ""                                 , ""                    , ""                                                                                },
   /*===[[ RE-CHECK TYPE ]]=================================================*/
   { "RTYPE"    , '·' , 'r' , "type"     , "checked file type"                , "·°-drshcbpi?"        , "same as expected type, lower case if matching, upper case if not-match"          },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   /*===[[ RE-CHECK CHARS ]]================================================*/
   { "ROWNER"   , '·' , 'o' , "owner"    , "re-checked owner"                 , "·°oO"                , "o) match      O) not-match"                                                      },
   { "RGROUP"   , '·' , 'g' , "group"    , "re-checked group"                 , "·°gG"                , "g) match      G) not-match"                                                      },
   { "RPERMS"   , '·' , 'p' , "perms"    , "re-checked permissions"           , "·°pP"                , "p) match      P) not-match"                                                      },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   /*===[[ RE-CHECK DEVICE ]]===============================================*/
   { "RMAJOR"   , '·' , 'j' , "major"    , "re-checked owner"                 , "·°jJ"                , "j) match      J) not-match"                                                      },
   { "RMINOR"   , '·' , 'n' , "minor"    , "re-checked group"                 , "·°nN"                , "n) match      N) not-match"                                                      },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   /*===[[ RE-CHECK LINK ]]=================================================*/
   { "RTTYPE"   , '·' , 'r' , "ttype"    , "re-checked link target type"      , "·°-drshcbpi?"        , "t) match      T) not-match"                                                      },
   { "RTARGET"  , '·' , 't' , "target"   , "re-checked link target"           , "·°tT"                , "t) match      T) not-match"                                                      },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   /*===[[ RE-CHECK JUDGE ]]================================================*/
   { "RECHECK"  , '-' , '´' , "ŒŒŒŒŒŒ"   , "judgement on re-check"            , "-°´"                 , "´) passed"                                                                       },
   { "   "      , ' ' ,  3  , ""         , ""                                 , ""                    , ""                                                                                },

   /*===[[ FINAL ]]=========================================================*/
   { "JDá "     , ' ' ,  0  , "³"        , ""                                 , ""                    , ""                                                                                },
   /*===[[ FINAL JUDGE ]]===================================================*/
   { "FINAL"    , ' ' , 'Ï' , "FINAL"    , "final judgement"                  , " °Ï"                 , "Ï) passed"                                                                       },
   { "   "      , ' ' ,  3  , ""         , ""                                 , ""                    , ""                                                                                },

   /*===[[ HACKED ]]========================================================*/
   { "HKá "     , ' ' ,  0  , "HACK"     , ""                                 , ""                    , ""                                                                                },
   /*===[[ HACKING CHECK ]]=================================================*/
   { "AEPOCH"   , '·' , 'e' , "epoch"    , "check update time"                , "·°eE"                , "e) matches    E) not-match"                                                      },
   { "ABYTES"   , '·' , 'b' , "bytes"    , "check size in bytes"              , "·°bB"                , "b) matches    B) not-match"                                                      },
   { "AINODE"   , '·' , 'i' , "inode"    , "check inode"                      , "·°iI"                , "i) matches    I) not-match"                                                      },
   { "AHASH"    , '·' , 'h' , "hash"     , "check file SHA hash"              , "·°#hH"               , "h) matches    H) not-match"                                                      },
   { " "        , ' ' ,  1  , ""         , ""                                 , ""                    , ""                                                                                },
   /*===[[ HACKING FINAL ]]=================================================*/
   { "HACKED"   , '-' , '´' , "ŒŒŒŒŒŒ"   , "final hacking judgement"          , "-°´"                 , "-) skipped    ´) passed"                                                         },

   /*===[[ END-OF-LIST ]]===================================================*/
   { "end-list" , '·' , '·' , ""         , "end-of-entries"                   , ""                    , ""                                                                                },

   /*===[[ DONE ]]==========================================================*/
};




