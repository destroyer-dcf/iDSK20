#include <iostream>
#include <cstdlib>
#include <cstring>
#include <algorithm> // to work around a std::vector bug ...
#include <vector>
#include <sstream>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "getopt_pp.h" /* Command line handling */

using namespace std;

#include "MyType.h"
#include "GestDsk.h"
#include "Outils.h"
#include "Main.h"
#include "endianPPC.h"
#include "ViewFile.h"
#include "Basic.h"
#include "Ascii.h"

// Global variable to control color output
bool useColors = true;

#ifdef _WIN32
// Windows-specific functions for ANSI color support
void enableWindowsColors() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hErr = GetStdHandle(STD_ERROR_HANDLE);
    DWORD dwModeOut = 0, dwModeErr = 0;
    
    if (hOut != INVALID_HANDLE_VALUE && GetConsoleMode(hOut, &dwModeOut)) {
        dwModeOut |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwModeOut);
    }
    
    if (hErr != INVALID_HANDLE_VALUE && GetConsoleMode(hErr, &dwModeErr)) {
        dwModeErr |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hErr, dwModeErr);
    }
}

bool isWindowsColorsSupported() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    return (hOut != INVALID_HANDLE_VALUE && 
            GetConsoleMode(hOut, &dwMode) && 
            (dwMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING));
}
#endif

// Helper function to output colored text
string colorize(const string& text, const string& color) {
    if (!useColors) return text;
    return color + text + COLOR_RESET;
}

// Function to execute RetroVirtualMachine with specified parameters
bool executeRetroVirtualMachine(const string& dskFile, const string& fileToRun) {
	// Get RetroVirtualMachine path from environment variable
	const char* rvmPath = getenv("RVM_PATH");
	if (!rvmPath) {
		rvmPath = getenv("RETROVIRTUALMACHINE_PATH");
	}
	
	if (!rvmPath) {
		cerr << "Error: RetroVirtualMachine path not found." << endl;
		cerr << "Please set RVM_PATH or RETROVIRTUALMACHINE_PATH environment variable." << endl;
		cerr << "Example: export RVM_PATH=\"/Applications/Retro Virtual Machine 2.app/Contents/MacOS/Retro Virtual Machine 2\"" << endl;
		return false;
	}
	
	// Get CPC model from environment variable, default to 6128
	string cpcModel = "6128"; // Default model
	const char* envCpcModel = getenv("CPC_MODEL");
	if (envCpcModel != NULL && strlen(envCpcModel) > 0) {
		cpcModel = string(envCpcModel);
		// Validate model
		if (cpcModel != "464" && cpcModel != "664" && cpcModel != "6128") {
			cerr << "Warning: Invalid CPC_MODEL '" << cpcModel << "'. Using default 6128." << endl;
			cpcModel = "6128";
		}
	}
	
	// Close any existing RetroVirtualMachine instances
	cout << "Closing existing RetroVirtualMachine instances..." << endl;
	
	cout << "Killing RetroVirtualMachine processes..." << endl;
	
	#ifdef _WIN32
		// Windows: Use taskkill to terminate RetroVirtualMachine processes
		string killCommand1 = "taskkill /F /IM \"Retro Virtual Machine*.exe\" 2>nul || echo.";
		string killCommand2 = "taskkill /F /IM \"RetroVirtualMachine*.exe\" 2>nul || echo.";
		string killCommand3 = "wmic process where \"name like '%Retro Virtual Machine%'\" delete 2>nul || echo.";
		
		system(killCommand1.c_str());
		system(killCommand2.c_str());
		system(killCommand3.c_str());
	#else
		// Unix/Linux/macOS: Use pkill with different patterns
		string killCommand1 = "pkill -9 -f \"Retro Virtual Machine\" 2>/dev/null || true";
		string killCommand2 = "pkill -9 -f \"RetroVirtualMachine\" 2>/dev/null || true";
		string killCommand3 = "killall -9 \"Retro Virtual Machine 2\" 2>/dev/null || true";
		
		system(killCommand1.c_str());
		system(killCommand2.c_str());
		system(killCommand3.c_str());
	#endif
	
	// Wait a moment for processes to close
	#ifdef _WIN32
		Sleep(2000); // Windows - wait 2 seconds
	#else
		sleep(2); // Unix/Linux/macOS - wait 2 seconds
	#endif
	
	// Build command to run in background
	string command = "\"" + string(rvmPath) + "\"";
	command += " -b=cpc" + cpcModel;
	command += " -i \"" + dskFile + "\"";
	command += " -c='run\"" + fileToRun + "\\n'";
	
	#ifdef _WIN32
		// Windows: Use start command to run in background
		command = "start \"\" " + command;
	#else
		// Unix/Linux/macOS: Redirect output and run in background
		command += " > /dev/null 2>&1 &";
	#endif
	
	cout << "Launching RetroVirtualMachine..." << endl;
	cout << "Command: " << command << endl;
	
	// Execute command in background
	int result = system(command.c_str());
	
	if (result == 0) {
		cout << "RetroVirtualMachine launched successfully in background." << endl;
		cout << "Running file: " << fileToRun << " on CPC" << cpcModel << endl;
		return true;
	} else {
		cerr << "Error launching RetroVirtualMachine (exit code: " << result << ")" << endl;
		return false;
	}
}

// Function to check if argument contains CPC-style syntax (commas)
bool containsCPCStyle(const string& arg) {
	return arg.find(',') != string::npos;
}

// Function to parse CPC-style save syntax: "filename",B,load,exec or "filename",R etc.
bool parseCPCStyle(const string& arg, string& filename, int& type, int& loadAddr, int& execAddr, bool& readOnly, bool& systemFile) {
	// Reset values
	type = 0; // ASCII by default
	loadAddr = 0;
	execAddr = 0;
	readOnly = false;
	systemFile = false;
	
	// Split by commas
	vector<string> parts;
	stringstream ss(arg);
	string part;
	
	while (getline(ss, part, ',')) {
		// Trim whitespace
		part.erase(0, part.find_first_not_of(" \t"));
		part.erase(part.find_last_not_of(" \t") + 1);
		parts.push_back(part);
	}
	
	if (parts.empty()) return false;
	
	// First part is filename (remove quotes if present)
	filename = parts[0];
	if (filename.front() == '"' && filename.back() == '"') {
		filename = filename.substr(1, filename.length() - 2);
	}
	
	// Process additional parameters
	for (size_t i = 1; i < parts.size(); i++) {
		string param = parts[i];
		
		if (param == "A" || param == "a") {
			// ASCII/BASIC file (type 0)
			type = 0;
		}
		else if (param == "B" || param == "b") {
			// Binary file (type 1)
			type = 1;
		}
		else if (param == "R" || param == "r") {
			// Raw file (type 2 - no AMSDOS header)
			type = 2;
		}
		else if (param == "P" || param == "p") {
			// Protected (read-only attribute)
			readOnly = true;
		}
		else if (param == "S" || param == "s") {
			// System file attribute
			systemFile = true;
		}
		else if (param == "PS" || param == "SP" || param == "ps" || param == "sp") {
			// Protected + System
			readOnly = true;
			systemFile = true;
		}
		else {
			// Try to parse as hex address
			char* endptr;
			long addr = strtol(param.c_str(), &endptr, 16);
			if (*endptr == '\0' && addr >= 0) {
				if (loadAddr == 0) {
					loadAddr = addr;
				} else if (execAddr == 0) {
					execAddr = addr;
				}
			}
		}
	}
	
	return true;
}

// Function to get DSK file from arguments or environment variable
string GetDskFile(int argc, char **argv, int dskArgIndex, bool expectDskArg) {
	// First try to get DSK file from command line argument
	if (argc > dskArgIndex) {
		return string(argv[dskArgIndex]);
	}
	
	// No DSK argument provided, check environment variable
	const char* envDskFile = getenv("DISC_DSK_FILE");
	if (envDskFile != NULL && strlen(envDskFile) > 0) {
		return string(envDskFile);
	}
	
	// No DSK file found
	return "";
}

// Function to detect if a file is tokenized BASIC or ASCII text
bool IsTokenizedBasic(unsigned char* buffer, int fileSize, int loadAddress) {
	// If file is too small, treat as ASCII
	if (fileSize < 10) {
		return false;
	}
	
	// Check if load address is &0170 (368 decimal) - typical for tokenized BASIC
	bool hasBasicLoadAddress = (loadAddress == 0x0170);
	
	// If load address is NOT &0170, it's very unlikely to be BASIC
	if (!hasBasicLoadAddress) {
		return false;
	}
	
	// Look for BASIC line number structure at the beginning
	// Tokenized BASIC files start with line numbers in little-endian format
	bool hasBasicStructure = false;
	if (fileSize >= 4) {
		// Check if first bytes look like BASIC line structure
		// Format: [line_length_low] [line_length_high] [line_number_low] [line_number_high]
		int lineLength = buffer[0] + (buffer[1] << 8);
		int lineNumber = buffer[2] + (buffer[3] << 8);
		
		// Valid BASIC line: reasonable length (< 255) and line number (1-65535)
		if (lineLength > 0 && lineLength < 255 && lineNumber > 0 && lineNumber < 65536) {
			hasBasicStructure = true;
		}
	}
	
	// Count BASIC tokens and printable characters
	int tokenCount = 0;
	int printableCount = 0;
	int controlCount = 0;
	
	// Analyze first 100 bytes or entire file if smaller
	int bytesToCheck = (fileSize < 100) ? fileSize : 100;
	
	for (int i = 0; i < bytesToCheck; i++) {
		unsigned char byte = buffer[i];
		
		// Count BASIC tokens (0x80-0xFF range)
		if (byte >= 0x80) {
			tokenCount++;
		}
		// Count printable ASCII characters (0x20-0x7E)
		else if (byte >= 0x20 && byte <= 0x7E) {
			printableCount++;
		}
		// Count control characters and nulls
		else if (byte < 0x20) {
			controlCount++;
		}
	}
	
	// Must have BASIC load address to be considered BASIC
	if (!hasBasicLoadAddress) {
		return false;
	}
	
	// If has BASIC load address and structure, very likely BASIC
	if (hasBasicLoadAddress && hasBasicStructure) {
		return true;
	}
	
	// If has BASIC load address and some tokens, likely BASIC
	if (hasBasicLoadAddress && tokenCount > 0) {
		return true;
	}
	
	// Default to BASIC if has correct load address (since we already checked above)
	return true;
}

int main(int argc, char **argv)
{
	bool IsDskLoc, IsDskSet,
		ModeListDsk, ModeImportFile,
		ModeRemoveFile,
		ModeDisaFile, ModeListBasic,
		ModeListDams, ModeListHex,
		ModeGetFile, ModeNewDsk, Force_Overwrite,
		Read_only, System_file, Split_lines, ModeListAscii, ModeListFile, ModeRunFile, NoOptionSet;

	ModeListDsk = ModeImportFile = ModeListAscii =
		ModeRemoveFile = ModeDisaFile =
			ModeListBasic = ModeListDams = ModeListHex = ModeNewDsk =
				ModeGetFile = IsDskLoc = IsDskSet = Force_Overwrite = Read_only = System_file = ModeListFile = ModeRunFile = false;
	NoOptionSet = true;			

	string DskFile, AmsdosFile, Command;
	vector<string> AmsdosFileList;

	int exeAdress = 0, loadAdress = 0, AmsdosType = 1, UserNumber = 0;

	DSK MyDsk;

	IsDsk = IsDskValid = false;
	IsDskSaved = true;

	// Initialize colors and check for --no-color flag
	#ifdef _WIN32
		enableWindowsColors();
	#endif
	
	// Check for color control options
	const char* noColor = getenv("NO_COLOR");
	const char* discNoColor = getenv("DISC_NO_COLOR");
	
	if (noColor != NULL || discNoColor != NULL) {
		useColors = false;
	}
	
	// Check for --no-color flag
	for (int i = 1; i < argc; i++) {
		if (string(argv[i]) == "--no-color") {
			useColors = false;
			break;
		}
	}

	// Parse command line arguments manually
	if (argc < 2) {
		cerr << colorize("\nYou must specify a command!\n", COLOR_ERROR) << endl;
		help();
		return EXIT_FAILURE;
	}

	Command = argv[1];
	
	// Helper function to check for --help in arguments
	auto hasHelpFlag = [&]() -> bool {
		for (int i = 2; i < argc; i++) {
			string arg = argv[i];
			if (arg == "--help" || arg == "-h") {
				return true;
			}
		}
		return false;
	};
	
	// Handle commands that don't need a DSK file
	if (Command == "help" || Command == "--help" || Command == "-h") {
		help();
		return 0;
	}
	
	// Check for command-specific help
	if (hasHelpFlag()) {
		if (Command == "cat") {
			help_cat();
			return 0;
		} else if (Command == "new") {
			help_new();
			return 0;
		} else if (Command == "save" || Command == "import") {
			help_save();
			return 0;
		} else if (Command == "era" || Command == "rm") {
			help_era();
			return 0;
		} else if (Command == "get" || Command == "export") {
			help_get();
			return 0;
		} else if (Command == "list") {
			help_list();
			return 0;
		} else if (Command == "basic") {
			help_basic();
			return 0;
		} else if (Command == "dams") {
			help_dams();
			return 0;
		} else if (Command == "disasm" || Command == "disassemble") {
			help_disasm();
			return 0;
		} else if (Command == "ascii") {
			help_ascii();
			return 0;
		} else if (Command == "hex") {
			help_hex();
			return 0;
		} else if (Command == "run") {
			help_run();
			return 0;
		}
	}
	
	if (Command == "new") {
		if (argc < 3) {
			cerr << "Usage: " << PROGNAME << " new <dskfile>" << endl;
			return EXIT_FAILURE;
		}
		DskFile = argv[2];
		ModeNewDsk = true;
		IsDskSet = true;
		NoOptionSet = false;
	}
	else {
		// Commands that need a DSK file
		// For cat command, always try environment variable first
		bool expectDskArg = true; // Cat command should accept DSK file as argument
		string dskFile = GetDskFile(argc, argv, 2, expectDskArg);
		if (dskFile.empty()) {
			cerr << "Usage: " << PROGNAME << " " << Command << " <dskfile> [options]" << endl;
			cerr << "Or set DISC_DSK_FILE environment variable" << endl;
			return EXIT_FAILURE;
		}
		
		DskFile = dskFile;
		IsDskSet = true;
		
		if (Command == "cat") {
			ModeListDsk = true;
			// Don't set NoOptionSet = false here, let the default listing logic handle it
		}
		else if (Command == "list") {
			// Determine if DSK is provided or use environment variable
			const char* envDskFile = getenv("DISC_DSK_FILE");
			bool hasEnvDsk = (envDskFile != NULL && strlen(envDskFile) > 0);
			
			// If we have 3 args (prog list file), use env var
			// If we have 4+ args (prog list dsk file), use arg
			bool hasDskArg = (argc >= 4);
			int fileArgIndex = hasDskArg ? 3 : 2;
			
			if (argc < 3) {
				cerr << "Usage: " << PROGNAME << " list <dskfile> <file> [-p]" << endl;
				cerr << "Or set DISC_DSK_FILE environment variable and use: " << PROGNAME << " list <file> [-p]" << endl;
				return EXIT_FAILURE;
			}
			
			// Get DSK file
			string dskFile;
			if (hasDskArg) {
				dskFile = string(argv[2]);
			} else if (hasEnvDsk) {
				dskFile = string(envDskFile);
			} else {
				cerr << "Error: No DSK file specified and DISC_DSK_FILE not set" << endl;
				cerr << "Usage: " << PROGNAME << " list <dskfile> <file> [-p]" << endl;
				cerr << "Or set DISC_DSK_FILE environment variable and use: " << PROGNAME << " list <file> [-p]" << endl;
				return EXIT_FAILURE;
			}
			DskFile = dskFile;
			
			ModeListFile = true;
			NoOptionSet = false;
			AmsdosFileList.push_back(argv[fileArgIndex]);
			// Check for -p option
			for (int i = fileArgIndex + 1; i < argc; i++) {
				if (string(argv[i]) == "-p") {
					Split_lines = true;
				}
			}
		}
		else if (Command == "save" || Command == "import") {
			// Check if DISC_DSK_FILE environment variable is set
			const char* envDskFile = getenv("DISC_DSK_FILE");
			bool hasEnvDsk = (envDskFile != NULL && strlen(envDskFile) > 0);
			
			// Determine if DSK file is provided as argument
			// If env var exists and argc == 3, then argv[2] is the file to save
			// If env var doesn't exist and argc >= 4, then argv[2] is DSK and argv[3] is file
			bool hasDskArg = !hasEnvDsk && (argc >= 4);
			int fileArgIndex = hasDskArg ? 3 : 2;
			
			// Minimum arguments check
			int minArgs = hasEnvDsk ? 3 : 4; // save <file> OR save <dsk> <file>
			if (argc < minArgs) {
				cerr << "Usage: " << PROGNAME << " save <dskfile> <file> [options]" << endl;
				cerr << "Or set DISC_DSK_FILE environment variable and use: " << PROGNAME << " save <file> [options]" << endl;
				cerr << "CPC style: " << PROGNAME << " save \"file.bin\",B,load,exec" << endl;
				return EXIT_FAILURE;
			}
			
			// Get DSK file
			string dskFile;
			if (hasEnvDsk) {
				dskFile = string(envDskFile);
			} else if (argc >= 4) {
				dskFile = string(argv[2]);
			} else {
				cerr << "Error: No DSK file specified and DISC_DSK_FILE not set" << endl;
				cerr << "Usage: " << PROGNAME << " save <dskfile> <file> [options]" << endl;
				cerr << "Or set DISC_DSK_FILE environment variable and use: " << PROGNAME << " save <file> [options]" << endl;
				return EXIT_FAILURE;
			}
			DskFile = dskFile;
			
			ModeImportFile = true;
			NoOptionSet = false;
			
			// Check if using CPC-style syntax
			string firstFileArg = argv[fileArgIndex];
			if (containsCPCStyle(firstFileArg)) {
				// Parse CPC-style syntax: "filename",B,load,exec
				string filename;
				int cpcType, cpcLoadAddr, cpcExecAddr;
				bool cpcReadOnly, cpcSystemFile;
				
				if (parseCPCStyle(firstFileArg, filename, cpcType, cpcLoadAddr, cpcExecAddr, cpcReadOnly, cpcSystemFile)) {
					// Apply parsed values
					AmsdosType = cpcType;
					loadAdress = cpcLoadAddr;
					exeAdress = cpcExecAddr;
					Read_only = cpcReadOnly;
					System_file = cpcSystemFile;
					AmsdosFileList.push_back(filename);
				} else {
					cerr << "Error parsing CPC-style syntax: " << firstFileArg << endl;
					return EXIT_FAILURE;
				}
			} else {
				// Use traditional flag-based parsing
				// Track if we've added the first file (to avoid adding multiple files)
				bool firstFileAdded = false;
				
				for (int i = fileArgIndex; i < argc; i++) {
					string arg = argv[i];
					
					if (arg == "-t" && i + 1 < argc) {
						AmsdosType = atoi(argv[++i]);
					}
					else if (arg == "-e" && i + 1 < argc) {
						exeAdress = strtol(argv[++i], NULL, 16);
					}
					else if (arg == "-c" && i + 1 < argc) {
						loadAdress = strtol(argv[++i], NULL, 16);
					}
					else if (arg == "-f") {
						Force_Overwrite = true;
					}
					else if (arg == "-o") {
						Read_only = true;
					}
					else if (arg == "-s") {
						System_file = true;
					}
					else if (arg == "-u" && i + 1 < argc) {
						UserNumber = atoi(argv[++i]);
					}
					else if (arg[0] != '-') {
						// Add the file (should be the first non-flag argument after fileArgIndex)
						if (!firstFileAdded) {
							AmsdosFileList.push_back(arg);
							firstFileAdded = true;
						}
					}
				}
			}
		}
		else if (Command == "era" || Command == "rm") {
			// Determine if DSK is provided or use environment variable
			const char* envDskFile = getenv("DISC_DSK_FILE");
			bool hasEnvDsk = (envDskFile != NULL && strlen(envDskFile) > 0);
			
			bool hasDskArg = (argc >= 4);
			int fileArgIndex = hasDskArg ? 3 : 2;
			
			if (argc < 3) {
				cerr << "Usage: " << PROGNAME << " era <dskfile> <file>" << endl;
				cerr << "Or set DISC_DSK_FILE environment variable and use: " << PROGNAME << " era <file>" << endl;
				return EXIT_FAILURE;
			}
			
			// Get DSK file
			string dskFile;
			if (hasDskArg) {
				dskFile = string(argv[2]);
			} else if (hasEnvDsk) {
				dskFile = string(envDskFile);
			} else {
				cerr << "Error: No DSK file specified and DISC_DSK_FILE not set" << endl;
				cerr << "Usage: " << PROGNAME << " era <dskfile> <file>" << endl;
				cerr << "Or set DISC_DSK_FILE environment variable and use: " << PROGNAME << " era <file>" << endl;
				return EXIT_FAILURE;
			}
			DskFile = dskFile;
			
			ModeRemoveFile = true;
			NoOptionSet = false;
			for (int i = fileArgIndex; i < argc; i++) {
				AmsdosFileList.push_back(argv[i]);
			}
		}
		else if (Command == "get" || Command == "export") {
			// Determine if DSK is provided or use environment variable
			const char* envDskFile = getenv("DISC_DSK_FILE");
			bool hasEnvDsk = (envDskFile != NULL && strlen(envDskFile) > 0);
			
			bool hasDskArg = (argc >= 4);
			int fileArgIndex = hasDskArg ? 3 : 2;
			
			if (argc < 3) {
				cerr << "Usage: " << PROGNAME << " get <dskfile> <file>" << endl;
				cerr << "Or set DISC_DSK_FILE environment variable and use: " << PROGNAME << " get <file>" << endl;
				return EXIT_FAILURE;
			}
			
			// Get DSK file
			string dskFile;
			if (hasDskArg) {
				dskFile = string(argv[2]);
			} else if (hasEnvDsk) {
				dskFile = string(envDskFile);
			} else {
				cerr << "Error: No DSK file specified and DISC_DSK_FILE not set" << endl;
				cerr << "Usage: " << PROGNAME << " get <dskfile> <file>" << endl;
				cerr << "Or set DISC_DSK_FILE environment variable and use: " << PROGNAME << " get <file>" << endl;
				return EXIT_FAILURE;
			}
			DskFile = dskFile;
			
			ModeGetFile = true;
			NoOptionSet = false;
			for (int i = fileArgIndex; i < argc; i++) {
				AmsdosFileList.push_back(argv[i]);
			}
		}
		else if (Command == "basic") {
			// Determine if DSK is provided or use environment variable
			const char* envDskFile = getenv("DISC_DSK_FILE");
			bool hasEnvDsk = (envDskFile != NULL && strlen(envDskFile) > 0);
			
			bool hasDskArg = (argc >= 4);
			int fileArgIndex = hasDskArg ? 3 : 2;
			
			if (argc < 3) {
				cerr << "Usage: " << PROGNAME << " basic <dskfile> <file> [-p]" << endl;
				cerr << "Or set DISC_DSK_FILE environment variable and use: " << PROGNAME << " basic <file> [-p]" << endl;
				return EXIT_FAILURE;
			}
			
			// Get DSK file
			string dskFile;
			if (hasDskArg) {
				dskFile = string(argv[2]);
			} else if (hasEnvDsk) {
				dskFile = string(envDskFile);
			} else {
				cerr << "Error: No DSK file specified and DISC_DSK_FILE not set" << endl;
				cerr << "Usage: " << PROGNAME << " basic <dskfile> <file> [-p]" << endl;
				cerr << "Or set DISC_DSK_FILE environment variable and use: " << PROGNAME << " basic <file> [-p]" << endl;
				return EXIT_FAILURE;
			}
			DskFile = dskFile;
			
			ModeListBasic = true;
			NoOptionSet = false;
			AmsdosFileList.push_back(argv[fileArgIndex]);
			// Check for -p option
			for (int i = fileArgIndex + 1; i < argc; i++) {
				if (string(argv[i]) == "-p") {
					Split_lines = true;
				}
			}
		}
		else if (Command == "dams") {
			// Determine if DSK is provided or use environment variable
			const char* envDskFile = getenv("DISC_DSK_FILE");
			bool hasEnvDsk = (envDskFile != NULL && strlen(envDskFile) > 0);
			
			bool hasDskArg = (argc >= 4);
			int fileArgIndex = hasDskArg ? 3 : 2;
			
			if (argc < 3) {
				cerr << "Usage: " << PROGNAME << " dams <dskfile> <file>" << endl;
				cerr << "Or set DISC_DSK_FILE environment variable and use: " << PROGNAME << " dams <file>" << endl;
				return EXIT_FAILURE;
			}
			
			// Get DSK file
			string dskFile;
			if (hasDskArg) {
				dskFile = string(argv[2]);
			} else if (hasEnvDsk) {
				dskFile = string(envDskFile);
			} else {
				cerr << "Error: No DSK file specified and DISC_DSK_FILE not set" << endl;
				cerr << "Usage: " << PROGNAME << " dams <dskfile> <file>" << endl;
				cerr << "Or set DISC_DSK_FILE environment variable and use: " << PROGNAME << " dams <file>" << endl;
				return EXIT_FAILURE;
			}
			DskFile = dskFile;
			
			ModeListDams = true;
			NoOptionSet = false;
			AmsdosFileList.push_back(argv[fileArgIndex]);
		}
		else if (Command == "disasm" || Command == "disassemble") {
			// Determine if DSK is provided or use environment variable
			const char* envDskFile = getenv("DISC_DSK_FILE");
			bool hasEnvDsk = (envDskFile != NULL && strlen(envDskFile) > 0);
			
			bool hasDskArg = (argc >= 4);
			int fileArgIndex = hasDskArg ? 3 : 2;
			
			if (argc < 3) {
				cerr << "Usage: " << PROGNAME << " disasm <dskfile> <file>" << endl;
				cerr << "Or set DISC_DSK_FILE environment variable and use: " << PROGNAME << " disasm <file>" << endl;
				return EXIT_FAILURE;
			}
			
			// Get DSK file
			string dskFile;
			if (hasDskArg) {
				dskFile = string(argv[2]);
			} else if (hasEnvDsk) {
				dskFile = string(envDskFile);
			} else {
				cerr << "Error: No DSK file specified and DISC_DSK_FILE not set" << endl;
				cerr << "Usage: " << PROGNAME << " disasm <dskfile> <file>" << endl;
				cerr << "Or set DISC_DSK_FILE environment variable and use: " << PROGNAME << " disasm <file>" << endl;
				return EXIT_FAILURE;
			}
			DskFile = dskFile;
			
			ModeDisaFile = true;
			NoOptionSet = false;
			AmsdosFileList.push_back(argv[fileArgIndex]);
		}
		else if (Command == "ascii") {
			// Determine if DSK is provided or use environment variable
			const char* envDskFile = getenv("DISC_DSK_FILE");
			bool hasEnvDsk = (envDskFile != NULL && strlen(envDskFile) > 0);
			
			bool hasDskArg = (argc >= 4);
			int fileArgIndex = hasDskArg ? 3 : 2;
			
			if (argc < 3) {
				cerr << "Usage: " << PROGNAME << " ascii <dskfile> <file>" << endl;
				cerr << "Or set DISC_DSK_FILE environment variable and use: " << PROGNAME << " ascii <file>" << endl;
				return EXIT_FAILURE;
			}
			
			// Get DSK file
			string dskFile;
			if (hasDskArg) {
				dskFile = string(argv[2]);
			} else if (hasEnvDsk) {
				dskFile = string(envDskFile);
			} else {
				cerr << "Error: No DSK file specified and DISC_DSK_FILE not set" << endl;
				cerr << "Usage: " << PROGNAME << " ascii <dskfile> <file>" << endl;
				cerr << "Or set DISC_DSK_FILE environment variable and use: " << PROGNAME << " ascii <file>" << endl;
				return EXIT_FAILURE;
			}
			DskFile = dskFile;
			
			ModeListAscii = true;
			NoOptionSet = false;
			AmsdosFileList.push_back(argv[fileArgIndex]);
		}
		else if (Command == "run") {
			// Determine if DSK is provided or use environment variable
			const char* envDskFile = getenv("DISC_DSK_FILE");
			bool hasEnvDsk = (envDskFile != NULL && strlen(envDskFile) > 0);
			
			bool hasDskArg = (argc >= 4);
			int fileArgIndex = hasDskArg ? 3 : 2;
			
			if (argc < 3) {
				cerr << "Usage: " << PROGNAME << " run <dskfile> <file>" << endl;
				cerr << "Or set DISC_DSK_FILE environment variable and use: " << PROGNAME << " run <file>" << endl;
				cerr << "Set CPC_MODEL environment variable to specify CPC model (464, 664, 6128) - default: 6128" << endl;
				return EXIT_FAILURE;
			}
			
			// Get DSK file
			string dskFile;
			if (hasDskArg) {
				dskFile = string(argv[2]);
			} else if (hasEnvDsk) {
				dskFile = string(envDskFile);
			} else {
				cerr << "Error: No DSK file specified and DISC_DSK_FILE not set" << endl;
				cerr << "Usage: " << PROGNAME << " run <dskfile> <file>" << endl;
				cerr << "Or set DISC_DSK_FILE environment variable and use: " << PROGNAME << " run <file>" << endl;
				return EXIT_FAILURE;
			}
			DskFile = dskFile;
			
			ModeRunFile = true;
			NoOptionSet = false;
			AmsdosFileList.push_back(argv[fileArgIndex]);
		}
		else if (Command == "hex") {
			// Determine if DSK is provided or use environment variable
			const char* envDskFile = getenv("DISC_DSK_FILE");
			bool hasEnvDsk = (envDskFile != NULL && strlen(envDskFile) > 0);
			
			bool hasDskArg = (argc >= 4);
			int fileArgIndex = hasDskArg ? 3 : 2;
			
			if (argc < 3) {
				cerr << "Usage: " << PROGNAME << " hex <dskfile> <file>" << endl;
				cerr << "Or set DISC_DSK_FILE environment variable and use: " << PROGNAME << " hex <file>" << endl;
				return EXIT_FAILURE;
			}
			
			// Get DSK file
			string dskFile;
			if (hasDskArg) {
				dskFile = string(argv[2]);
			} else if (hasEnvDsk) {
				dskFile = string(envDskFile);
			} else {
				cerr << "Error: No DSK file specified and DISC_DSK_FILE not set" << endl;
				cerr << "Usage: " << PROGNAME << " hex <dskfile> <file>" << endl;
				cerr << "Or set DISC_DSK_FILE environment variable and use: " << PROGNAME << " hex <file>" << endl;
				return EXIT_FAILURE;
			}
			DskFile = dskFile;
			
			ModeListHex = true;
			NoOptionSet = false;
			AmsdosFileList.push_back(argv[fileArgIndex]);
		}
		else {
			cerr << "\nUnknown command: " << Command << "\n" << endl;
			help();
			return EXIT_FAILURE;
		}
	}

	if (!IsDskSet)
	{
		cerr << "\nYou did not select a DSK file to work with !\n" << endl;
		help();
	}
	else {
		// Only show Drive A: for commands that are not save/import/era/new
		if (!ModeImportFile && !ModeRemoveFile && !ModeNewDsk) {
			cerr << "\nDrive A: " << DskFile << endl;
		}
	}

	if (ModeListBasic || ModeListHex || ModeListDams || ModeDisaFile || ModeListAscii || ModeListFile)
	{
		NoOptionSet = false;
		if (!MyDsk.ReadDsk(DskFile))
		{
			cerr << "\nError reading file (" << DskFile << ").\n" << endl;
			exit(EXIT_FAILURE);
		}
		if (!MyDsk.CheckDsk())
		{
			cerr << "\nUnsupported image file (" << DskFile << ").\n" << endl;
			exit(EXIT_FAILURE);
		}
		int Indice;
		for (vector<string>::iterator iter = AmsdosFileList.begin(); iter != AmsdosFileList.end(); iter++)
		{
			char *amsdosF = GetNomAmsdos(iter->c_str());
			if (ModeListFile) {
				cerr << "File: " << amsdosF;
			} else {
				cerr << "File: " << amsdosF << endl;
			}
			if ((Indice = MyDsk.FileIsIn(amsdosF)) < 0)
			{
				cerr << "\nFile " << amsdosF << " not found.\n" << endl;
				exit(EXIT_FAILURE);
			}
			MyDsk.OnViewFic(Indice);

			if (ModeListBasic)
				cout << ViewBasic(Split_lines) << endl;
			else if (ModeListDams)
				cout << "\nNot yet coded ! Please try a newer version of disc ! Sorry !\n" << endl;
			else if (ModeListHex)
			{
				cerr << endl;
				MyDsk.Hexdecimal();
				cout << Listing << endl;
			}
			else if (ModeDisaFile)
				cout << ViewDesass() << endl;
			else if (ModeListAscii)
				cout << ViewAscii() << endl;
			else if (ModeListFile)
			{
				// Auto-detect file type and display accordingly
				if (IsTokenizedBasic(BufFile, TailleFic, AdresseCharg))
				{
					cerr << " (Tokenized BASIC file)" << endl;
					cerr << "Size: " << TailleFic << endl;
					cerr << endl;
					// Call ViewBasic but suppress its "File size" output by redirecting stderr temporarily
					Basic(BufFile, Listing, true, Split_lines);
					cout << Listing << endl;
				}
				else
				{
					cerr << " (ASCII text file)" << endl;
					cerr << "Size: " << TailleFic << endl;
					cerr << endl;
					// Call ViewAscii but suppress its "File size" output by redirecting stderr temporarily
					Ascii(BufFile, Listing, TailleFic);
					cout << Listing << endl;
				}
			}
		}
	}

	if (ModeNewDsk)
	{
		NoOptionSet = false;
		MyDsk.FormatDsk(9, 42);
		if (!MyDsk.WriteDsk(DskFile))
		{
			cerr << "\nError writing file " << DskFile << "\n" << endl;
			exit(EXIT_FAILURE);
		}
		cerr << "\nDisk image " << DskFile << " created successfully." << endl;
		cerr << endl;
		cerr << "Drive A: " << DskFile << endl;
		cout << MyDsk.ReadDskDir();
	}

	

	if (ModeImportFile)
	{ // Add files to dsk
		NoOptionSet = false;
		if (!MyDsk.ReadDsk(DskFile))
		{
			cerr << "\nError reading file (" << DskFile << ").\n" << endl;
			exit(EXIT_FAILURE);
		}

		if (!MyDsk.CheckDsk())
		{
			cerr << "\nUnsupported dsk file (" << DskFile << ").\n" << endl;
			exit(EXIT_FAILURE);
		}

		for (vector<string>::iterator iter = AmsdosFileList.begin(); iter != AmsdosFileList.end(); iter++)
		{
			string amsdosfile = GetNomAmsdos(iter->c_str());
			int Indice;
			// Ensure Indice is valid (the file is in the disk already) before atempting to remove it
			if ((Indice = MyDsk.FileIsIn(amsdosfile)) != -1)
			{
				if (!Force_Overwrite)
				{
					cerr << "\n(" << amsdosfile << ") File exists, replace ? (Y/N) (try -f switch for autoreplace...):";
					string answer;
					cin >> answer;
					if (toupper(answer[0]) == 'Y')
						MyDsk.RemoveFile(Indice);
					else
					{
						cerr << "Import cancelled, dsk unchanged." << endl;
						cout << MyDsk.ReadDskDir();
						exit(EXIT_SUCCESS);
					}
				}
				else
					MyDsk.RemoveFile(Indice);
			}
			cerr << "\nFile " << *iter << " added successfully." << endl;

			MyDsk.PutFileInDsk(*iter, AmsdosType, loadAdress, exeAdress, UserNumber, System_file, Read_only);
		}
		if (MyDsk.WriteDsk(DskFile)) {
			cerr << endl;
			cerr << "Drive A: " << DskFile << endl;
			cout << MyDsk.ReadDskDir();
		}
		else
			cerr << "\nError writing file : " << DskFile << endl;
	}

	if (ModeRemoveFile)
	{
		NoOptionSet = false;
		if (!MyDsk.ReadDsk((char *)DskFile.c_str()))
		{
			cerr << "\nError reading file (" << DskFile << ")." << endl;
			exit(EXIT_FAILURE);
		}
		if (!MyDsk.CheckDsk())
		{
			cerr << "\nunsupported DSK file (" << DskFile << ")." << endl;
			exit(EXIT_FAILURE);
		}
		int Indice;
		for (vector<string>::iterator iter = AmsdosFileList.begin(); iter != AmsdosFileList.end(); iter++)
		{
			char *amsdosF = GetNomAmsdos(iter->c_str());
			
			if ((Indice = MyDsk.FileIsIn(amsdosF)) < 0)
			{
				// Check if file exists but is protected (system or read-only)
				bool fileExistsAsProtected = false;
				
				// Search through all directory entries (including protected files)
				for (int i = 0; i < 64; i++) {
					StDirEntry * Dir = MyDsk.GetInfoDirEntry(i);
					if (Dir->User == USER_DELETED) continue;
					
					// Create clean name for comparison (remove attribute bits)
					char cleanName[12];
					memcpy(cleanName, Dir->Nom, 8);
					memcpy(&cleanName[8], Dir->Ext, 3);
					for(int q = 0; q < 11; q++) {
						cleanName[q] = cleanName[q] & 127;
					}
					cleanName[11] = 0;
					
					// Create target name in same format
					StDirEntry * targetEntry = MyDsk.GetNomDir(*iter);
					char targetName[12];
					memcpy(targetName, targetEntry->Nom, 11);
					targetName[11] = 0;
					
					if (!memcmp(cleanName, targetName, 11)) {
						bool isSystemFile = (Dir->Ext[1] & 0x80) != 0;
						bool isReadOnlyFile = (Dir->Ext[0] & 0x80) != 0;
						
						if (isSystemFile) {
							cerr << "\nSystem file cannot be deleted: " << amsdosF << "\n" << endl;
							fileExistsAsProtected = true;
						} else if (isReadOnlyFile) {
							cerr << "\nRead-only file cannot be deleted: " << amsdosF << "\n" << endl;
							fileExistsAsProtected = true;
						}
						break;
					}
				}
				
				if (!fileExistsAsProtected) {
					cerr << "\nFile " << amsdosF << " not found.\n" << endl;
				}
				exit(EXIT_FAILURE);
			}
			MyDsk.RemoveFile(Indice);
			if (MyDsk.WriteDsk((char *)DskFile.c_str())) {
				cerr << "\nFile " << *iter << " deleted successfully." << endl;
				cerr << endl;
				cerr << "Drive A: " << DskFile << endl;
				cout << MyDsk.ReadDskDir();
			}
			else
				cerr << "Error writing file " << (*iter) << endl;
		}
	}

	if (ModeGetFile)
	{
		NoOptionSet = false;
		if (!MyDsk.ReadDsk((char *)DskFile.c_str()))
		{
			cerr << "Error reading dskfile (" << DskFile << ")." << endl;
			exit(EXIT_FAILURE);
		}
		if (!MyDsk.CheckDsk())
		{
			cerr << "Unsupported dsk (" << DskFile << ")." << endl;
			exit(EXIT_FAILURE);
		}
		int Indice;

		for (vector<string>::iterator iter = AmsdosFileList.begin(); iter != AmsdosFileList.end(); iter++)
		{
			char *amsdosF = GetNomAmsdos(iter->c_str());
			if ((Indice = MyDsk.FileIsIn(amsdosF)) < 0)
			{
				cerr << "\nFile " << amsdosF << " not found.\n" << endl;
				exit(EXIT_FAILURE);
			}
			if (!MyDsk.GetFileInDsk((char *)(*iter).c_str(), Indice))
			{
				cerr << "System error : unable to copy (" << AmsdosFile << ")." << endl;
				exit(EXIT_FAILURE);
			}
			cerr << "File " << *iter << " extracted successfully." << endl;
		}
	}

	if (ModeRunFile)
	{
		NoOptionSet = false;
		if (!MyDsk.ReadDsk(DskFile))
		{
			cerr << "Error reading file (" << DskFile << ")." << endl;
			exit(EXIT_FAILURE);
		}
		if (!MyDsk.CheckDsk())
		{
			cerr << "Unsupported image file (" << DskFile << ")." << endl;
			exit(EXIT_FAILURE);
		}
		
		// Check if file exists in DSK
		for (vector<string>::iterator iter = AmsdosFileList.begin(); iter != AmsdosFileList.end(); iter++)
		{
			char *amsdosF = GetNomAmsdos(iter->c_str());
			int Indice = MyDsk.FileIsIn(amsdosF);
			if (Indice < 0)
			{
				cerr << "Error: File " << amsdosF << " not found in DSK." << endl;
				exit(EXIT_FAILURE);
			}
			
			// Execute RetroVirtualMachine
			if (!executeRetroVirtualMachine(DskFile, *iter))
			{
				exit(EXIT_FAILURE);
			}
		}
	}
	
	if (ModeListDsk || NoOptionSet)
	{ // lire Dsk
		if (!MyDsk.ReadDsk(DskFile))
		{
			cerr << "Error reading file (" << DskFile << ")." << endl;
			exit(EXIT_FAILURE);
		}
		if (!MyDsk.CheckDsk())
		{
			cerr << "Unsupported dsk file (" << DskFile << ")." << endl;
			exit(EXIT_FAILURE);
		}
		cout << MyDsk.ReadDskDir();
	}
	
	cerr << endl;

	return (EXIT_SUCCESS);
}

void help(void)
{
	cout << endl;
	cout << colorize("▗ ▛▀▖▞▀▖▌ ▌▞▀▖▞▀▖", COLOR_BRIGHT_YELLOW) << endl;
	cout << colorize("▄ ▌ ▌▚▄ ▙▞  ▗▘▌▞▌", COLOR_BRIGHT_YELLOW) << endl;
	cout << colorize("▐ ▌ ▌▖ ▌▌▝▖▗▘ ▛ ▌", COLOR_BRIGHT_YELLOW) << endl;
	cout << colorize("▀▘▀▀ ▝▀ ▘ ▘▀▀▘▝▀ ", COLOR_BRIGHT_YELLOW) << endl;              
	cout << colorize(VERSION, COLOR_BRIGHT_GREEN) << endl;
    cout << colorize("by Destroyer 2025", COLOR_YELLOW) << endl;
	cout << colorize("https://github.com/destroyer.dcf/iDSK20", COLOR_YELLOW) << endl;
    cout << colorize("License: MIT", COLOR_YELLOW) << endl;
	cout << endl;
	cout << colorize("iDSK20 is a fork of idsk created by:)", COLOR_DIM) << endl;
	cout << colorize("  Jérôme Le Saux (Sid), Colin Pitrat, Thomas Bernard (miniupnp)", COLOR_DIM) << endl;
	cout << colorize("  Romain Giot (Krusty) and Adrien Destugues (PulkoMandy)", COLOR_DIM) << endl;
	cout << endl;
	cout << colorize("Usage: ", COLOR_BOLD) << colorize(PROGNAME, COLOR_FILENAME) << " <command> [arguments]" << endl;
	cout << endl;
	cout << colorize("COMMANDS:", COLOR_HEADER) << endl;
	cout << "  " << colorize("cat", COLOR_BRIGHT_CYAN) << " <dskfile>                    List disk catalog" << endl;
	cout << "  " << colorize("new", COLOR_BRIGHT_CYAN) << " <dskfile>                    Create new DSK file" << endl;
	cout << "  " << colorize("save", COLOR_BRIGHT_CYAN) << " <dskfile> <file> [options]  Import file to DSK" << endl;
	cout << "  " << colorize("era", COLOR_BRIGHT_CYAN) << " <dskfile> <file>             Remove file from DSK" << endl;
	cout << "  " << colorize("get", COLOR_BRIGHT_CYAN) << " <dskfile> <file>             Export file from DSK" << endl;
	cout << "  " << colorize("list", COLOR_BRIGHT_CYAN) << " <dskfile> <file> [-p]       Auto-detect and show file content" << endl;
	cout << "  " << colorize("dams", COLOR_BRIGHT_CYAN) << " <dskfile> <file>            Show DAMS file content" << endl;
	cout << "  " << colorize("disasm", COLOR_BRIGHT_CYAN) << " <dskfile> <file>          Disassemble binary file" << endl;
	cout << "  " << colorize("hex", COLOR_BRIGHT_CYAN) << " <dskfile> <file>             Show file in hexadecimal" << endl;
	cout << "  " << colorize("run", COLOR_BRIGHT_CYAN) << " <dskfile> <file>             Run file in RetroVirtualMachine" << endl;
	cout << "  " << colorize("help", COLOR_BRIGHT_CYAN) << "                             Show this help" << endl;
	cout << endl;
	cout << colorize("OPTIONS for 'save' command:", COLOR_HEADER) << endl;
	cout << "  " << colorize("-t <type>", COLOR_YELLOW) << "     File type (0=ASCII, 1=BINARY, 2=raw)" << endl;
	cout << "  " << colorize("-e <address>", COLOR_YELLOW) << "  Execute address (hex)" << endl;
	cout << "  " << colorize("-c <address>", COLOR_YELLOW) << "  Load address (hex)" << endl;
	cout << "  " << colorize("-f", COLOR_YELLOW) << "            Force overwrite if file exists" << endl;
	cout << "  " << colorize("-o", COLOR_YELLOW) << "            Mark file as read-only" << endl;
	cout << "  " << colorize("-s", COLOR_YELLOW) << "            Mark file as system file" << endl;
	cout << "  " << colorize("-u <number>", COLOR_YELLOW) << "   Set user number" << endl;
	cout << endl;
	cout << colorize("CPC SYNTAX for 'save' command (like Amstrad SAVE):", COLOR_HEADER) << endl;
	cout << "  " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("save", COLOR_BRIGHT_CYAN) << " <dskfile> " << colorize("\"<file>\"", COLOR_YELLOW) << colorize("[,type][,load,exec][,attributes]", COLOR_DIM) << endl;
	cout << endl;
	cout << "  " << colorize("File Types:", COLOR_BOLD) << endl;
	cout << "    " << colorize("A", COLOR_YELLOW) << " = ASCII/BASIC (type 0, with AMSDOS header)" << endl;
	cout << "    " << colorize("B", COLOR_YELLOW) << " = Binary (type 1, with AMSDOS header + addresses)" << endl;
	cout << "    " << colorize("R", COLOR_YELLOW) << " = Raw (type 2, no AMSDOS header)" << endl;
	cout << endl;
	cout << "  " << colorize("Attributes (optional):", COLOR_BOLD) << endl;
	cout << "    " << colorize("P", COLOR_YELLOW) << " = Protected (read-only)" << endl;
	cout << "    " << colorize("S", COLOR_YELLOW) << " = System file" << endl;
	cout << endl;
	cout << "  " << colorize("Examples:", COLOR_BOLD) << endl;
	cout << "    " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("save", COLOR_BRIGHT_CYAN) << " floppy.dsk " << colorize("\"program.bas\"", COLOR_YELLOW) << "             " << colorize("# ASCII (default)", COLOR_DIM) << endl;
	cout << "    " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("save", COLOR_BRIGHT_CYAN) << " floppy.dsk " << colorize("\"program.bas\",A", COLOR_YELLOW) << "           " << colorize("# ASCII explicit", COLOR_DIM) << endl;
	cout << "    " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("save", COLOR_BRIGHT_CYAN) << " floppy.dsk " << colorize("\"loader.bin\",B,8000,8010", COLOR_YELLOW) << "  " << colorize("# Binary with addresses", COLOR_DIM) << endl;
	cout << "    " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("save", COLOR_BRIGHT_CYAN) << " floppy.dsk " << colorize("\"data.dat\",R", COLOR_YELLOW) << "              " << colorize("# Raw (no header)", COLOR_DIM) << endl;
	cout << "    " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("save", COLOR_BRIGHT_CYAN) << " floppy.dsk " << colorize("\"config.cfg\",A,P", COLOR_YELLOW) << "          " << colorize("# ASCII protected", COLOR_DIM) << endl;
	cout << "    " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("save", COLOR_BRIGHT_CYAN) << " floppy.dsk " << colorize("\"game.bin\",B,4000,4000,P", COLOR_YELLOW) << "  " << colorize("# Binary protected", COLOR_DIM) << endl;
	cout << endl;
	cout << colorize("ENVIRONMENT VARIABLES:", COLOR_HEADER) << endl;
	cout << "  " << colorize("DISC_DSK_FILE", COLOR_BRIGHT_GREEN) << "             Default DSK file for all commands" << endl;
	cout << "  " << colorize("RVM_PATH", COLOR_BRIGHT_GREEN) << "                  Path to RetroVirtualMachine executable" << endl;
	cout << "  " << colorize("RETROVIRTUALMACHINE_PATH", COLOR_BRIGHT_GREEN) << "  Alternative path to RetroVirtualMachine" << endl;
	cout << "  " << colorize("CPC_MODEL", COLOR_BRIGHT_GREEN) << "                 CPC model for 'run' command (464, 664, 6128) - default: 6128" << endl;
	cout << "  " << colorize("NO_COLOR", COLOR_BRIGHT_GREEN) << "                  Disable colored output (standard)" << endl;
	cout << "  " << colorize("DISC_NO_COLOR", COLOR_BRIGHT_GREEN) << "             Disable colored output (disc-specific)" << endl;
	cout << endl;
	cout << colorize("EXAMPLES:", COLOR_HEADER) << endl;
	cout << "  " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("cat", COLOR_BRIGHT_CYAN) << " floppy.dsk" << endl;
	cout << "  " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("new", COLOR_BRIGHT_CYAN) << " blank.dsk" << endl;
	cout << "  " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("save", COLOR_BRIGHT_CYAN) << " floppy.dsk program.bas " << colorize("-t 0", COLOR_YELLOW) << endl;
	cout << "  " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("era", COLOR_BRIGHT_CYAN) << " floppy.dsk program.bas" << endl;
	cout << "  " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("get", COLOR_BRIGHT_CYAN) << " floppy.dsk program.bas" << endl;
	cout << "  " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("list", COLOR_BRIGHT_CYAN) << " floppy.dsk program.bas" << endl;
	cout << "  " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("disasm", COLOR_BRIGHT_CYAN) << " floppy.dsk game.bin" << endl;
	cout << "  " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("run", COLOR_BRIGHT_CYAN) << " floppy.dsk game.bas" << endl;
	cout << endl;
	cout << colorize("RVM_PATH EXAMPLES:", COLOR_HEADER) << endl;
	#ifdef _WIN32
		cout << "  " << colorize("Windows:", COLOR_BOLD) << " set " << colorize("RVM_PATH", COLOR_BRIGHT_GREEN) << "=\"C:\\Program Files\\RetroVirtualMachine\\RetroVirtualMachine.exe\"" << endl;
	#elif __APPLE__
		cout << "  " << colorize("macOS:", COLOR_BOLD) << " export " << colorize("RVM_PATH", COLOR_BRIGHT_GREEN) << "=\"/Applications/Retro Virtual Machine 2.app/Contents/MacOS/Retro Virtual Machine 2\"" << endl;
	#else
		cout << "  " << colorize("Linux:", COLOR_BOLD) << " export " << colorize("RVM_PATH", COLOR_BRIGHT_GREEN) << "=\"/usr/bin/retrovirtualmachine\"" << endl;
	#endif
	cout << endl;
	cout << colorize("COLOR CONTROL:", COLOR_HEADER) << endl;
	cout << "  " << colorize("--no-color", COLOR_YELLOW) << "           Disable colored output (command line)" << endl;
	cout << "  " << colorize("NO_COLOR=1", COLOR_BRIGHT_GREEN) << "           Disable colored output (environment, standard)" << endl;
	cout << "  " << colorize("DISC_NO_COLOR=1", COLOR_BRIGHT_GREEN) << "       Disable colored output (environment, disc-specific)" << endl;
	cout << endl;
	cout << colorize("Use --no-color flag or set NO_COLOR/DISC_NO_COLOR environment variable", COLOR_DIM) << endl;
	cout << endl;
	exit(0);
}

void help_cat(void)
{
	cout << endl;
	cout << colorize("COMMAND: cat", COLOR_HEADER) << endl;
	cout << colorize("List disk catalog showing all files with their attributes", COLOR_INFO) << endl;
	cout << endl;
	cout << colorize("USAGE:", COLOR_BOLD) << endl;
	cout << "  " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("cat", COLOR_BRIGHT_CYAN) << " <dskfile>" << endl;
	cout << "  " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("cat", COLOR_BRIGHT_CYAN) << "           " << colorize("(uses DISC_DSK_FILE)", COLOR_DIM) << endl;
	cout << endl;
	cout << colorize("DESCRIPTION:", COLOR_BOLD) << endl;
	cout << "  Shows a formatted table with filename, user, load address, exec address," << endl;
	cout << "  file size, and attributes (R=read-only, S=system) for all files in the DSK." << endl;
	cout << endl;
	cout << colorize("EXAMPLES:", COLOR_BOLD) << endl;
	cout << "  " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("cat", COLOR_BRIGHT_CYAN) << " game.dsk" << endl;
	cout << "  export DISC_DSK_FILE=game.dsk && " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("cat", COLOR_BRIGHT_CYAN) << endl;
	cout << endl;
	exit(0);
}

void help_new(void)
{
	cout << endl;
	cout << "COMMAND: new" << endl;
	cout << "Create a new empty DSK file" << endl;
	cout << endl;
	cout << "USAGE:" << endl;
	cout << "  " << PROGNAME << " new <dskfile>" << endl;
	cout << endl;
	cout << "DESCRIPTION:" << endl;
	cout << "  Creates a new empty DSK file with standard CPC disk format." << endl;
	cout << "  The file will be ready to store CPC files." << endl;
	cout << endl;
	cout << "EXAMPLES:" << endl;
	cout << "  " << PROGNAME << " new blank.dsk" << endl;
	cout << "  " << PROGNAME << " new \"my game.dsk\"" << endl;
	cout << endl;
	exit(0);
}

void help_save(void)
{
	cout << endl;
	cout << colorize("COMMAND: save", COLOR_HEADER) << endl;
	cout << colorize("Import a file into the DSK", COLOR_INFO) << endl;
	cout << endl;
	cout << colorize("USAGE:", COLOR_BOLD) << endl;
	cout << "  " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("save", COLOR_BRIGHT_CYAN) << " <dskfile> <file> [options]" << endl;
	cout << "  " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("save", COLOR_BRIGHT_CYAN) << " <file> [options]           " << colorize("(uses DISC_DSK_FILE)", COLOR_DIM) << endl;
	cout << endl;
	cout << colorize("CPC STYLE SYNTAX:", COLOR_BOLD) << endl;
	cout << "  " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("save", COLOR_BRIGHT_CYAN) << " <dskfile> \"file.bas\"" << endl;
	cout << "  " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("save", COLOR_BRIGHT_CYAN) << " <dskfile> \"file.bin\",B,8000,8010" << endl;
	cout << "  " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("save", COLOR_BRIGHT_CYAN) << " <dskfile> \"file.txt\",R" << endl;
	cout << "  " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("save", COLOR_BRIGHT_CYAN) << " <dskfile> \"file.sys\",S" << endl;
	cout << endl;
	cout << colorize("OPTIONS:", COLOR_BOLD) << endl;
	cout << "  " << colorize("-t <type>", COLOR_YELLOW) << "     File type (0=ASCII, 1=BINARY, 2=raw)" << endl;
	cout << "  " << colorize("-e <address>", COLOR_YELLOW) << "  Execute address (hex)" << endl;
	cout << "  " << colorize("-c <address>", COLOR_YELLOW) << "  Load address (hex)" << endl;
	cout << "  " << colorize("-f", COLOR_YELLOW) << "            Force overwrite if file exists" << endl;
	cout << "  " << colorize("-o", COLOR_YELLOW) << "            Mark file as read-only" << endl;
	cout << "  " << colorize("-s", COLOR_YELLOW) << "            Mark file as system file" << endl;
	cout << "  " << colorize("-u <number>", COLOR_YELLOW) << "   Set user number" << endl;
	cout << endl;
	cout << colorize("EXAMPLES:", COLOR_BOLD) << endl;
	cout << "  " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("save", COLOR_BRIGHT_CYAN) << " game.dsk program.bas" << endl;
	cout << "  " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("save", COLOR_BRIGHT_CYAN) << " game.dsk loader.bin " << colorize("-t 1 -c 8000 -e 8000", COLOR_YELLOW) << endl;
	cout << "  " << colorize(PROGNAME, COLOR_FILENAME) << " " << colorize("save", COLOR_BRIGHT_CYAN) << " game.dsk \"loader.bin\",B,8000,8000" << endl;
	cout << endl;
	exit(0);
}

void help_era(void)
{
	cout << endl;
	cout << "COMMAND: era" << endl;
	cout << "Remove a file from the DSK" << endl;
	cout << endl;
	cout << "USAGE:" << endl;
	cout << "  " << PROGNAME << " era <dskfile> <file>" << endl;
	cout << "  " << PROGNAME << " era <file>           (uses DISC_DSK_FILE)" << endl;
	cout << endl;
	cout << "ALIASES:" << endl;
	cout << "  rm - Alternative command name" << endl;
	cout << endl;
	cout << "DESCRIPTION:" << endl;
	cout << "  Removes a file from the DSK. Files marked as read-only (R) or system (S)" << endl;
	cout << "  are protected and cannot be deleted." << endl;
	cout << endl;
	cout << "EXAMPLES:" << endl;
	cout << "  " << PROGNAME << " era game.dsk oldfile.bas" << endl;
	cout << "  " << PROGNAME << " rm game.dsk temp.bin" << endl;
	cout << endl;
	exit(0);
}

void help_get(void)
{
	cout << endl;
	cout << "COMMAND: get" << endl;
	cout << "Export a file from the DSK to the current directory" << endl;
	cout << endl;
	cout << "USAGE:" << endl;
	cout << "  " << PROGNAME << " get <dskfile> <file>" << endl;
	cout << "  " << PROGNAME << " get <file>           (uses DISC_DSK_FILE)" << endl;
	cout << endl;
	cout << "ALIASES:" << endl;
	cout << "  export - Alternative command name" << endl;
	cout << endl;
	cout << "DESCRIPTION:" << endl;
	cout << "  Extracts a file from the DSK and saves it to the current directory" << endl;
	cout << "  with the same name." << endl;
	cout << endl;
	cout << "EXAMPLES:" << endl;
	cout << "  " << PROGNAME << " get game.dsk program.bas" << endl;
	cout << "  " << PROGNAME << " export game.dsk loader.bin" << endl;
	cout << endl;
	exit(0);
}

void help_list(void)
{
	cout << endl;
	cout << "COMMAND: list" << endl;
	cout << "Auto-detect file type and show content" << endl;
	cout << endl;
	cout << "USAGE:" << endl;
	cout << "  " << PROGNAME << " list <dskfile> <file> [-p]" << endl;
	cout << "  " << PROGNAME << " list <file> [-p]      (uses DISC_DSK_FILE)" << endl;
	cout << endl;
	cout << "OPTIONS:" << endl;
	cout << "  -p            Show debug information about detection" << endl;
	cout << endl;
	cout << "DESCRIPTION:" << endl;
	cout << "  Automatically detects if the file is a tokenized BASIC program or ASCII text" << endl;
	cout << "  and displays it accordingly. BASIC programs are detokenized to readable text." << endl;
	cout << endl;
	cout << "EXAMPLES:" << endl;
	cout << "  " << PROGNAME << " list game.dsk program.bas" << endl;
	cout << "  " << PROGNAME << " list game.dsk readme.txt -p" << endl;
	cout << endl;
	exit(0);
}

void help_basic(void)
{
	cout << endl;
	cout << "COMMAND: basic" << endl;
	cout << "Show tokenized BASIC program as readable text" << endl;
	cout << endl;
	cout << "USAGE:" << endl;
	cout << "  " << PROGNAME << " basic <dskfile> <file>" << endl;
	cout << "  " << PROGNAME << " basic <file>           (uses DISC_DSK_FILE)" << endl;
	cout << endl;
	cout << "DESCRIPTION:" << endl;
	cout << "  Forces interpretation of the file as a tokenized BASIC program and" << endl;
	cout << "  converts it to readable text format." << endl;
	cout << endl;
	cout << "EXAMPLES:" << endl;
	cout << "  " << PROGNAME << " basic game.dsk program.bas" << endl;
	cout << "  " << PROGNAME << " basic game.dsk loader.bin" << endl;
	cout << endl;
	exit(0);
}

void help_ascii(void)
{
	cout << endl;
	cout << "COMMAND: ascii" << endl;
	cout << "Show file content as ASCII text" << endl;
	cout << endl;
	cout << "USAGE:" << endl;
	cout << "  " << PROGNAME << " ascii <dskfile> <file>" << endl;
	cout << "  " << PROGNAME << " ascii <file>           (uses DISC_DSK_FILE)" << endl;
	cout << endl;
	cout << "DESCRIPTION:" << endl;
	cout << "  Forces interpretation of the file as ASCII text and displays it." << endl;
	cout << "  Useful for text files or to see raw content of other file types." << endl;
	cout << endl;
	cout << "EXAMPLES:" << endl;
	cout << "  " << PROGNAME << " ascii game.dsk readme.txt" << endl;
	cout << "  " << PROGNAME << " ascii game.dsk program.bas" << endl;
	cout << endl;
	exit(0);
}

void help_hex(void)
{
	cout << endl;
	cout << "COMMAND: hex" << endl;
	cout << "Show file content in hexadecimal format" << endl;
	cout << endl;
	cout << "USAGE:" << endl;
	cout << "  " << PROGNAME << " hex <dskfile> <file>" << endl;
	cout << "  " << PROGNAME << " hex <file>           (uses DISC_DSK_FILE)" << endl;
	cout << endl;
	cout << "DESCRIPTION:" << endl;
	cout << "  Displays the file content in hexadecimal format with ASCII representation." << endl;
	cout << "  Useful for analyzing binary files, headers, and raw data." << endl;
	cout << endl;
	cout << "EXAMPLES:" << endl;
	cout << "  " << PROGNAME << " hex game.dsk loader.bin" << endl;
	cout << "  " << PROGNAME << " hex game.dsk program.bas" << endl;
	cout << endl;
	exit(0);
}

void help_disasm(void)
{
	cout << endl;
	cout << "COMMAND: disasm" << endl;
	cout << "Disassemble binary file to Z80 assembly" << endl;
	cout << endl;
	cout << "USAGE:" << endl;
	cout << "  " << PROGNAME << " disasm <dskfile> <file>" << endl;
	cout << "  " << PROGNAME << " disasm <file>           (uses DISC_DSK_FILE)" << endl;
	cout << endl;
	cout << "ALIASES:" << endl;
	cout << "  disassemble - Full command name" << endl;
	cout << endl;
	cout << "DESCRIPTION:" << endl;
	cout << "  Disassembles binary files showing Z80 assembly code with addresses." << endl;
	cout << "  Useful for analyzing machine code programs and loaders." << endl;
	cout << endl;
	cout << "EXAMPLES:" << endl;
	cout << "  " << PROGNAME << " disasm game.dsk loader.bin" << endl;
	cout << "  " << PROGNAME << " disassemble game.dsk game.bin" << endl;
	cout << endl;
	exit(0);
}

void help_dams(void)
{
	cout << endl;
	cout << "COMMAND: dams" << endl;
	cout << "Show DAMS (Devpac Amstrad Macro System) file content" << endl;
	cout << endl;
	cout << "USAGE:" << endl;
	cout << "  " << PROGNAME << " dams <dskfile> <file>" << endl;
	cout << "  " << PROGNAME << " dams <file>           (uses DISC_DSK_FILE)" << endl;
	cout << endl;
	cout << "DESCRIPTION:" << endl;
	cout << "  Displays DAMS assembler source files in readable format." << endl;
	cout << "  DAMS was a popular Z80 assembler for the Amstrad CPC." << endl;
	cout << endl;
	cout << "EXAMPLES:" << endl;
	cout << "  " << PROGNAME << " dams game.dsk source.dam" << endl;
	cout << "  " << PROGNAME << " dams game.dsk main.asm" << endl;
	cout << endl;
	exit(0);
}

void help_run(void)
{
	cout << endl;
	cout << "COMMAND: run" << endl;
	cout << "Launch RetroVirtualMachine with the DSK and file" << endl;
	cout << endl;
	cout << "USAGE:" << endl;
	cout << "  " << PROGNAME << " run <dskfile> <file>" << endl;
	cout << "  " << PROGNAME << " run <file>           (uses DISC_DSK_FILE)" << endl;
	cout << endl;
	cout << "ENVIRONMENT VARIABLES:" << endl;
	cout << "  RVM_PATH               Path to RetroVirtualMachine executable" << endl;
	cout << "  RETROVIRTUALMACHINE_PATH  Alternative path to RetroVirtualMachine" << endl;
	cout << "  CPC_MODEL              CPC model (464, 664, 6128) - default: 6128" << endl;
	cout << endl;
	cout << "DESCRIPTION:" << endl;
	cout << "  Closes any existing RetroVirtualMachine instances and launches a new one" << endl;
	cout << "  with the specified DSK loaded and ready to run the file." << endl;
	cout << endl;
	cout << "EXAMPLES:" << endl;
	cout << "  " << PROGNAME << " run game.dsk program.bas" << endl;
	cout << "  export CPC_MODEL=464 && " << PROGNAME << " run game.dsk loader.bin" << endl;
	cout << endl;
	exit(0);
}
