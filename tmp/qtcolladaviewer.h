#ifndef QTCOLLADAVIEWER_H
#define QTCOLLADAVIEWER_H

#include <QtGui/QMainWindow>
#include <QFileInfo>
#include "ui_qtcolladaviewer.h"

namespace collada {
	class proxy;
}/* proxy*/

class mesh_builder;

class qtColladaViewer : public QMainWindow
{
	Q_OBJECT

public:
	qtColladaViewer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~qtColladaViewer();

private slots:
	void on_ResetTransformation_Button_clicked();
	void on_actionOpen_triggered();
	void on_actionOpenTexture_triggered();
	void onUpdateTranslation();
	void onUpdateRotation();
	void onUpdateScale();
	void on_Export_Button_clicked();
	void on_SnapToGrid_Button_clicked();

private:
	void printColladaObjectInfos(const collada::proxy &colladaObject, const QFileInfo &fileInfo);

private:
	Ui::qtColladaViewerClass ui;
	mesh_builder *pMeshBuilder_;
};

#endif // QTCOLLADAVIEWER_H
