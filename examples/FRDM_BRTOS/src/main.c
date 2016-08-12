//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "BRTOS.h"
#include "tasks.h"
#include "xhw_types.h"
#include "xsysctl.h"
#include "MKL25Z4.h"
#include "xhw_ints.h"
#include "xcore.h"

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

BRTOS_Mbox     *Data;

int main(void)
{
	// Initialize BRTOS
	BRTOS_Init();
	////////////////////////////////////////////////////////

	// Cria um gerenciador de eventos p/ dados acelerômetro
	if (OSMboxCreate(&Data, NULL) != ALLOC_EVENT_OK)
	{
		// Oh Oh
		// Não deveria entrar aqui !!!
		while(1){};
		};

	if(InstallTask(&System_Time,"System Time",384,31,NULL,NULL) != OK)
	{
		// Oh Oh
		// N�o deveria entrar aqui !!!
		while(1){};
	};

	if(InstallTask(&Test_Task_1,"Tarefa de Teste 1",384,13,NULL,NULL) != OK)
	{
		// Oh Oh
		// N�o deveria entrar aqui !!!
		while(1){};
	};

	if(InstallTask(&Test_Task_2,"Tarefa de Teste 2",384,14,NULL,NULL) != OK)
	{
		// Oh Oh
		// N�o deveria entrar aqui !!!
		while(1){};
	};

	if(InstallTask(&SerialTask,"Tarefa Serial",512,10,NULL,NULL) != OK)
	{
		// Oh Oh
		// N�o deveria entrar aqui !!!
		while(1){};
	};

	// Start Task Scheduler
	if(BRTOSStart() != OK)
	{
		// Oh Oh
		// N�o deveria entrar aqui !!!
		for(;;){};
	};

	// Infinite loop
	while (1)
	{
	}
	// Infinite loop, never return.
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
