/**
 * @file request.h
 * @brief Request handling for hyper project
 */

#ifndef HYPER_REQUEST_H
#define HYPER_REQUEST_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "logger.h"

#define HTTP_VERSION_PATTERN "HTTP/"

#define METHOD_LEN 8
#define VERSION_LEN 8
#define FILE_NAME_LEN 256

/**
 * @brief Request struct
 */
typedef struct {
  char method[METHOD_LEN];             /**< Method    */
  char version[VERSION_LEN];           /**< Version   */
  char file_name[FILE_NAME_LEN];       /**< File name */
} request_t;

/**
 * @brief Result of request operations
 */
typedef enum {
  REQUEST_SUCCESS             =  0,
  REQUEST_ERR_MALLOC          = -1,
  REQUEST_ERR_INVALID_METHOD  = -2,
  REQUEST_ERR_INVALID_VERSION = -3,
  REQUEST_ERR_INVALID_FILE    = -4
} request_result_t;

/**
 * @brief Request cleanup struct
 */
typedef struct {
  int request_allocated;
  void* request;
} request_cleanup_t;

/**
 * @brief Checks if a method is valid
 *
 * @param method Method string
 * @return int 0 if valid, -1 if error
 */
int is_valid_method(char method[METHOD_LEN]);

/**
 * @brief Checks if a version is valid
 *
 * @param version Version string
 * @return int 0 if valid, -1 if error
 */
int is_valid_version(char version[VERSION_LEN]);

/**
 * @brief Checks if a file path is valid
 *
 * @param file_name File name string
 * @return int 0 if valid, -1 if error
 */
int is_valid_file(char file_name[FILE_NAME_LEN]);

/**
 * @brief Parses a request
 *
 * @param raw_request Raw request string
 * @param result Result of the operation
 * @param cleanup Cleanup struct
 * @return request_t* Parsed request or NULL if error
 */
request_t* parse_request(const char raw_request[], request_result_t* result, request_cleanup_t* cleanup);

#endif