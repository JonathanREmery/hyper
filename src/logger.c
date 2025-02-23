#include "logger.h"

/**
 * @brief Logs a formatted message
 *
 * @param level Logging level
 * @param format Format string
 * @param ... Variable arguments to format
 */
void log_message(log_level_t level, const char* format, ...) {
  // initialize log message
  const char* prefixes[] = {"INFO", "ERROR", "DEBUG"};
  char log_message[16];
  snprintf(log_message, sizeof(log_message), "[%s] ", prefixes[level]);

  // initialize format message and variadic arguments
  char formatted_message[512 - strlen(log_message)];
  va_list vargs;

  // format the message with variable arguments
  va_start(vargs, format);
  vsnprintf(formatted_message, sizeof(formatted_message), format, vargs);
  va_end(vargs);

  // format final message with prefix
  char output_message[512];
  snprintf(output_message, sizeof(output_message), "%s%s", log_message, formatted_message);

  // write to stdout
  write(1, output_message, strlen(output_message));
}