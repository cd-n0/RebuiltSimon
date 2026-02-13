#include "logger.h"
#define CLOG_IMPLEMENTATION
#include "clog.h"

static FILE *log_file;

static void rs_log_callback(clog_log_level_e log_level, const char* message) {
	if (log_file) {
		fprintf(log_file, message);
		fflush(log_file);
	}
	if (g_CoF.pEngine) g_CoF.pEngine->Con_Printf(message);
}

void initialize_logger(void) {
	log_file = fopen("RebuiltSimon.log", "a+b");
	clog_set_log_callback(rs_log_callback);
}

void deinitialize_logger(void) {
	clog_set_log_callback(NULL);
	fclose(log_file);
}