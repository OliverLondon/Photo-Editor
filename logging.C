#include <logging.h>
#include <string.h>

FILE *Logger::logger = logger = fopen("log","w");
void Logger::Finalize(){fclose(logger);}

void Logger::LogEvent(const char *event)
{
	fwrite(event, sizeof(char), strlen(event), logger);
}

DataFlowException::DataFlowException(const char *type, const char *error)
{
	sprintf(this->msg,"Throwing Exception: (%s): %s",type,error);
	Logger::LogEvent(this->msg);
	Logger::Finalize();
}