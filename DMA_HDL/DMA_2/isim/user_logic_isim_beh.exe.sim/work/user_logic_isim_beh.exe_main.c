/**********************************************************************/
/*   ____  ____                                                       */
/*  /   /\/   /                                                       */
/* /___/  \  /                                                        */
/* \   \   \/                                                       */
/*  \   \        Copyright (c) 2003-2009 Xilinx, Inc.                */
/*  /   /          All Right Reserved.                                 */
/* /---/   /\                                                         */
/* \   \  /  \                                                      */
/*  \___\/\___\                                                    */
/***********************************************************************/

#include "xsi.h"

struct XSI_INFO xsi_info;

char *IEEE_P_2592010699;
char *STD_STANDARD;
char *IEEE_P_3499444699;
char *IEEE_P_3620187407;
char *PROC_COMMON_V3_00_A_P_2444876401;
char *PROC_COMMON_V3_00_A_P_4147123038;
char *IEEE_P_1242562249;


int main(int argc, char **argv)
{
    xsi_init_design(argc, argv);
    xsi_register_info(&xsi_info);

    xsi_register_min_prec_unit(-12);
    ieee_p_2592010699_init();
    ieee_p_3499444699_init();
    ieee_p_3620187407_init();
    proc_common_v3_00_a_p_2444876401_init();
    ieee_p_1242562249_init();
    proc_common_v3_00_a_p_4147123038_init();
    proc_common_v3_00_a_a_0307373905_3306564128_init();
    proc_common_v3_00_a_a_2419661608_3212880686_init();
    proc_common_v3_00_a_a_0030151582_3306564128_init();
    proc_common_v3_00_a_a_1627319469_3306564128_init();
    work_a_3268654154_3306564128_init();


    xsi_register_tops("work_a_3268654154_3306564128");

    IEEE_P_2592010699 = xsi_get_engine_memory("ieee_p_2592010699");
    xsi_register_ieee_std_logic_1164(IEEE_P_2592010699);
    STD_STANDARD = xsi_get_engine_memory("std_standard");
    IEEE_P_3499444699 = xsi_get_engine_memory("ieee_p_3499444699");
    IEEE_P_3620187407 = xsi_get_engine_memory("ieee_p_3620187407");
    PROC_COMMON_V3_00_A_P_2444876401 = xsi_get_engine_memory("proc_common_v3_00_a_p_2444876401");
    PROC_COMMON_V3_00_A_P_4147123038 = xsi_get_engine_memory("proc_common_v3_00_a_p_4147123038");
    IEEE_P_1242562249 = xsi_get_engine_memory("ieee_p_1242562249");

    return xsi_run_simulation(argc, argv);

}
