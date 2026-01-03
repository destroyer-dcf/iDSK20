#ifndef __MAIN_CPP__
#define __MAIN_CPP__
#define VERSION "Version: 2.0.0"
#define PROGNAME "iDSK20"

// ANSI Color codes for cross-platform terminal colors
#ifdef _WIN32
    // Windows: Enable ANSI colors in Windows 10+
    #include <windows.h>
    #include <io.h>
    #define ENABLE_COLORS() enableWindowsColors()
    #define COLORS_SUPPORTED() isWindowsColorsSupported()
#else
    // Unix/Linux/macOS: ANSI colors supported by default
    #define ENABLE_COLORS() 
    #define COLORS_SUPPORTED() true
#endif

// Color definitions
#define COLOR_RESET     "\033[0m"
#define COLOR_BOLD      "\033[1m"
#define COLOR_DIM       "\033[2m"

// Text colors
#define COLOR_RED       "\033[31m"
#define COLOR_GREEN     "\033[32m"
#define COLOR_YELLOW    "\033[33m"
#define COLOR_BLUE      "\033[34m"
#define COLOR_MAGENTA   "\033[35m"
#define COLOR_CYAN      "\033[36m"
#define COLOR_WHITE     "\033[37m"
#define COLOR_GRAY      "\033[90m"

// Bright colors
#define COLOR_BRIGHT_RED     "\033[91m"
#define COLOR_BRIGHT_GREEN   "\033[92m"
#define COLOR_BRIGHT_YELLOW  "\033[93m"
#define COLOR_BRIGHT_BLUE    "\033[94m"
#define COLOR_BRIGHT_MAGENTA "\033[95m"
#define COLOR_BRIGHT_CYAN    "\033[96m"

// Combined styles for common use cases
#define COLOR_ERROR     COLOR_BRIGHT_RED
#define COLOR_SUCCESS   COLOR_BRIGHT_GREEN
#define COLOR_WARNING   COLOR_BRIGHT_YELLOW
#define COLOR_INFO      COLOR_BRIGHT_CYAN
#define COLOR_HEADER    COLOR_BOLD COLOR_BRIGHT_BLUE
#define COLOR_FILENAME  COLOR_WHITE
#define COLOR_SIZE      COLOR_YELLOW
#define COLOR_ADDRESS   COLOR_CYAN
#define COLOR_ATTRIBUTE COLOR_MAGENTA

char Nom[256];
char Msg[128];
StDirEntry TabDir[64];
int PosItem[64];
int Langue;
bool IsDsk, IsDskValid, IsDskSaved;
int TypeModeImport, TypeModeExport;

void help(void);
void help_cat(void);
void help_new(void);
void help_save(void);
void help_era(void);
void help_get(void);
void help_list(void);
void help_dams(void);
void help_disasm(void);
void help_ascii(void);
void help_hex(void);
void help_run(void);
void help_basic(void);
void DecomposeArg(char **argv, int argc);

#ifdef _WIN32
void enableWindowsColors();
bool isWindowsColorsSupported();
#endif

#endif
