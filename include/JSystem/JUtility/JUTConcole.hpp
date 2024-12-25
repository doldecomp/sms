#ifndef JUT_CONSOLE_HPP
#define JUT_CONSOLE_HPP

extern "C" {
void JUTReportConsole(const char*);
void JUTReportConsole_f(const char*, ...);
void JUTWarningConsole(const char*);
void JUTWarningConsole_f(const char*, ...);
}

#endif
