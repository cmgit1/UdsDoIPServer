#include "did_handlers.h"

int readDidDataToSendBuffer(UDSServer_t *srv, void *arg) {
    size_t index = srv->r.send_len;
    uint8_t ret = UDS_PositiveResponse;
    UDSRDBIArgs_t *requestParameter = (UDSRDBIArgs_t *) arg;
    // printf("data id is %d\n", requestParameter->dataId);
    switch (requestParameter->dataId)
    {
    case 0x3200:
        srv->r.send_buf[index] = 0x0;
        index += 1;
        break;
    case 0x3201:
        srv->r.send_buf[index] = 0x1;
        index += 1;
        break;
    default:
        ret = UDS_NRC_SubFunctionNotSupported;
        break;
    }

    srv->r.send_len = index;

    return ret;
}
