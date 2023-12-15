#ifndef BB_SYSTEM_WINDOWS_H
#define BB_SYSTEM_WINDOWS_H

#include <bb/system/system.h>
#include <bb/bank/bank.h>
#include <windows.h>
#include <string>

class WindowsSystemDriver : public BBSystemDriver{
private:
	void refreshSystemProperties();
public:
	bool isXPorLess();

public:
	WindowsSystemDriver();
	~WindowsSystemDriver();
};

#include "commands.h"

#define bbWindowsSystemDriver ((WindowsSystemDriver*)bbSystemDriver)

#endif
