%{
#include <stdio.h>
#include "ib_internal.h"
#undef EXTERN
#include "ibP.h"
#include <string.h>
#include <stdlib.h>
#include "ibConfYacc.h"

#define YYERROR_VERBOSE

YY_DECL;

ibConf_t ibFindConfigs[FIND_CONFIGS_LENGTH];
unsigned int findIndex;
int bdid;

%}

%pure_parser

%union
{
int  ival;
char *sval;
char bval;
char cval;
}

%token T_INTERFACE T_DEVICE T_NAME T_MINOR T_BASE T_IRQ T_DMA
%token T_PAD T_SAD T_TIMO T_EOSBYTE T_BOARD_TYPE T_PCI_BUS T_PCI_SLOT
%token T_REOS T_BIN T_INIT_S T_DCL T_XEOS T_EOT
%token T_MASTER T_LLO T_DCL T_EXCL T_INIT_F T_AUTOPOLL

%token T_NUMBER T_STRING T_BOOL T_TIVAL
%type <ival> T_NUMBER
%type <ival> T_TIVAL
%type <sval> T_STRING
%type <bval> T_BOOL

%%

	input: /* empty */
		| device input
		| interface input
		| error
			{
				fprintf(stderr, "input error on line %i of %s\n", @1.first_line, DEFAULT_CONFIG_FILE);
				return -1;
			}
		;

	interface: T_INTERFACE '{' minor parameter '}'
			{
				ibFindConfigs[findIndex].is_interface = 1;
				if(++findIndex >= FIND_CONFIGS_LENGTH)
				{
					fprintf(stderr, " too many devices in config file\n");
					return -1;
				}
			}
		;

	minor : T_MINOR '=' T_NUMBER {
				bdid = $3; ibFindConfigs[findIndex].defaults.board = $3;
				if(bdid < MAX_BOARDS)
					snprintf(ibBoard[bdid].device, sizeof(ibBoard[bdid].device), "/dev/gpib%i", bdid);
				else
					return -1;
			}
		;

	parameter: /* empty */
		| statement parameter
		| error
			{
				fprintf(stderr, "parameter error on line %i of %s\n", @1.first_line, DEFAULT_CONFIG_FILE);
				return -1;
			}
		;

	statement: T_PAD '=' T_NUMBER      { ibBoard[bdid].pad =  $3; ibFindConfigs[findIndex].defaults.pad = $3;}
		| T_SAD '=' T_NUMBER      { ibBoard[bdid].sad = $3 - sad_offset; ibFindConfigs[findIndex].defaults.sad = $3 - sad_offset;}
		| T_EOSBYTE '=' T_NUMBER  { ibFindConfigs[findIndex].defaults.eos = $3;}
		| T_REOS T_BOOL           { ibFindConfigs[findIndex].defaults.eos_flags |= $2 * REOS;}
		| T_BIN  T_BOOL           { ibFindConfigs[findIndex].defaults.eos_flags |= $2 * BIN;}
		| T_REOS '=' T_BOOL           { ibFindConfigs[findIndex].defaults.eos_flags |= $3 * REOS;}
		| T_XEOS '=' T_BOOL           { ibFindConfigs[findIndex].defaults.eos_flags |= $3 * XEOS;}
		| T_BIN '=' T_BOOL           { ibFindConfigs[findIndex].defaults.eos_flags |= $3 * BIN;}
		| T_EOT '=' T_BOOL           { ibFindConfigs[findIndex].defaults.send_eoi = $3;}
		| T_TIMO '=' T_TIVAL      { ibFindConfigs[findIndex].defaults.usec_timeout = $3; }
		| T_TIMO '=' T_NUMBER      { ibFindConfigs[findIndex].defaults.usec_timeout = timeout_to_usec( $3 ); }
		| T_BASE '=' T_NUMBER     { ibBoard[bdid].base = $3; }
		| T_IRQ  '=' T_NUMBER     { ibBoard[bdid].irq = $3; }
		| T_DMA  '=' T_NUMBER     { ibBoard[bdid].dma = $3; }
		| T_PCI_BUS  '=' T_NUMBER     { ibBoard[bdid].pci_bus = $3; }
		| T_PCI_SLOT  '=' T_NUMBER     { ibBoard[bdid].pci_slot = $3; }
		| T_MASTER T_BOOL	{ ibBoard[bdid].is_system_controller = $2; }
		| T_MASTER '=' T_BOOL	{ ibBoard[bdid].is_system_controller = $3; }
		| T_BOARD_TYPE '=' T_STRING
			{
				strncpy(ibBoard[bdid].board_type, $3,
					sizeof(ibBoard[bdid].board_type));
			}
		| T_NAME '=' T_STRING
			{
				strncpy(ibFindConfigs[findIndex].name, $3,
					sizeof(ibFindConfigs[findIndex].name));
			}
		;

	device: T_DEVICE '{' option '}'
			{
				ibFindConfigs[findIndex].is_interface = 0;
				if(++findIndex >= FIND_CONFIGS_LENGTH)
				{
					fprintf(stderr, "too many devices in config file\n");
					return -1;
				}
			}
		;

	option: /* empty */
		| assign option
		| error
 			{
 				fprintf(stderr, "option error on line %i of %s\n", @1.first_line, DEFAULT_CONFIG_FILE);
				return -1;
			}
		;

	assign:
		T_PAD '=' T_NUMBER { ibFindConfigs[findIndex].defaults.pad = $3; }
		| T_SAD '=' T_NUMBER { ibFindConfigs[findIndex].defaults.sad = $3 - sad_offset; }
		| T_INIT_S '=' T_STRING { strncpy(ibFindConfigs[findIndex].init_string,$3,60); }
		| T_EOSBYTE '=' T_NUMBER  { ibFindConfigs[findIndex].defaults.eos = $3; }
		| T_REOS T_BOOL           { ibFindConfigs[findIndex].defaults.eos_flags |= $2 * REOS;}
		| T_REOS '=' T_BOOL           { ibFindConfigs[findIndex].defaults.eos_flags |= $3 * REOS;}
		| T_XEOS '=' T_BOOL           { ibFindConfigs[findIndex].defaults.eos_flags |= $3 * XEOS;}
		| T_BIN T_BOOL           { ibFindConfigs[findIndex].defaults.eos_flags |= $2 * BIN; }
		| T_BIN '=' T_BOOL           { ibFindConfigs[findIndex].defaults.eos_flags |= $3 * BIN; }
		| T_EOT '=' T_BOOL           { ibFindConfigs[findIndex].defaults.send_eoi = $3;}
		| T_AUTOPOLL              { ibFindConfigs[findIndex].flags |= CN_AUTOPOLL; }
		| T_INIT_F '=' flags
		| T_NAME '=' T_STRING	{ strncpy(ibFindConfigs[findIndex].name,$3, sizeof(ibFindConfigs[findIndex].name));}
		| T_MINOR '=' T_NUMBER	{ ibFindConfigs[findIndex].defaults.board = $3;}
		| T_TIMO '=' T_TIVAL      { ibFindConfigs[findIndex].defaults.usec_timeout = $3; }
		| T_TIMO '=' T_NUMBER      { ibFindConfigs[findIndex].defaults.usec_timeout = timeout_to_usec( $3 ); }
		;

	flags: /* empty */
		| ',' flags
		| oneflag flags
		;

	oneflag: T_LLO       { ibFindConfigs[findIndex].flags |= CN_SLLO; }
		| T_DCL       { ibFindConfigs[findIndex].flags |= CN_SDCL; }
		| T_EXCL      { ibFindConfigs[findIndex].flags |= CN_EXCLUSIVE; }
		;

%%



void yyerror(char *s)
{
	fprintf(stderr, "%s\n", s);
}


