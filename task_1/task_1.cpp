#include <iostream>
#include "command.h"

void print(LogCommand& command, const std::string& message ) {
    command.print(message);
}

int main()
{
    ConsoleLogCommand consolecmd;
    FileLogCommand filecmd("log.txt");
    print(consolecmd, "Message1");
    print(filecmd, "Message2");
    return 0;
}

