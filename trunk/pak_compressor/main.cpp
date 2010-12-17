#include <iostream>
#include <djah/fs/filesystem.hpp>
#include <djah/fs/directory_source.hpp>
#include "pak_compressor.hpp"


int main(int argc, char *argv[])
{
	djah::fs::filesystem::get_instance().addLoadingChannel(new djah::fs::directory_source("."));
	djah::fs::filesystem::get_instance().addSavingChannel(new djah::fs::directory_source(".", true));
	return (argc < 2) ? pak_compressor::show_help() : pak_compressor(argv[1], argc > 3 ? argv[2] : "").compress();
}