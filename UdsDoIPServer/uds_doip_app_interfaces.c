#include "uds_doip_app_interfaces.h"
#include "iso14229.h"
#include "uds_service_handlers.h"
#include <sys/types.h>

const unsigned short SERVER_LOGICAL_ADDRESS = 0x3000;
const unsigned short CLIENT_LOGICAL_ADDRESS = 0x0e80;

#define BUFFER_SIZE  (4096 + 12)
unsigned char tcp_recv_buf[BUFFER_SIZE];
unsigned char tcp_send_buf[BUFFER_SIZE];
static const size_t UDS_PAYLOAD_OFFSET = 12;    // UDS data always starts at index 12 in the buffer.

size_t recv_uds_len = 0;

ssize_t doip_get_send_buf(struct UDSTp *hdl, uint8_t **p_buf) {
    (void)hdl;
/** 
 * Get UDS message sending buffer's starting address and size.
 * @param p_buf          Pointer to UDS sending buffer's starting address
 * @return               The maximum number of bytes of a UDS message can be put in the sending buffer
 */
    *p_buf = &(tcp_send_buf[12]);
    return sizeof(tcp_send_buf) - 12;
}

ssize_t doip_send(struct UDSTp *hdl, uint8_t *buf, size_t len, UDSSDU_t *info) {
    (void)hdl;
    (void)info;
    DoIPServerSendDiagnosticPayload(SERVER_LOGICAL_ADDRESS, buf, len);
    return len;
}

UDSTpStatus_t doip_poll(struct UDSTp *hdl) {
    (void)hdl;
    return UDS_TP_IDLE;
}

ssize_t doip_peek(struct UDSTp *hdl, uint8_t **buf, UDSSDU_t *info) {
    (void)hdl;
    (void)info;
    *buf = tcp_recv_buf + UDS_PAYLOAD_OFFSET;
    return recv_uds_len;
}
    
void doip_ack_recv(struct UDSTp *hdl) {
    (void)hdl;
    recv_uds_len = 0;
}

void InitializeUDSServer(UDSServer_t *uds_srv, UDSTp_t *tp) {
    tp->get_send_buf = doip_get_send_buf;
    tp->send = doip_send;
    tp->poll = doip_poll;
    tp->peek = doip_peek;
    tp->ack_recv = doip_ack_recv;
            
    UDSServerInit(uds_srv);
    uds_srv->tp = tp;
    uds_srv->fn = udsServerCallback;
}

int udsServerCallback(UDSServer_t *srv, UDSEvent_t ev, void *arg) {
    int ret;
    switch (ev) 
    {
    case UDS_EVT_ReadDataByIdent:
        ret = readDidDataToSendBuffer(srv, arg);
        break;
    default:
        printf("Unhandled event: %d\n", ev);
        ret = UDS_NRC_GeneralReject;
        break;
    }

    return ret;
}
