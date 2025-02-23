/**
 * @file logger.h
 * @brief Logging functionality for hyper project
 */

#ifndef HYPER_LOGGER_H
#define HYPER_LOGGER_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

/**
 * @brief Logging levels
 */
typedef enum {
  LOG_INFO  = 0,
  LOG_ERROR = 1,
  LOG_DEBUG = 2
} log_level_t;

/**
 * @brief Logs a formatted message
 *
 * @param level Logging level
 * @param format Format string
 * @param ... Variable arguments to format
 */
void log_message(log_level_t level, const char* format, ...);

#endif