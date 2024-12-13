#include "./core1.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../../missions/missons.h"
#include "../../board_conf/id_conf.h"
#include "../../lib/raceup_board/raceup_board.h"
#include "../../alive_blink/alive_blink.h"
#include "../../board_can/board_can.h"

static alive_blink_fd alive_fd;

static void setup(void)
{
    hardware_init_interrupt();
    board_can_init(CAN_MODULE_INVERTER, _500_KBYTE_S_);
    board_can_init(CAN_MODULE_GENERAL, _500_KBYTE_S_);
    board_can_init(CAN_MODULE_DV, _500_KBYTE_S_);
    alive_fd = i_m_alive_init(100 MILLIS, LED_2);
}

static void loop(void)
{
    i_m_alive(alive_fd);
    CanMessage mex;
    int read_ok=-1;

    read_ok = board_can_read(CAN_MODULE_INVERTER, &mex);
    if(read_ok >= 0){
        board_can_manage_message(CAN_MODULE_INVERTER, &mex);
    }

    read_ok = board_can_read(CAN_MODULE_GENERAL, &mex);
    if(read_ok >= 0){
        board_can_manage_message(CAN_MODULE_GENERAL, &mex);
    }

    read_ok = board_can_read(CAN_MODULE_DV, &mex);
    if(get_current_mission() != MANUALY && read_ok >= 0){
        board_can_manage_message(CAN_MODULE_DV, &mex);
    }
}

//INFO: Service core
void main_1(void)
{
    setup();
    for(;;){
        loop();
    }
}
