/***
 * monitoringplug - check_qnap_disks.c
 **
 *
 * Copyright (C) 2011 Marius Rieder <marius.rieder@durchmesser.ch>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Id$
 */

const char *progname  = "check_qnap_disks";
const char *progvers  = "0.1";
const char *progcopy  = "2011";
const char *progauth = "Marius Rieder <marius.rieder@durchmesser.ch>";
const char *progusage = "[--help] [--timeout TIMEOUT]";

/* MP Includes */
#include "mp_common.h"
#include "snmp_utils.h"
/* Default Includes */
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Library Includes */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>


/* Global Vars */
const char  *hostname = NULL;
int         port = 0;

int main (int argc, char **argv) {
    /* Local Vars */
    int         i;
    char        *output = NULL;
    int         status = STATE_OK;
    struct mp_snmp_table    table_state;
    netsnmp_session         *ss;
    netsnmp_variable_list   *vars, *vars2;

    /* Set signal handling and alarm */
    if (signal (SIGALRM, timeout_alarm_handler) == SIG_ERR)
        critical("Setup SIGALRM trap faild!");

    /* Process check arguments */
    if (process_arguments(argc, argv) != OK)
        unknown("Parsing arguments faild!");

    /* Start plugin timeout */
    alarm(mp_timeout);

    // PLUGIN CODE
    ss = mp_snmp_init();

    // Default timeout is to short
    ss->timeout = 5;

    /* OIDs to query */
    struct mp_snmp_query_cmd snmpcmd_table = {{1,3,6,1,4,1,24681,1,2,11}, 10, 0, (void *)&table_state};

    mp_snmp_table_query(ss, &snmpcmd_table);

    mp_snmp_deinit();

    for (i = 0; i<table_state.row; i++) {
        vars = mp_snmp_table_get(table_state, 7, i);

        if (strcmp((char *)vars->val.string, "GOOD") == 1) {
            vars2 = mp_snmp_table_get(table_state, 2, i);

            char *t = (char *)malloc(5 + vars->val_len + vars2->val_len);
            sprintf(t, "%s is %s", vars2->val.string, vars->val.string);

            mp_strcat_space(&output, t);
            free(t);
            status = STATE_CRITICAL;
        }
    }
    /* Output and return */
    if (status == STATE_OK)
        ok("QNAP: All Disks \"GOOD\"");
    critical("QNAP: %s", output);
}

int process_arguments (int argc, char **argv) {
    int c;
    int option = 0;

    static struct option longopts[] = {
            MP_LONGOPTS_DEFAULT,
            MP_LONGOPTS_HOST,
            MP_LONGOPTS_PORT,
            SNMP_LONGOPTS,
            MP_LONGOPTS_TIMEOUT,
            MP_LONGOPTS_END
    };

    if (argc < 3) {
       print_help();
       exit(STATE_OK);
    }


    while (1) {
        c = getopt_long (argc, argv, MP_OPTSTR_DEFAULT"t:H:p:o:O:"SNMP_OPTSTR, longopts, &option);

        if (c == -1 || c == EOF)
            break;

        getopt_snmp(c);

        switch (c) {
            /* Default opts */
            MP_GETOPTS_DEFAULT
            /* Hostname opt */
            case 'H':
                getopt_host(optarg, &hostname);
                break;
            /* Port opt */
            case 'P':
                getopt_port(optarg, &port);
                break;
            /* Timeout opt */
            case 't':
                getopt_timeout(optarg);
                break;
        }
    }

    return(OK);
}

void print_help (void) {
    print_revision();
    print_revision_snmp();
    print_copyright();

    printf("\n");

    printf("This plugin check the disks of a QNAP NAS by snmp.");

    printf("\n\n");

    print_usage();
    print_help_default();
    print_help_snmp();
}

/* vim: set ts=4 sw=4 et syn=c : */
