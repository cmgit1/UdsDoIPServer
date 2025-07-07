#ifndef DID_HANDLERS_H
#define DID_HANDLERS_H

#include "iso14229.h"

#ifdef __cplusplus
extern "C" {
#endif

int readDidDataToSendBuffer(UDSServer_t *srv, void *arg);
int writeDidDataAndResponse(UDSServer_t *srv, void *arg);

#ifdef __cplusplus
}
#endif

#endif	// DID_HANDLERS_H
