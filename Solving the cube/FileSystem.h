#pragma once

#include <fstream>
#include "CUtils.h"

static void dumpToFile(const utils::String file_path, const char* data, unsigned int size) {
	std::fstream file(file_path.getData(), std::ios::out | std::ios::binary);
	file.write(data, size);
	file.close();
}

static utils::String readFromFile(const utils::String file_path) {
	std::ifstream file(file_path.getData(), std::ios::in | std::ios::binary);
	utils::String data(file);
	file.close();

	return data;
}

static bool doesFileExist(const utils::String file_path) {
	std::ifstream file(file_path.getData(), std::ios::binary);
	bool is_open = file.is_open();
	file.close();

	return is_open;
}