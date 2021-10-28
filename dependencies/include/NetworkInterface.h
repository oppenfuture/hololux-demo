//
//  NetworkInterface.h
//
//  Copyright © 2019 OppenFuture Technologies INC. All rights reserved.
//
#ifndef HOLOLUX_NETWORK_INTERFACE_H
#define HOLOLUX_NETWORK_INTERFACE_H

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

/**
 * Network error code constants.
 */
#define HLNET_ERR_NONE 0
#define HLNET_ERR_RANGE_UNSUPPORTED 1
#define HLNET_ERR_FAIL 2
#define HLNET_ERR_LENGTH_UNSUPPORTED 3
#define HLNET_ERR_INVALID_RANGE 4

/**
 * Download phase constants
 */
#define HLDL_PHASE_SIZE 0
#define HLDL_PHASE_DESC 1
#define HLDL_PHASE_LOWRES 2
#define HLDL_PHASE_HIGHRES 3

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct {
  const char *url;
  uint8_t phase;
  uint64_t offset;
  uint64_t length;
  uint64_t hl_ctx;
} HLHttpRequest;

typedef struct {
  uint8_t error;
  const char *data;
  uint64_t offset;
  uint64_t length;
  uint64_t hl_ctx;
} HLHttpResponse;

/**
 * @brief Network response callback typedef.
 * @details Function signature for the callbacks handling successful and failed network requests.
 *
 * @param response Response data structure.
 */
typedef void (*HLHttpResponseProc)(const HLHttpResponse *response);

/**
 * @brief Network operation callback typedef.
 * @details Function signature of both the GetFileSize and GetFileRange network APIs.
 *
 * @param request Pointer to an HTTP request struct, with the arguments of the request.
 * @param app_ctx App defined user data.
 * @param on_success Callback for successful completion of the request.
 * @param on_error Callback for any errors.
 */
typedef void (*HLNetworkProc)(const HLHttpRequest *request, void *app_ctx,
  HLHttpResponseProc on_success, HLHttpResponseProc on_error);

/**
 * @brief Cancellation callbacks.
 * @details Callback to cancel all outstanding network requests.
 * MUST Guarantee no further invokations of network request callbacks
 * on the outstanding requests will happen after the function returns.
 *
 * @param app_ctx App defined user data.
 */
typedef void (*HLCancelAllRequestsProc)(void *app_ctx);

/**
 * @brief Congregation of platform dependent network interfaces to be used by Hololux SDK.
 */
typedef struct {
  HLNetworkProc get_file_size;
  HLNetworkProc get_file_range;
  HLCancelAllRequestsProc cancel_all_requests;
} HLNetworkInterfaces;


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HOLOLUX_NETWORK_INTERFACE_H
