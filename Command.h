#pragma once
#include <iostream>

constexpr int MAX_COMMANDS = 3;

class Command
{
public:
	static void runCommandMenu();

private:
	static void runCommand(int command);
};
