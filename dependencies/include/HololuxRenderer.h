//
//  HololuxRenderer.h
//
//  Copyright © 2019 OppenFuture Technologies INC. All rights reserved.
//
#ifndef HOLOLUX_RENDERER_H
#define HOLOLUX_RENDERER_H

#include "NetworkInterface.h"

#define PHOLOLUXRENDERER void*

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief Create Hololux Renderer
 * @details Create a Hololux renderer instance, must be passed to all Hololux operations.
 * Must be manually destroyed using HLDestroyRenderer.
 *
 * @param p Output parameter, a pointer to the created renderer instance.
 * @return 0 if successful, error code otherwise.
 */
int HLCreateRenderer(PHOLOLUXRENDERER* p);

/**
 * @brief HLNetworkResultProc Signature
 * @details Function signature declaration.
 * HLDownloadLightField notifies caller of its result using this format.
 */
typedef void(*HLNetworkResultProc)(void *app_ctx);

/**
 * @brief Download description and low resolution LF
 * @details Async function, start downloading Light Field model description, metadata and
 * low resolution resources.
 *
 * @param renderer Pointer to a Hololux renderer object, created by HLCreateRenderer.
 * @param url URL address of the Light Field model file.
 * @param app_ctx App defined user data to be passed to ALL follow-up network requests' callbacks.
 * @param net_apis Platform dependent network APIs. Hololux SDK will use these to fetch data from sources.
 * @param on_success Callback function when the Light Field finishes downloading.
 * @param on_error Callback function if the downloading fails.
 */
void HLDownloadLightField(
  PHOLOLUXRENDERER renderer,
  const char* url, void *app_ctx,
  const HLNetworkInterfaces *net_apis,
  HLNetworkResultProc on_success, HLNetworkResultProc on_error);

/**
 * @brief Create support scene data structures.
 * @details Create non-model-specific resources, including camera node, geometry node, textures, etc.
 *
 * @param renderer Pointer to the Hololux renderer instance, created by HLCreateRenderer.
 * @param irrlicht_device Pointer to the frostfire device instance.
 *
 * @return 0 if successful, error code otherwise.
 */
int HLCreateScene(PHOLOLUXRENDERER renderer);

/**
 * @brief Create light field data strcutures.
 * @details Create model-specific resources.
 *
 * @param renderer Pointer to the Hololux renderer instance, created by HLCreateRenderer.
 * @return 0 if successful, error code otherwise.
 */
int HLCreateLightField(PHOLOLUXRENDERER renderer);

/**
 * @brief Render preparation.
 * @details Perform necessary calculations and prepare resources to render a frame.
 * Must be called before rendering every frame.
 *
 * @param renderer Pointer to the Hololux renderer instance, created by HLCreateRenderer.
 */
void HLOnPreRender(PHOLOLUXRENDERER renderer);

/**
 * @brief Destroys model-specific data structures.
 * @details Must be called before calling HLDestroyScene or HLDestroyRenderer.
 *
 * @param renderer Pointer to the Hololux renderer instance, created by HLCreateRenderer.
 */
void HLDestroyLightField(PHOLOLUXRENDERER renderer);

/**
 * @brief Destroys non-model-specific data structures.
 * @details Must be called before calling HLDestroyRenderer.
 *
 * @param renderer Pointer to the Hololux renderer instance, created by HLCreateRenderer.
 */
void HLDestroyScene(PHOLOLUXRENDERER renderer);

/**
 * @brief Destroy the Hololux renderer instance.
 * @details Must be called last in the destruction sequence.
 *
 * @param renderer Pointer to the Hololux renderer instance, created by HLCreateRenderer.
 */
void HLDestroyRenderer(PHOLOLUXRENDERER renderer);

/**
 * @brief Inject finger touch events.
 * @details Inject finger touch events to Hololux for camera manipulation.
 *
 * @param renderer Pointer to the Hololux renderer instance, created by HLCreateRenderer.
 * @param x x coordinate of screen position.
 * @param y y coordinate of screen position.
 */
//void HLOnMouseMove(PHOLOLUXRENDERER renderer, int x, int y);

/**
 * @brief Inject finger touch events.
 * @details Inject finger touch events to Hololux for camera manipulation.
 *
 * @param renderer Pointer to the Hololux renderer instance, created by HLCreateRenderer.
 * @param button_down Boolean value indicating button action type. True == button down, False == button up.
 * @param x x coordinate of the screen position.
 * @param y y coordinate of the screen position.
 */
//void HLOnMouseButton(PHOLOLUXRENDERER renderer, bool button_down, int x, int y);

/**
 * @brief Inject finger pinch events.
 * @details Inject pinch events to Hololux for camera manipulation.
 *
 * @param renderer Pointer to the Hololux renderer instance, created by HLCreateRenderer.
 * @param scale Relative scale of the pinch, i.e. given the distances between touch points, scale = new distance / old distance.
 */
//void HLOnPinch(PHOLOLUXRENDERER renderer, float scale);

/**
 * @brief Get camera node's name.
 *
 * @param renderer Pointer to the Hololux renderer instance, created by HLCreateRenderer.
 * @return C-style string if successful, NULL otherwise.
 */
const char *HLGetCameraNodeName(PHOLOLUXRENDERER renderer);

/**
 * @brief Get the main camera's name.
 *
 * @param renderer Pointer to the Hololux renderer instance, created by HLCreateRenderer.
 * @return C-style string if successful, NULL otherwise.
 */
const char *HLGetCameraName(PHOLOLUXRENDERER renderer);

/**
 * @brief Get the model node's name.
 *
 * @param renderer Pointer to the Hololux renderer instance, created by HLCreateRenderer.
 * @return C-style string if successful, NULL otherwise.
 */
const char *HLGetModelNodeName(PHOLOLUXRENDERER renderer);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif /* HOLOLUX_RENDERER_H */
