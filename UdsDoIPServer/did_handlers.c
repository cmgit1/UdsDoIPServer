#include "did_handlers.h"

#define SERIAL_NUMBER_LENGTH 20
uint8_t serial_number[SERIAL_NUMBER_LENGTH] = {0};

int readDidDataToSendBuffer(UDSServer_t *srv, void *arg) {
    size_t index = srv->r.send_len;
    uint8_t ret = UDS_PositiveResponse;
    UDSRDBIArgs_t *requestParameter = (UDSRDBIArgs_t *) arg;

    switch (requestParameter->dataId)
    {
    case 0xf18c:
        for(int i=0; i<SERIAL_NUMBER_LENGTH; i++) {
            srv->r.send_buf[index + i] = serial_number[i];
        }
        index += SERIAL_NUMBER_LENGTH;
        break;
    case 0x3200:
        srv->r.send_buf[index] = 0x0;
        index += 1;
        break;
    case 0x3201:
        srv->r.send_buf[index] = 0x1;
        index += 1;
        break;
    default:
        ret = UDS_NRC_RequestOutOfRange;
        break;
    }

    srv->r.send_len = index;

    return ret;
}

int writeDidDataAndResponse(UDSServer_t *srv, void *arg) {
    size_t index = srv->r.send_len;
    uint8_t ret = UDS_PositiveResponse;
    UDSWDBIArgs_t *requestParameter = (UDSWDBIArgs_t *) arg;

    switch (requestParameter->dataId) {
    case 0xf18c:
        if(requestParameter->len != SERIAL_NUMBER_LENGTH) {
            ret = UDS_NRC_IncorrectMessageLengthOrInvalidFormat;
        } else {
            for (int i=0; i<SERIAL_NUMBER_LENGTH; i++) {
                serial_number[i] = requestParameter->data[i];
            }
        }
        break;
    default:
        ret = UDS_NRC_RequestOutOfRange;
        break;
    }

    srv->r.send_len = index;

    return ret;
}
