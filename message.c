//
//  message.c
//  cdos
//
//  Created by Andre Zay on 17.12.2017.
//  Copyright © 2017 Andre Zay. All rights reserved.
//

#include "message.h"
#include "ddos.h"

bool hide_warnings;
void info(const char* format, ...)
{
    PRINT_FORMATTED(INFO)
}
void error(const char* format, ...)
{
    PRINT_FORMATTED(ERROR)
}
void warning(const char* format, ...)
{
    if (!hide_warnings) {
        PRINT_FORMATTED(WARN)
    }
}
void success(const char* format, ...)
{
    PRINT_FORMATTED(SUCCESS)
}
void success_n(const char* format, ...)
{
    PRINT_FORMATTED_NO_NEWLINE(SUCCESS)
}
void die(const char* format, ...)
{
    PRINT_FORMATTED(ERROR)
    error("Fatal error");
    error("Quiting...");
    exit(-1);
}
