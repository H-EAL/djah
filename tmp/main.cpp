#include "qtcolladaviewer.h"
#include <QtGui/QApplication>
#include <djah/filesystem/browser.hpp>
#include <djah/filesystem/directory_source.hpp>
#include <djah/system/gl.hpp>
#include <djah/debug/log.hpp>
#include <djah/debug/console_sink.hpp>

int main(int argc, char *argv[])
{
	djah::filesystem::browser::get().addLoadingChannel( new djah::filesystem::directory_source("./") );

	std::shared_ptr<djah::debug::basic_sink> pDebugSink( new djah::debug::output_debug_sink(djah::debug::log_filter("any")) );
	djah::debug::core_logger::get().addSink( pDebugSink );

	QApplication a(argc, argv);
	qtColladaViewer w;
	w.showMaximized();
	return a.exec();
}
