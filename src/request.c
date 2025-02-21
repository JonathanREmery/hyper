#include "request.h"

/**
 * @brief Checks if a method is valid
 *
 * @param method Method string
 * @return int 0 if valid, -1 if error
 */
int is_valid_method(char method[METHOD_LEN]) {
  // GET method
  if (strcmp(method, "GET") == 0) {
    return 0;
  }

  return -1;
}

/**
 * @brief Checks if a version is valid
 *
 * @param version Version string
 * @return int 0 if valid, -1 if error
 */
int is_valid_version(char version[VERSION_LEN]) {
  // HTTP/1.1 version
  if (strcmp(version, "1.1") == 0) {
    return 0;
  }

  return -1;
}

/**
 * @brief Checks if a file name is valid
 *
 * @param file_name File name string
 * @return int 0 if valid, -1 if error
 */
int is_valid_file(char file_name[FILE_NAME_LEN]) {
  // check if file exists
  if (access(file_name, F_OK) == 0) {
    return 0;
  }

  return -1;
}

/**
 * @brief Parses a request
 *
 * @param raw_request Raw request string
 * @param result Result of the operation
 * @param cleanup Cleanup struct
 * @return request_t* Parsed request or NULL if error
 */
request_t* parse_request(const char raw_request[], request_result_t* result, request_cleanup_t* cleanup) {
  // initialize result
  *result = REQUEST_SUCCESS;

  // initialize request
  request_t* request = malloc(sizeof(request_t));
  if (request == NULL) {
    *result = REQUEST_ERR_MALLOC;
    return NULL;
  }

  // set cleanup allocation flag
  cleanup->request_allocated = 1;
  cleanup->request = request;

  // initialize method
  char method[METHOD_LEN] = {0};

  // parse method
  strncpy(method, raw_request, METHOD_LEN);
  for (int i = 0; i < METHOD_LEN; i++) {
    if (method[i] == ' ') {
      method[i] = '\0';
      break;
    }
  }

  // check method validity
  if (is_valid_method(method) == -1) {
    *result = REQUEST_ERR_INVALID_METHOD;
    return NULL;
  }

  // set method
  strncpy(request->method, method, METHOD_LEN);
  request->method[METHOD_LEN] = '\0';

  // parse version
  char* version = strstr(raw_request, HTTP_VERSION_PATTERN) + strlen(HTTP_VERSION_PATTERN);
  for (int i = 0; i < VERSION_LEN; i++) {
    if (version[i] == '\r') {
      version[i] = '\0';
      break;
    }
  }

  // check version validity
  if (is_valid_version(version) == -1) {
    *result = REQUEST_ERR_INVALID_VERSION;
    return NULL;
  }

  // set version
  strncpy(request->version, version, VERSION_LEN);
  request->version[VERSION_LEN] = '\0';

  // parse file name
  char* file_name = strchr(raw_request, '/') + 1;
  for (int i = 0; i < FILE_NAME_LEN; i++) {
    if (file_name[i] == ' ') {
      file_name[i] = '\0';
      break;
    }
  }

  // default to index.html
  if (strcmp(file_name, "") == 0) {
    strncpy(file_name, "index.html", FILE_NAME_LEN);
  }

  // check file name validity
  if (is_valid_file(file_name) == -1) {
    *result = REQUEST_ERR_INVALID_FILE;
    return NULL;
  }

  // set file_name
  strncpy(request->file_name, file_name, FILE_NAME_LEN);
  request->file_name[FILE_NAME_LEN] = '\0';

  return request;
}