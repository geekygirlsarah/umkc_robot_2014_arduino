#include "exit_handlers.h"
ExitHandler::ExitHandler(Logger* logger_){
	LedNotifier ledNotifier(true);
	logger = logger_;
}

//id_flame will take our return code from id_flame and throw led 
//notification based on the outcome
void ExitHandler::id_flame(int returnCode){
	//switch based on return code 
	flame = 0;
	switch(returnCode)
	{
	case 0:
	{
		ledNotifier.throwLedCode("id_flame_notfound");
	}
	case 1:
	{
		flame = 1; 
		ledNotifier.throwLedCode("id_flame_square");
	}
	case 2:
	{
		flame = 2;
		ledNotifier.throwLedCode("id_flame_triangle");
	}
	case 3:
	{
		flame = 3;
		ledNotifier.throwLedCode("id_flame_circle");
	}
	case 10:
	case 20:
	case 30:
	case 40:
	{
		ledNotifier.throwLedCode("cant_find_file");
	}
}
	//set our internal private variable flame = to returnCode
	//for future reference in other programs
	flame = returnCode;
}

//id_tool function will take our return code from id_tool program and 
//throw led notification based on the outcome
void ExitHandler::id_tool(int returnCode)
{
	if(returnCode == 0)
	{
		ledNotifier.throwLedCode("id_tool_failure");
	}
	//set our private variable tool = to returnCode for documentation
	tool = returnCode;
}

