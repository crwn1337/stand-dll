#include "defs.hpp"

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <windows.h>
#include <knownfolders.h>
#include <shlobj.h>
#include <urlmon.h>
#include <fstream>

std::pair<std::wstring, std::wstring> split(const std::wstring &s) {
    auto pos = s.find(L":");
    return {s.substr(0, pos), s.substr(pos + 1)};
}

std::wstring read_file(const std::wstring &path) {
    std::wifstream file(path.c_str(), std::ios::binary);
    std::wstring contents((std::istreambuf_iterator<wchar_t>(file)), std::istreambuf_iterator<wchar_t>());
    return contents;
}

bool dir_exists(const std::wstring &path) {
    return std::filesystem::is_directory(path);
}

bool create_dir(const std::wstring &path) {
    return std::filesystem::create_directory(path);
}

bool file_exists(const std::wstring &path) {
    return std::filesystem::exists(path);
}

bool delete_file(const std::wstring &path) {
    return std::filesystem::remove(path);
}

bool delete_dir(const std::wstring &path) {
    return std::filesystem::remove_all(path);
}

std::wstring get_folder(const GUID &id) {
    wchar_t *temp;
    SHGetKnownFolderPath(id, 0, NULL, &temp);
    std::wstring path(temp);
    CoTaskMemFree(temp);
    return path;
}

void attach() {
    const auto stand_folder = get_folder(FOLDERID_RoamingAppData) + L"\\Stand";
    if (!dir_exists(stand_folder))
        create_dir(stand_folder);

    const auto bin_folder = stand_folder + L"\\Bin";
    if (!dir_exists(bin_folder))
        create_dir(bin_folder);

    const auto versions_file = bin_folder + L"\\versions.txt";
    if (file_exists(versions_file))
        delete_file(versions_file);
    if (URLDownloadToFileW(nullptr, L"https://stand.sh/versions.txt", versions_file.c_str(), BINDF_GETNEWESTVERSION, nullptr) != S_OK) {
        MessageBoxW(nullptr, L"Failed to get the version list :(", L"Stand", MB_ICONERROR);
        return;
    }

    const auto dll_version = split(read_file(versions_file)).second;
    const auto dll_file = bin_folder + L"\\stand.dll";
    if (file_exists(dll_file))
        delete_file(dll_file);
    if (URLDownloadToFileW(nullptr, (std::wstring(L"https://stand.sh/Stand%20") + dll_version + std::wstring(L".dll")).c_str(), dll_file.c_str(), BINDF_GETNEWESTVERSION, nullptr) != S_OK) {
        MessageBoxW(nullptr, L"Failed to get the dll :(", L"Stand", MB_ICONERROR);
        return;
    }

    // BUG BUG BUG
    // for some reason stand crashes on linux if this folder exists, no idea why
    const auto theme_folder = stand_folder + L"\\Theme";
    if (dir_exists(theme_folder))
        delete_dir(theme_folder);

    LoadLibraryW(dll_file.c_str());
}

BOOL APIENTRY DllMain(const HMODULE hmod, const DWORD reason, const LPVOID) {
    if (reason == DLL_PROCESS_ATTACH)
        if (const auto thread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)attach, nullptr, 0, nullptr))
            CloseHandle(thread);
    return TRUE;
}