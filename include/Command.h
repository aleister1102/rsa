#pragma once
#include "RSA.h"

class Command {
public:
	static void run();
};

int printKeyTypeMenu();
RSA handleKeyTypeMenu(int type);
int printKeySizeMenu();
int handleKeySizeMenu(int option);
int printKeyBaseMenu();
int handleKeyBaseMenu(int option);
int printExportMethodMenu();
int handleExportMethodMenu(int option);
int printOperationMenu();
void handleOperationMenu(RSA rsa, int option);