#ifndef UDS_DOIP_APP_INTERFACES_H
#define UDS_DOIP_APP_INTERFACES_H

#include "iso14229.h"
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const unsigned short SERVER_LOGICAL_ADDRESS, CLIENT_LOGICAL_ADDRESS;

/* Tx and Rx buffers shared by both DoIP and UDS to avoid buffer copy.
*  Data frame in buffer is like this
*  Generic DoIP Header (8 bytes) | Source Logic Address (2 bytes) | Target Logic Address (2 bytes) | UDS Data (0 to N bytes) |
*/
extern unsigned char tcp_recv_buf[], tcp_send_buf[];

/* The number of bytes of received UDS payload which hasn't been processed by UDS layer.
 * 
   DoIP will set this variable based on received UDS message length,
   UDS will clear this variable when it ackownledges the message. 
   DoIP cannot receive another message before this variable is cleared.
*/
extern size_t recv_uds_len;

void DoIPServerSendDiagnosticPayload(unsigned short sourceAddress, unsigned char* data, int length);
void InitializeUDSServer(UDSServer_t *uds_srv, UDSTp_t *tp);
ssize_t doip_get_send_buf(struct UDSTp *hdl, uint8_t **p_buf);
ssize_t doip_send(struct UDSTp *hdl, uint8_t *buf, size_t len, UDSSDU_t *info);
UDSTpStatus_t doip_poll(struct UDSTp *hdl);
ssize_t doip_peek(struct UDSTp *hdl, uint8_t **buf, UDSSDU_t *info);
void doip_ack_recv(struct UDSTp *hdl);
int udsServerCallback(UDSServer_t *srv, UDSEvent_t ev, void *arg);

#ifdef __cplusplus
}
#endif

#endif   // UDS_DOIP_APP_INTERFACES_H
