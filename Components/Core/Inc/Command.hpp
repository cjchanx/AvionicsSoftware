/**
 ******************************************************************************
 * File Name          : Command.hpp
 * Description        : Command is a unique object used to communicate information
 *	to and between tasks.
 ******************************************************************************
*/
#ifndef AVIONICS_INCLUDE_SOAR_CORE_COMMAND_H
#define AVIONICS_INCLUDE_SOAR_CORE_COMMAND_H
/* Includes ------------------------------------------------------------------*/
#include "cmsis_os.h"

/* Macros --------------------------------------------------------------------*/

/* Enums -----------------------------------------------------------------*/
enum GLOBAL_COMMANDS : uint8_t
{
	COMMAND_NONE = 0,		// No command, packet can probably be ignored
	TASK_SPECIFIC_COMMAND,	// Runs a task specific command when given this object
};

/* Class -----------------------------------------------------------------*/

/**
 * @brief Command class
 *
 * Each Command object contains one set of commands, a GLOBAL_COMMANDS and a task command that can be task specific.
 *
 * Note, this class must be able to be treated as 'Plain-Old-Data' as it will be handled with raw-copy in RTOS queues
*/
class Command
{
public:
	Command(void);
	Command(GLOBAL_COMMANDS command);
	Command(uint16_t taskCommand);

	//~Command();	// We can't handle memory like this, since the object would be 'destroyed' after copying to the RTOS queue

	// Functions
	bool AllocateData(uint16_t dataSize);	// Dynamically allocates data for the command
	bool SetCommandData(uint8_t* dataPtr, uint16_t size, bool bFreeMemory);	// Set data pointer to a pre-allocated buffer, if bFreeMemory is set to true, responsibility for freeing memory will fall on Command

	void Reset();	// Reset the command, equivalent of a destructor that must be called, counts allocations and deallocations, asserts an error if the allocation count is too high

	// Getters
	uint16_t GetDataSize() const;
	const uint8_t* GetDataPointer() const { return data; }
	GLOBAL_COMMANDS GetCommand() const { return command; }
	uint16_t GetTaskCommand() const { return taskCommand; }


protected:
	// Data -- note each insertion and removal from a queue will do a full copy of this object, so this data should be as small as possible
	GLOBAL_COMMANDS command;	// General GLOBAL command, each task must be able to handle these types of commands
	uint16_t taskCommand;		// Task specific command, the task this command event is sent to needs to handle this

	uint8_t* data;				// Pointer to optional data
	uint16_t dataSize;			// Size of optional data

private:
	bool bShouldFreeData;		// Should the Command handle freeing the data pointer

	static uint16_t statAllocationCounter;	// Static allocation counter shared by all command objects
};

#endif /* AVIONICS_INCLUDE_SOAR_CORE_COMMAND_H */