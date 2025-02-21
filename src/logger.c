#include "logger.h"

/**
 * @brief Logs a message
 *
 * @param level Logging level
 * @param message Message to log
 */
void log_message(log_level_t level, const char message[]) {
  // initialize variables
  const char* prefixes[] = {"INFO", "ERROR", "DEBUG"};
  char log_message[1024];

  // format message
  sprintf(log_message, "[%s] %s", prefixes[level], message);

  // write to stdout
  write(1, log_message, strlen(log_message));
}