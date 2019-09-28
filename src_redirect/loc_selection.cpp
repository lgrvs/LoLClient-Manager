#include <iostream>

#include <string>
#include <fstream>
#include <Windows.h>


int main(int argc, char * argv[])
{
	std::string newCmd = ""; // Modified LeagueClient's arguments
	std::string workingDir = ""; // LeagueClient's directory
	std::string locale_setting = ""; // User selected locale through the GUI, saved in a simple .txt file
	newCmd.append("\"");
	newCmd.append(argv[0]);
	newCmd.append("\"");

	std::ifstream read_loc;
	read_loc.open("chosen_locale.txt");
	if (!read_loc) {
		MessageBoxA(0, "Could not find or load language settings.\nPlease select a language before using this utility.", "LoL Language utility", MB_ICONERROR);
		return 0;
	}
	
	read_loc >> locale_setting;
	read_loc.close();

	int rval = 0; //If LeagueClient can't be opened, still returns 0 to RiotClient to remain undetected. Otherwise used to return LeagueClient's exit code as usual.
	for (int i = 1; i < argc; i++) {
		newCmd.append(" ");
		if (std::string(argv[i]).find("locale") == std::string::npos) { // If the arg is not about the locale
			newCmd.append("\"");
			newCmd.append(argv[i]);
			newCmd.append("\"");
		}
		else {
			newCmd.append("\"--locale=" + locale_setting); // Replace the locale
			newCmd.append("\"");
		}
	}
	{
		std::string path_to_client = argv[0];
		workingDir = path_to_client.substr(0, path_to_client.length() - 16);
	}
	
	system("start cmd /c \"move LeagueClient.exe loc_selection.exe & move og.exe LeagueClient.exe\""); // Open new process
	// to replace the intercepter with the real LeagueClient
	Sleep(100);

	STARTUPINFOA start_info;
	PROCESS_INFORMATION process_info;
	ZeroMemory(&start_info, sizeof(start_info));
	start_info.cb = sizeof start_info;

	DWORD client_r_code;
	
	if (!CreateProcessA(argv[0], (char *)newCmd.c_str(), 0, 0, true, 0, 0, workingDir.c_str(), &start_info, &process_info)) {
		MessageBoxA(0, "Could not create process LeagueClient.exe\nPlease report this issue.", "LoL Language utility", MB_ICONERROR);
	} // Execute LeagueClient with new args and retrieve handle to return its exit code to RiotClient when it exits, so we don't interfere with that.
	else {
		WaitForSingleObject(process_info.hProcess, INFINITE);
		GetExitCodeProcess(process_info.hProcess, &client_r_code);
		rval = client_r_code;
	}
	return rval;
}
