#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "IfxCan_Can.h"
#include "IfxCan.h"
#include "IfxCpu_Irq.h"
#include "IfxPort.h"

#include "../../Libraries/ControlUnitLogicOperator/lib/raceup_board/components/can.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>


#define MAXIMUM_CAN_DATA_PAYLOAD    2                       /* Define maximum classical CAN payload in 4-byte words */
#define CAN_ISR_PROVIDER            IfxSrc_Tos_cpu0                         /* Interrupt provider                   */
#define ISR_PRIORITY_CAN_RX         2                       			    /* Define the CAN RX interrupt priority */
#define MAXIMUM_SEND_ATTEMPTS       4096
#define CAN_MESSAGE_ID              (uint32)0x777           /* Message ID that will be used in arbitration phase    */
#define PIN5                        5                       /* LED1 used in TX ISR is connected to this pin         */
#define PIN6                        6                       /* LED2 used in RX ISR is connected to this pin         */
#define INVALID_RX_DATA_VALUE       0xA5                    /* Used to invalidate RX message data content           */
#define INVALID_ID_VALUE            (uint32)0xFFFFFFFF      /* Used to invalidate RX message ID value               */
#define ISR_PRIORITY_CAN_TX         2                       /* Define the CAN TX interrupt priority                 */
#define ISR_PRIORITY_CAN_RX         1                       /* Define the CAN RX interrupt priority                 */
#define TX_DATA_LOW_WORD            (uint32)0xC0CAC01A      /* Define CAN data lower word to be transmitted         */
#define TX_DATA_HIGH_WORD           (uint32)0xBA5EBA11      /* Define CAN data higher word to be transmitted        */
#define MAXIMUM_CAN_DATA_PAYLOAD    2                       /* Define maximum classical CAN payload in 4-byte words */


typedef struct
{
    IfxCan_Can_Config canConfig;                            /* CAN module configuration structure                   */
    IfxCan_Can canModule;                                   /* CAN module handle                                    */
    IfxCan_Can_Node canSrcNode;                             /* CAN source node handle data structure                */
    IfxCan_Can_Node canDstNode;                             /* CAN destination node handle data structure           */
    IfxCan_Can_NodeConfig canNodeConfig;                    /* CAN node configuration structure                     */
    IfxCan_Filter canFilter;                                /* CAN filter configuration structure                   */
    IfxCan_Message txMsg;                                   /* Transmitted CAN message structure                    */
    IfxCan_Message rxMsg;                                   /* Received CAN message structure                       */
    uint32 txData[MAXIMUM_CAN_DATA_PAYLOAD];                /* Transmitted CAN data array                           */
    uint32 rxData[MAXIMUM_CAN_DATA_PAYLOAD];                /* Received CAN data array                              */
} McmcanType;

McmcanType                  g_mcmcan;

struct CanNode{
    struct{
        IfxCan_Can_Config canConfig;                            /* CAN module configuration structure                   */
        IfxCan_Can canModule;                                   /* CAN module handle                                    */
        IfxCan_Can_Node canNode;                                /* CAN node handle data structure                       */
        IfxCan_Can_NodeConfig canNodeConfig;                    /* CAN node configuration structure                     */
        IfxCan_Message txMsg;                                   /* Transmitted CAN message structure                    */
        IfxCan_Message rxMsg;                                   /* Received CAN message structure                       */
        uint32 txData[MAXIMUM_CAN_DATA_PAYLOAD];                /* Transmitted CAN data array                           */
        uint32 rxData[MAXIMUM_CAN_DATA_PAYLOAD];                /* Received CAN data array                              */
    } g_mcmcan;
    boolean canBusBusy;
};

static struct CanNode nodes[3];



static struct CanNode* extract_can_node_from_id(const BoardComponentId id){
    switch (id) {
        case 0:
            return &nodes[0];
        default:
            return NULL;
    }
}
static void setup_pin_from_id(const BoardComponentId id,IfxCan_Can_Pins* pins){
    pins->txPinMode = IfxPort_OutputMode_pushPull;
    pins->rxPinMode = IfxPort_InputMode_pullUp;
    pins->padDriver = IfxPort_PadDriver_cmosAutomotiveSpeed1;
    pins->txPin = &IfxCan_TXD00_P20_8_OUT;
    pins->rxPin = &IfxCan_RXD00B_P20_7_IN;
    switch (id) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        default:
            break;
    }
}


static void setup_interrupt_from_id(const BoardComponentId id)
{
    struct CanNode* can_node= extract_can_node_from_id(id);
    IfxCan_Can_InterruptConfig* interrupt_config = &can_node->g_mcmcan.canNodeConfig.interruptConfig;


    switch (id) {
        case 0:
            interrupt_config->messageStoredToDedicatedRxBufferEnabled = TRUE;
            interrupt_config->reint.priority = ISR_PRIORITY_CAN_RX;
            interrupt_config->reint.interruptLine = IfxCan_InterruptLine_0;
            interrupt_config->reint.typeOfService = CAN_ISR_PROVIDER;
            break;
        default:
            break;
    
    }
}

//public
int8_t hardware_init_can(const BoardComponentId id,uint32_t baud_rate)
{
    /* ==========================================================================================
     * CAN module configuration and initialization:
     * ==========================================================================================
     *  - load default CAN module configuration into configuration structure
     *  - initialize CAN module with the default configuration
     * ==========================================================================================
     */
    IfxCan_Can_initModuleConfig(&g_mcmcan.canConfig, &MODULE_CAN0);

    IfxCan_Can_initModule(&g_mcmcan.canModule, &g_mcmcan.canConfig);

    /* ==========================================================================================
     * Source CAN node configuration and initialization:
     * ==========================================================================================
     *  - load default CAN node configuration into configuration structure
     *
     *  - set source CAN node in the "Loop-Back" mode (no external pins are used)
     *  - assign source CAN node to CAN node 0
     *
     *  - define the frame to be the transmitting one
     *
     *  - once the transmission is completed, raise the interrupt
     *  - define the transmission complete interrupt priority
     *  - assign the interrupt line 0 to the transmission complete interrupt
     *  - transmission complete interrupt service routine should be serviced by the CPU0
     *
     *  - initialize the source CAN node with the modified configuration
     * ==========================================================================================
     */
    IfxCan_Can_initNodeConfig(&g_mcmcan.canNodeConfig, &g_mcmcan.canModule);
    IfxCan_Can_Pins pins;
    pins.txPinMode = IfxPort_OutputMode_pushPull;
    pins.rxPinMode = IfxPort_InputMode_pullUp;
    pins.padDriver = IfxPort_PadDriver_cmosAutomotiveSpeed1;
    pins.txPin = &IfxCan_TXD00_P20_8_OUT;
    pins.rxPin = &IfxCan_RXD00B_P20_7_IN;

    g_mcmcan.canNodeConfig.pins = &pins;
    g_mcmcan.canNodeConfig.busLoopbackEnabled = FALSE;
    g_mcmcan.canNodeConfig.nodeId = IfxCan_NodeId_0;
    g_mcmcan.canNodeConfig.baudRate.baudrate = 500000;


    g_mcmcan.canNodeConfig.frame.type = IfxCan_FrameType_transmit;

    IfxCan_Can_initNode(&g_mcmcan.canSrcNode, &g_mcmcan.canNodeConfig);

    // struct CanNode* can_node= extract_can_node_from_id(id);
    //
    // IfxCpu_disableInterrupts();
    // IfxCan_Can_Pins pins;
    // setup_pin_from_id(id, &pins);
    //
    // IfxCan_Can_initModuleConfig(&can_node->g_mcmcan.canConfig, pins.rxPin->module);
    //
    // IfxCan_Can_initModule(&can_node->g_mcmcan.canModule, &can_node->g_mcmcan.canConfig);
    // 
    // IfxCan_Can_initNodeConfig(&can_node->g_mcmcan.canNodeConfig, &can_node->g_mcmcan.canModule);
    //
    // //g_mcmcan.canNodeConfig.busLoopbackEnabled = TRUE;
    // can_node->g_mcmcan.canNodeConfig.pins = &pins;
    // can_node->g_mcmcan.canNodeConfig.nodeId = pins.rxPin->nodeId;
    // can_node->g_mcmcan.canNodeConfig.baudRate.baudrate = baud_rate;
    //
    // can_node->g_mcmcan.canNodeConfig.frame.type = IfxCan_FrameType_transmitAndReceive;
    //
    // // setup_interrupt_from_id(id);
    //
    // if(!IfxCan_Can_initNode(&(can_node->g_mcmcan.canNode), &(can_node->g_mcmcan.canNodeConfig)))
    // {
    //     while(1){}
    // }
    //
    // IfxCpu_enableInterrupts();
    return 0;
}

int8_t hardware_read_can(const BoardComponentId id, CanMessage* mex)
{
    struct CanNode* can_node= extract_can_node_from_id(id);
    IfxCan_Can_readMessage(&can_node->g_mcmcan.canNode , &can_node->g_mcmcan.rxMsg , can_node->g_mcmcan.rxData );
    memcpy(mex->buffer, &(can_node->g_mcmcan.rxData), 8);
    mex->id = can_node->g_mcmcan.rxMsg.messageId;
    mex->message_size = 8;

    return 0;
}

int8_t hardware_write_can(const BoardComponentId id, const CanMessage* restrict const mex)
{
    IfxCan_Can_initMessage(&g_mcmcan.rxMsg);

       /* Invalidation of the RX message data content */
       memset((void *)(&g_mcmcan.rxData[0]), INVALID_RX_DATA_VALUE, MAXIMUM_CAN_DATA_PAYLOAD * sizeof(uint32));

       /* Initialization of the TX message with the default configuration */
       IfxCan_Can_initMessage(&g_mcmcan.txMsg);

       /* Define the content of the data to be transmitted */
       g_mcmcan.txData[0] = TX_DATA_LOW_WORD;
       g_mcmcan.txData[1] = TX_DATA_HIGH_WORD;

       /* Set the message ID that is used during the receive acceptance phase */
       g_mcmcan.txMsg.messageId = CAN_MESSAGE_ID;

       /* Send the CAN message with the previously defined TX message content */
       while( IfxCan_Status_notSentBusy ==
              IfxCan_Can_sendMessage(&g_mcmcan.canSrcNode, &g_mcmcan.txMsg, &g_mcmcan.txData[0]) )
       {
       }
       return 0;
//     struct CanNode* can_node= extract_can_node_from_id(id);
//     IfxCan_Can_initMessage(&(can_node->g_mcmcan.txMsg));
//
//     /* Define the content of the data to be transmitted */
//     uint8_t mex_size = mex->message_size % 9;
//     uint8_t* data_buffer = mex->buffer;
// //    memcpy(&can_node->g_mcmcan.txData[0], data_buffer, mex->message_size);
// //    if (mex->message_size > 4) {
// //        memcpy(&can_node->g_mcmcan.txData[1], &data_buffer[4], mex_size - 4);
// //    }
//     can_node->g_mcmcan.txData[0] = 12;
//     /* Set frame parameters */
//     can_node->g_mcmcan.txMsg.messageId = 12;
//     can_node->g_mcmcan.txMsg.dataLengthCode = 8;
//     can_node->g_mcmcan.txMsg.errorStateIndicator = FALSE;
//     can_node->g_mcmcan.txMsg.remoteTransmitRequest = FALSE;
//     can_node->g_mcmcan.txMsg.storeInTxFifoQueue = FALSE;
//     can_node->g_mcmcan.txMsg.txEventFifoControl = FALSE;
//
//     /* Send the CAN message with the previously defined TX message content */
//
//     uint16 sendRetry = 0;
//     while(IfxCan_Status_notSentBusy == IfxCan_Can_sendMessage(
//                 &(can_node->g_mcmcan.canNode), 
//                 &(can_node->g_mcmcan.txMsg), 
//                 &(can_node->g_mcmcan.txData[0])) && sendRetry < MAXIMUM_SEND_ATTEMPTS){
//         sendRetry++;
//     }
//
//     return !(sendRetry < MAXIMUM_SEND_ATTEMPTS);
}
