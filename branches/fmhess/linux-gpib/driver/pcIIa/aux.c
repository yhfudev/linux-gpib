

#include <board.h>





/* -- bdSendAuxCmd(int cmd)
 * Performs single auxiliary commands (nec7210)
 *
 *
 */

IBLCL int bdSendAuxCmd(int cmd)
{
  DBGin("bdSendAuxCmd");
  switch(cmd){

  case AUX_GTS:
    DBGprint(DBG_BRANCH,("Aux Send GTS"));
    GPIBout(auxmr,AUX_GTS); 
    break;
  case AUX_TCS:
    DBGprint(DBG_BRANCH,("Aux Send TCS"));
    GPIBout(auxmr,AUX_TCS);
    break;
  case AUX_TCA:
    DBGprint(DBG_BRANCH,("Aux Send TCA"));
    GPIBout(auxmr,AUX_TCA);
    break;
  case AUX_EPP:
    DBGprint(DBG_BRANCH,("Aux Send EPP"));
    GPIBout(auxmr,AUX_EPP);
    break;
  case AUX_SIFC:
    DBGprint(DBG_BRANCH,("Aux Send IFC"));
    GPIBout(auxmr,AUX_SIFC); 
    break;
  case AUX_SREN:
    DBGprint(DBG_BRANCH,("Aux Send REM"));
    GPIBout(auxmr,AUX_SREN);
    break;
  case AUX_CREN:
    DBGprint(DBG_BRANCH,("Aux Send unset REM"));
    GPIBout(auxmr,AUX_CREN); 
    break;
  case AUX_CIFC:
    DBGprint(DBG_BRANCH,("Aux Send unset IFC"));
    GPIBout(auxmr,AUX_CIFC); 
    break;
  case AUX_FH:
    DBGprint(DBG_BRANCH,("Aux Send FH"));
    GPIBout(auxmr,AUX_FH);
    break;
  case AUX_SEOI:
    if( pgmstat & PS_NOEOI){ 
      DBGprint(DBG_BRANCH,("Aux Send SEOI disabled"));
    } else {
      DBGprint(DBG_BRANCH,("Aux Send EOI"));
      GPIBout(auxmr, AUX_SEOI);
    }
    break;  
  default:
    DBGprint(DBG_BRANCH,(" warning: illegal auxiliary command"));
    GPIBout(auxmr,cmd);
    break;
  }
  bdlines();
  DBGout();
}



/* -- bdSendAuxACmd(int cmd)
 * Set Auxiliary A Register
 *
 *
 */

IBLCL int bdSendAuxACmd(int cmd)
{
  DBGin("bdSendAuxACmd");
  DBGprint(DBG_BRANCH,("Aux Send=0x%x",cmd));

  GPIBout(auxmr, auxrabits | cmd);
  DBGout();
}


