
#include "ib_internal.h"
#include <ibP.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"

ibConf_t *ibConfigs[NUM_CONFIGS];

int ibParseConfigFile(char *filename)
{
	extern FILE *gpib_yyin;
	FILE *infile;
	int stat = 0;

	if ((infile = fopen(filename, "r")) == NULL)
	{
		fprintf(stderr, "failed to open configuration file\n");
		iberr = ECFG;
		return -1;
	}

	/*ibPutMsg("Parse Config %s",filename); */
	gpib_yyin = infile;
	gpib_yyrestart(gpib_yyin); /* Hmm ? */

	// initialize line counter to 1 before calling yyparse
	yylloc.first_line = 1;
	ibBoardDefaultValues();
	// initialize variables used in yyparse() - see ibConfYacc.y
	findIndex = 0;
	if(gpib_yyparse() < 0)
	{
		fprintf(stderr, "failed to parse configuration file\n");
		stat = -1 ;
	}
	fclose(infile);

	return stat;
}

/**********************************************************************/

int ibGetDescriptor(ibConf_t p)
{
	ibConf_t *conf;
	int ib_ndev;

	/* check validity of values */
	if( p.pad >= IB_MAXDEV )
	{
		iberr = ETAB;
		return -1;
	}
	// search for an unused descriptor
	for(ib_ndev = 0; ib_ndev < NUM_CONFIGS; ib_ndev++)
	{
		if(ibConfigs[ib_ndev] == NULL)
		{
			ibConfigs[ib_ndev] = malloc(sizeof(ibConf_t));
			break;
		}
	}
	if( ib_ndev == NUM_CONFIGS)
	{
		iberr = ETAB;
		return -1;
	}
	conf = ibConfigs[ib_ndev];
	/* put entry to the table */
	strncpy(conf->name, p.name, sizeof(conf->name) );
	conf->board = p.board;
	conf->pad = p.pad;
	conf->sad = p.sad;
	conf->flags = p.flags;
	conf->eos = p.eos;
	conf->eosflags = p.eosflags;
	conf->tmo = p.tmo;

	strncpy(conf->init_string, p.init_string, sizeof(conf->init_string));
	return ib_ndev;
}

/**********************************************************************/
int ibFindDevIndex(char *name)
{
	int i;

	for(i = 0; i < FIND_CONFIGS_LENGTH; i++)
	{
		if(!strcmp(ibFindConfigs[i].name, name)) return i;
	}

	return -1;
}

int ibCheckDescriptor(int ud)
{
	if(ud < 0 || ud > NUM_CONFIGS || ibConfigs[ud] == NULL)
		return -1;
	return 0;
}

ssize_t device_command(ibBoard_t *board, uint8_t *cmd, size_t count, int pad, int sad)
{
	if(send_setup(board, pad, sad) < 0)
	{
		return -1;
	}

	return __ibcmd(board, cmd, count);
}