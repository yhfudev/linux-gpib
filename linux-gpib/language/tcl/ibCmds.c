#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <tcl.h>
#include <ib.h>
#include <ibP.h>

void ib_CreateVerboseError(Tcl_Interp *interp,char *entry );

int Gpib_Init ( Tcl_Interp *interp ){


extern int gpibCmd _ANSI_ARGS_(( ClientData clientData,
			      Tcl_Interp *interp,
			       int argc,
			       char *argv[]
			       ));

    Tcl_CreateCommand(interp,"gpib",gpibCmd,
		         (ClientData) NULL,
			 (Tcl_CmdDeleteProc *) NULL );

    return TCL_OK;
}


/**********************************************************************/
int ibWrite _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc,char *argv[])){

  if( argc != 3 ){
    Tcl_SetResult(interp, "Error: write <dev> <string> ", TCL_STATIC);
    return TCL_ERROR;
  }

  if( ibwrt( atoi( argv[1]) , argv[2], strlen(argv[2]) ) & ERR ){
    ib_CreateVerboseError(interp,"ibwrt");
    return TCL_ERROR;
  }
     return TCL_OK;

}
/**********************************************************************/
int ibCmd _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc,char *argv[])){

  if( argc != 3 ){
    Tcl_SetResult(interp, "Error: cmd <dev> <string> ", TCL_STATIC);
    return TCL_ERROR;
  }

  if( ibcmd( atoi( argv[1]) , argv[2], strlen(argv[2]) ) & ERR ){
    ib_CreateVerboseError(interp,"ibcmd");
    return TCL_ERROR;
  }
     return TCL_OK;

}
/**********************************************************************/
int ibRead  _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc,char *argv[])){


  char *buf;

  int len;

  if( argc != 3 ){
    Tcl_SetResult(interp, "Error: read <dev> <num>", TCL_STATIC);
    return TCL_ERROR;
  }

  len = atoi(argv[2]);
  

  if ((buf = (char *)malloc( len + 1 )) == NULL) {
    Tcl_SetResult(interp, "Error: Out of Memory", TCL_STATIC);
    return TCL_ERROR;
  }


  if( ibrd( atoi( argv[1]) , buf, len ) & ERR ){
/*  ib_CreateVerboseError(interp,"ibrd");
    return TCL_ERROR;
*/

  Tcl_AppendResult(interp, "ERROR" , (char *) NULL );
  free(buf);

  return TCL_OK;
  }


  buf[ibcnt] = '\0';
  Tcl_AppendResult(interp, buf, (char *) NULL ); 
  free(buf);

  return TCL_OK;
}

/**********************************************************************/
static int Gpib_find_called = 0;

int ibFind  _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc,char *argv[])){

  int dev;
  char res[10];
    char *tmpbuf[120];
    int i;
    extern ibConf_t  ibConfigs[];

  if( argc != 2 ){
    Tcl_SetResult(interp, "Error: ibfind <string>", TCL_STATIC);
    return TCL_ERROR;
  }


  if(( dev = ibfind(argv[1])) & ERR ){
    ib_CreateVerboseError(interp,"ibfind");
    return TCL_ERROR;
  }

  if( ! Gpib_find_called ){

    sprintf((char *)tmpbuf,"%d",ibGetNrDev());
    Tcl_SetVar(interp,"Gpib_Devices",(char *)tmpbuf, TCL_GLOBAL_ONLY);
    sprintf((char *)tmpbuf,"%d",ibGetNrBoards());
    Tcl_SetVar(interp,"Gpib_Boards",(char *)tmpbuf, TCL_GLOBAL_ONLY);
    
    for(i=0; i<ibGetNrDev() ; i++){
        sprintf((char *)tmpbuf,"%d",i );
        Tcl_SetVar2(interp,"ibDevices",(char *)tmpbuf,ibConfigs[i].name,TCL_GLOBAL_ONLY);

        /* names */
        sprintf((char *)tmpbuf,"%d",ibConfigs[i].padsad & 0xff );
        Tcl_SetVar2(interp,ibConfigs[i].name,"pad",(char *)tmpbuf,TCL_GLOBAL_ONLY);
        sprintf((char *)tmpbuf,"%d",ibConfigs[i].padsad>>8 & 0xff );
        Tcl_SetVar2(interp,ibConfigs[i].name,"sad",(char *)tmpbuf,TCL_GLOBAL_ONLY);
        /*flags*/
        sprintf((char *)tmpbuf,"0x%x",ibConfigs[i].flags);
        Tcl_SetVar2(interp,ibConfigs[i].name,"flags",(char *)tmpbuf,TCL_GLOBAL_ONLY);
        sprintf((char *)tmpbuf,"0x%x",ibConfigs[i].eos);
        Tcl_SetVar2(interp,ibConfigs[i].name,"eos",(char *)tmpbuf,TCL_GLOBAL_ONLY);
        sprintf((char *)tmpbuf,"0x%x",ibConfigs[i].eosflags);
        Tcl_SetVar2(interp,ibConfigs[i].name,"eosflags",(char *)tmpbuf,TCL_GLOBAL_ONLY);

        if( ibConfigs[i].networkdb != NULL )
        Tcl_SetVar2(interp,ibConfigs[i].name,"network",ibConfigs[i].networkdb,TCL_GLOBAL_ONLY);

        Tcl_SetVar2(interp,ibConfigs[i].name,"init",ibConfigs[i].init_string,TCL_GLOBAL_ONLY);

    }
    Gpib_find_called++;
  }

  sprintf(res,"%4d",dev );
  Tcl_SetResult( interp,res, TCL_VOLATILE );

  return TCL_OK;
}
/**********************************************************************/
int ibInfo  _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc,char *argv[])){

  int dev;
  char res[10];
    char *tmpbuf[120];
    int i;
    extern ibConf_t  ibConfigs[];

  for(i=0; i < ibGetNrDev(); i++ ){
    Tcl_AppendResult(interp,ibConfigs[i].name," ",(char *)NULL);
  }
  return TCL_OK;

}

/**********************************************************************/

int ibSre   _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc,char *argv[])){

  if( argc != 3 ){
    Tcl_SetResult(interp, "Error: ibsre <bool> ", TCL_STATIC);
    return TCL_ERROR;
  }

  if( ibsre( atoi( argv[1]) , atoi( argv[2] )) & ERR ){
    ib_CreateVerboseError(interp,"ibsre");
    return TCL_ERROR;
  }
     return TCL_OK;
}
/**********************************************************************/

int ibSic   _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc,char *argv[])){

  if( argc != 2 ){
    Tcl_SetResult(interp, "Error: iclear <dev> ", TCL_STATIC);
    return TCL_ERROR;
  }

  if( ibsic( atoi( argv[1])) & ERR ){
    ib_CreateVerboseError(interp,"ibsic");
    return TCL_ERROR;
  }
     return TCL_OK;

}
/**********************************************************************/
int ibClr   _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc,char *argv[])){

  if( argc != 2 ){
    Tcl_SetResult(interp, "Error: clear <dev> ", TCL_STATIC);
    return TCL_ERROR;
  }

  if( ibclr( atoi( argv[1])) & ERR ){
    ib_CreateVerboseError(interp,"ibclr");
    return TCL_ERROR;
  }
     return TCL_OK;
}
/**********************************************************************/
int ibOnl   _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc,char *argv[])){

  if( argc != 3 ){
    Tcl_SetResult(interp, "Error: onl <dev> <val>", TCL_STATIC);
    return TCL_ERROR;
  }

  if( ibonl( atoi( argv[1]),atoi(argv[2])) & ERR ){
    ib_CreateVerboseError(interp,"ibonl");
    return TCL_ERROR;
  }
     return TCL_OK;
}

/**********************************************************************/
int ibWait   _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc,char *argv[])){

  int mask=0;
  register int i;

  if( argc < 3 ){
    Tcl_SetResult(interp, "Error: wait <dev> <what>", TCL_STATIC);
    return TCL_ERROR;
  }

  for( i=2; i<argc; i++ ){

    if( *argv[i] == 's' && !strcmp(argv[i],"srq"))  mask |= SRQI;
    else if( *argv[i] == 'c' && !strcmp(argv[i],"cmpl")) mask |= CMPL;
    else if( *argv[i] == 'r' && !strcmp(argv[i],"rqs"))  mask |= RQS;
    else if( *argv[i] == 'c' && !strcmp(argv[i],"cic"))  mask |= CIC;
    else if( *argv[i] == 'a' && !strcmp(argv[i],"atn"))  mask |= ATN;
    else if( *argv[i] == 't' && !strcmp(argv[i],"timo")) mask |= TIMO;

    else {
      Tcl_SetResult(interp, "Wait: illegal Argument", TCL_STATIC );
      return TCL_ERROR;
    }

  }

  if( ibwait( atoi( argv[1]),SRQI) & ERR ){
    ib_CreateVerboseError(interp,"ibwait");
    return TCL_ERROR;
  }
     return TCL_OK;
}
/**********************************************************************/
int ibClose  _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc,char *argv[])){


  if( argc < 2){
    Tcl_SetResult(interp, "Error: close <dev> ", TCL_STATIC);
    return TCL_ERROR;
  }

  if( ibonl( atoi( argv[1]),0 ) & ERR ){
    ib_CreateVerboseError(interp,"ibclose");
    return TCL_ERROR;
  }
  return TCL_OK;
}
/**********************************************************************/
/**********************************************************************/
int ibSetDBG  _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc,char *argv[])){


  if( argc < 3){
    Tcl_SetResult(interp, "Error: debug <dev> <level> ", TCL_STATIC);
    return TCL_ERROR;
  }

  if( ibSdbg( atoi( argv[1]), atoi(argv[2]) ) & ERR ){
    ib_CreateVerboseError(interp,"debug");
    return TCL_ERROR;
  }
  return TCL_OK;
}

/**********************************************************************/
int ibRsp  _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc,char *argv[])){


  char spb;
  char spr[5];

  if( argc < 2){
    Tcl_SetResult(interp, "Error: rsp <dev> ", TCL_STATIC);
    return TCL_ERROR;
  }

  if( ibrsp( atoi( argv[1]), &spb ) & ERR ){
    ib_CreateVerboseError(interp,"ibrsp");
    return TCL_ERROR;
  }
  sprintf(spr,"%3d",spb); /* convert serial response to integer string */
  Tcl_AppendResult(interp,spr,(char *)NULL);
  return TCL_OK;
}
/**********************************************************************/
int ibTrg  _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc,char *argv[])){

  if( argc < 2){
    Tcl_SetResult(interp, "Error: trg <dev> ", TCL_STATIC);
    return TCL_ERROR;
  }

  if( ibtrg( atoi( argv[1]) ) & ERR ){
    ib_CreateVerboseError(interp,"ibtrg");
    return TCL_ERROR;
  }
  return TCL_OK;
}
/**********************************************************************/
int ibRsv  _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc,char *argv[])){

  if( argc < 2){
    Tcl_SetResult(interp, "Error: rsv <dev> <val>", TCL_STATIC);
    return TCL_ERROR;
  }

  if( ibrsv( atoi( argv[1]) , (char) atoi( argv[2] ) ) & ERR ){
    ib_CreateVerboseError(interp,"ibrsv");
    return TCL_ERROR;
  }
  return TCL_OK;
}
/**********************************************************************/




static char errbuf[80]; 

void ib_CreateVerboseError(Tcl_Interp *interp,char *entry ){

strcpy(errbuf,entry);

strcat(errbuf,": \nIBSTAT = <");

  if ( ibsta & ERR )  strcat(errbuf," ERR");
  if ( ibsta & TIMO ) strcat(errbuf," | TIMO");
  if ( ibsta & END )  strcat(errbuf," | END"); 
  if ( ibsta & SRQI ) strcat(errbuf," | SRQI");
  if ( ibsta & RQS )  strcat(errbuf," | RQS");
  if ( ibsta & CMPL ) strcat(errbuf," | CMPL");
/*if ( ibsta & LOK )  strcat(errbuf," | LOK");*/
/*if ( ibsta & REM )  strcat(errbuf," | REM");*/ 
  if ( ibsta & CIC )  strcat(errbuf," | CIC"); 
  if ( ibsta & ATN )  strcat(errbuf," | ATM");
  if ( ibsta & TACS ) strcat(errbuf," | TACS");
  if ( ibsta & LACS ) strcat(errbuf," | LACS");
/*if ( ibsta & DTAS ) strcat(errbuf," | DATS");*/
/*if ( ibsta & DCAS ) strcat(errbuf," | DCTS");*/

strcat(errbuf,"> \nIBERR = ");

if ( iberr == EDVR) strcat(errbuf," EDVR <OS Error>");
if ( iberr == ECIC) strcat(errbuf," ECIC <Not CIC>");
if ( iberr == ENOL) strcat(errbuf," ENOL <No Listener>");
if ( iberr == EADR) strcat(errbuf," EADR <Adress Error>");
if ( iberr == EARG) strcat(errbuf," ECIC <Invalid Argument>");
if ( iberr == ESAC) strcat(errbuf," ESAC <No Sys Ctrlr>");
if ( iberr == EABO) strcat(errbuf," EABO <Operation Aborted>");
if ( iberr == ENEB) strcat(errbuf," ENEB <No Gpib Board>");
if ( iberr == EOIP) strcat(errbuf," EOIP <Async I/O in prg>");
if ( iberr == ECAP) strcat(errbuf," ECAP <No Capability>");
if ( iberr == EFSO) strcat(errbuf," EFSO <File sys. error>");
if ( iberr == EBUS) strcat(errbuf," EBUS <Command error>");
if ( iberr == ESTB) strcat(errbuf," ESTB <Status byte lost>");
if ( iberr == ESRQ) strcat(errbuf," ESRQ <SRQ stuck on>");
if ( iberr == EPAR) strcat(errbuf," EPAR <Parse Error in Config>");
if ( iberr == ECFG) strcat(errbuf," ECFG <Can't open Config>");
if ( iberr == ETAB) strcat(errbuf," ETAB <Device Table Overflow>");
if ( iberr == ENSD) strcat(errbuf," ENSD <Device not Found>");



Tcl_AppendResult(interp, errbuf , (char *) NULL );

}







/**********************************************************************/


int gpibCmd _ANSI_ARGS_(( ClientData clientData,
			      Tcl_Interp *interp,
			       int argc,
			       char *argv[]
			       )) 
{

char result[100];

if( *argv[1]=='f' && !strcmp(argv[1],"find")){
  return ibFind( clientData, interp, argc-1,argv+1 );
}
if( *argv[1]=='r' && !strcmp(argv[1],"read")){
  return ibRead( clientData, interp, argc-1,argv+1 );
}
if( *argv[1]=='w' && !strcmp(argv[1],"write")){
  return ibWrite( clientData, interp, argc-1,argv+1 );
}
if( *argv[1]=='o' && !strcmp(argv[1],"online")){
  return ibOnl( clientData, interp, argc-1,argv+1 );
}
if( *argv[1]=='c' && !strcmp(argv[1],"clear")){
  return ibClr( clientData, interp, argc-1,argv+1 );
}
if( *argv[1]=='r' && !strcmp(argv[1],"ren")){
  return ibSre( clientData, interp, argc-1,argv+1 );
}
if( *argv[1]=='s' && !strcmp(argv[1],"sic")){
  return ibSic( clientData, interp, argc-1,argv+1 );
}
if( *argv[1]=='c' && !strcmp(argv[1],"cmd")){
  return ibCmd( clientData, interp, argc-1,argv+1 );
}
if( *argv[1]=='w' && !strcmp(argv[1],"wait")){
  return ibWait( clientData, interp, argc-1,argv+1 );
}
if( *argv[1]=='c' && !strcmp(argv[1],"close")){
  return ibClose( clientData, interp, argc-1,argv+1 );
}
if( *argv[1]=='r' && !strcmp(argv[1],"rsp")){
  return ibRsp( clientData, interp, argc-1,argv+1 );
}
if( *argv[1]=='r' && !strcmp(argv[1],"rsv")){
  return ibRsp( clientData, interp, argc-1,argv+1 );
}
if( *argv[1]=='t' && !strcmp(argv[1],"trg")){
  return ibTrg( clientData, interp, argc-1,argv+1 );
}
if( *argv[1]=='i' && !strcmp(argv[1],"info")){
  return ibInfo( clientData, interp, argc-1,argv+1 );
}
if( *argv[1]=='d' && !strcmp(argv[1],"debug")){
  return ibSetDBG( clientData, interp, argc-1,argv+1 );
}


Tcl_SetResult(interp,"Error: gpib without command",TCL_STATIC);
return TCL_ERROR;
}


























