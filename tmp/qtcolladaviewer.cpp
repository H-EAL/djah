#include "qtcolladaviewer.h"
#include <QFileDialog>
#include <QTime>

#include <djah/math.hpp>

#include "collada/utils.hpp"
#include "collada/proxy.hpp"
#include "collada/library.hpp"

#include "../../collada_converter/sources/mesh_builder.hpp"

using namespace djah;

//----------------------------------------------------------------------------------------------
class qt_sink
	: public debug::basic_sink
{
public:
	qt_sink(QPlainTextEdit *pTxtEdit, const debug::log_filter &_filter = debug::log_filter())
		: debug::basic_sink(_filter)
		, pTxtEdit_(pTxtEdit)
	{

	}
	virtual ~qt_sink()
	{

	}

	virtual void consume(const debug::basic_record &rec)
	{
		QTime time;
		time.addMSecs(rec.timestamp());

		QString txt = QString("[%1] [%2] - %3")
			.arg( QTime::currentTime().toString("hh:mm:ss") )
			.arg(rec.channels().c_str())
			.arg(rec.message().c_str());

		pTxtEdit_->appendPlainText(txt);
	}
	
private:
	QPlainTextEdit *pTxtEdit_;
};
//----------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
qtColladaViewer::qtColladaViewer(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
	, pMeshBuilder_(0)
{
	ui.setupUi(this);

	std::shared_ptr<djah::debug::basic_sink> pQtSink( new qt_sink(ui.Log_TextEdit, djah::debug::log_filter("any")) );
	djah::debug::core_logger::get().addSink( pQtSink );

	connect(ui.Translation_X_SpinBox, SIGNAL(valueChanged(double)), this, SLOT(onUpdateTranslation()));
	connect(ui.Translation_Y_SpinBox, SIGNAL(valueChanged(double)), this, SLOT(onUpdateTranslation()));
	connect(ui.Translation_Z_SpinBox, SIGNAL(valueChanged(double)), this, SLOT(onUpdateTranslation()));

	connect(ui.Rotation_X_SpinBox, SIGNAL(valueChanged(int)), this, SLOT(onUpdateRotation()));
	connect(ui.Rotation_Y_SpinBox, SIGNAL(valueChanged(int)), this, SLOT(onUpdateRotation()));
	connect(ui.Rotation_Z_SpinBox, SIGNAL(valueChanged(int)), this, SLOT(onUpdateRotation()));

	connect(ui.Scale_X_SpinBox, SIGNAL(valueChanged(double)), this, SLOT(onUpdateScale()));
	connect(ui.Scale_Y_SpinBox, SIGNAL(valueChanged(double)), this, SLOT(onUpdateScale()));
	connect(ui.Scale_Z_SpinBox, SIGNAL(valueChanged(double)), this, SLOT(onUpdateScale()));
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
qtColladaViewer::~qtColladaViewer()
{
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void qtColladaViewer::on_ResetTransformation_Button_clicked()
{
	ui.Translation_X_SpinBox->setValue(0.0);
	ui.Translation_Y_SpinBox->setValue(0.0);
	ui.Translation_Z_SpinBox->setValue(0.0);

	ui.Rotation_X_SpinBox->setValue(0);
	ui.Rotation_Y_SpinBox->setValue(0);
	ui.Rotation_Z_SpinBox->setValue(0);

	ui.Scale_X_SpinBox->setValue(1.0);
	ui.Scale_Y_SpinBox->setValue(1.0);
	ui.Scale_Z_SpinBox->setValue(1.0);
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void qtColladaViewer::on_actionOpen_triggered()
{
	const QString &fileName = QFileDialog::getOpenFileName(this, "", "", "*.dae");

	if( !fileName.isEmpty() )
	{
		on_ResetTransformation_Button_clicked();
		ui.Collada_TreeWidget->clear();

		if( pMeshBuilder_ )
		{
			ui.GLView_Widget->setMesh(0);
			delete pMeshBuilder_;
			pMeshBuilder_ = 0;
		}

		ui.StatusBar->showMessage( QString("Loading %1 ...").arg(fileName) );
		collada::proxy colladaObject(fileName.toAscii().constData());
		if( colladaObject.good() )
		{
			printColladaObjectInfos(colladaObject, QFileInfo(fileName) );
			pMeshBuilder_ = new mesh_builder(colladaObject);
			ui.GLView_Widget->setMesh(pMeshBuilder_);
			ui.StatusBar->showMessage( QString("Loading %1 ... OK").arg(fileName) );

			on_actionOpenTexture_triggered();
		}
		else
		{
			ui.StatusBar->showMessage( QString("Loading %1 ... ERROR").arg(fileName) );
		}
	}
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void qtColladaViewer::on_actionOpenTexture_triggered()
{
	const QString &fileName = QFileDialog::getOpenFileName(this, "", "", "Images(*.jpg *.png *.tga *.bmp)");

	if( !fileName.isEmpty() )
	{
		ui.GLView_Widget->setTexture(fileName.toAscii().constData());
	}
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void qtColladaViewer::on_SnapToGrid_Button_clicked()
{
	if( pMeshBuilder_ )
	{
		float transY = ui.GLView_Widget->snapToGrid();
		ui.Translation_Y_SpinBox->setValue(transY);
	}
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void qtColladaViewer::printColladaObjectInfos(const collada::proxy &colladaObject, const QFileInfo &fileInfo)
{
	// OBJECT
	QTreeWidgetItem *pRootItem = new QTreeWidgetItem( ui.Collada_TreeWidget );
	pRootItem->setText( 0, fileInfo.baseName() );
	pRootItem->setExpanded(true);

	if( colladaObject.getGeometriesLib() )
	{
		bool first = true;
		auto geos = colladaObject.getGeometries();
		ui.ModelCount_Label->setText( QString("%1").arg(geos.size()) );

		QTreeWidgetItem *pGeoItem = nullptr;
		if( !geos.empty() )
		{
			pGeoItem = new QTreeWidgetItem(pRootItem);
			pGeoItem->setText(0, "Models");
			pGeoItem->setExpanded(true);
		}

		std::for_each(geos.begin(), geos.end(), [&](const collada::library::geometry *pGeo)
		{
			// MODEL
			QTreeWidgetItem *pModelItem = new QTreeWidgetItem(pGeoItem);
			pModelItem->setText(0, pGeo->name_.c_str());
			pModelItem->setCheckState(0, Qt::Checked);
			pModelItem->setExpanded(true);

			std::for_each( pGeo->mesh_->triangles_.begin(), pGeo->mesh_->triangles_.end(), [&](const collada::library::triangles *pTris)
			{
				// MESH
				QTreeWidgetItem *pMeshItem = new QTreeWidgetItem(pModelItem);
				pMeshItem->setText(0, pTris->material_.c_str());
				pMeshItem->setCheckState(0, Qt::Checked);
				pMeshItem->setExpanded(true);

				std::for_each(pTris->inputs_.begin(), pTris->inputs_.end(), [&](const collada::library::input *pInput)
				{
					// VERTEX ATTRIBUTES
					collada::library::source *pSource = collada::get_source_by_id(pInput->source_, pGeo->mesh_->sources_);
					if( !pSource )
					{
						collada::library::input *pIn = collada::get_input_by_semantic(collada::ESS_POSITION, pGeo->mesh_->vertices_->inputs_);
						pSource = pIn ? collada::get_source_by_id(pIn->source_, pGeo->mesh_->sources_) : nullptr;
					}
					if( pSource )
					{
						QTreeWidgetItem *pVFItem = new QTreeWidgetItem(pMeshItem);
						pVFItem->setText(0, QString("%1%2 (%3)").arg(pInput->semantic_.c_str()).arg(pInput->set_).arg(pSource->technique_common_->accessor_->stride_));
						pVFItem->setCheckState(0, Qt::Checked);
					}
				});
			});
		});
	}


	QString logTxt;
	if( colladaObject.getMaterialsLib() )
	{
		logTxt += QString("{ MATERIALS }\n");
		auto mats = colladaObject.getMaterials();
		std::for_each(mats.begin(), mats.end(), [&](const collada::library::material *pMat)
		{
			logTxt += QString("[%1] %2 (%3)\n").arg(pMat->id_.c_str(), pMat->name_.c_str(), pMat->instance_effect_->url_.c_str());
		});
	}


	if( colladaObject.getEffectsLib() )
	{
		logTxt += QString("\n");
		logTxt += QString("{ EFFECTS }\n");
		auto fxs = colladaObject.getEffects();
		std::for_each(fxs.begin(), fxs.end(), [&](const collada::library::effect *pFx)
		{
			logTxt += QString("[%1] %2\n").arg(pFx->id_.c_str(), pFx->name_.c_str());
		});
	}


	if( colladaObject.getImagesLib() )
	{
		logTxt += QString("\n");
		logTxt += QString("{ TEXTURES }\n");
		auto imgs = colladaObject.getImages();
		std::for_each(imgs.begin(), imgs.end(), [&](const collada::library::image *pImg)
		{
			logTxt += QString("%1 (%2) [%3]\n").arg(pImg->name_.c_str(), pImg->init_from_filename_.c_str(), pImg->init_from_fullpath_.c_str());
		});
	}

	ui.Log_TextEdit->setPlainText(logTxt);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void qtColladaViewer::onUpdateTranslation()
{
	const math::vector3f translation
	(
		static_cast<float>(ui.Translation_X_SpinBox->value()),
		static_cast<float>(ui.Translation_Y_SpinBox->value()),
		static_cast<float>(ui.Translation_Z_SpinBox->value())
	);

	ui.GLView_Widget->transformation().setTranslation(translation);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void qtColladaViewer::onUpdateRotation()
{
	const float xAngle = math::deg_to_rad( static_cast<float>(ui.Rotation_X_SpinBox->value()) );
	math::quatf rot = math::make_quaternion( xAngle, math::vector3f::x_axis );

	const float yAngle = math::deg_to_rad( static_cast<float>(ui.Rotation_Y_SpinBox->value()) );
	rot *= math::make_quaternion( yAngle, math::vector3f::y_axis );

	const float zAngle = math::deg_to_rad( static_cast<float>(ui.Rotation_Z_SpinBox->value()) );
	rot *= math::make_quaternion( zAngle, math::vector3f::z_axis );

	ui.GLView_Widget->transformation().setRotation(rot);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void qtColladaViewer::onUpdateScale()
{
	math::vector3f scale
	(
		static_cast<float>(ui.Scale_X_SpinBox->value()),
		static_cast<float>(ui.Scale_Y_SpinBox->value()),
		static_cast<float>(ui.Scale_Z_SpinBox->value())
	);

	if( ui.Scale_Uniform_CheckBox->checkState() == Qt::Checked )
	{
		scale.y = scale.z = scale.x;
	}

	ui.GLView_Widget->transformation().setScale(scale);
}
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
void qtColladaViewer::on_Export_Button_clicked()
{
	if( !pMeshBuilder_ )
		return;

	const QString &fileName = QFileDialog::getSaveFileName(this, "", "", "*.bdae");

	if( fileName.isEmpty() )
		return;

	QFile file(fileName);
	if( !file.open(QIODevice::WriteOnly) )
		return;

	ui.StatusBar->showMessage( QString("Exporting %1 ...").arg(fileName) );
	/*
	const int nbModels = (int)pMeshBuilder_->getModelCount();
	for(int m = 0; m < nbModels; ++m)
	{
		model *mod = pMeshBuilder_->getModel(m);
		const int nbMeshes = (int)mod->getMeshCount();
		for(int b = 0; b < nbMeshes; ++b)
		{
			submesh *msh = mod->getMesh(b);
			const std::vector<float>			&positions		= msh->positions_;
			const std::vector<float>			&normals		= msh->normals_;
			const std::vector<float>			&tex_coords0	= msh->tex_coords0_;
			const std::vector<float>			&tex_coords1	= msh->tex_coords1_;
			const std::vector<float>			&tex_coords2	= msh->tex_coords2_;
			const std::vector<float>			&tex_coords3	= msh->tex_coords3_;
			const std::vector<float>			&tex_tangents0	= msh->tex_tangents0_;
			const std::vector<float>			&tex_tangents1	= msh->tex_tangents1_;
			const std::vector<float>			&tex_binormals0	= msh->tex_binormals0_;
			const std::vector<float>			&tex_binormals1	= msh->tex_binormals1_;
			const std::vector<float>			&weights		= msh->weights_;
			const std::vector<unsigned short>	&influences		= msh->influences_;
			const size_t vertexSize								= msh->vertex_size_;

			const int pos_stride			= msh->getPositionStride();
			const int norm_stride			= msh->getNormalStride();
			const int tex_coord0_stride		= msh->getTexCoord0Stride();
			const int tex_coord1_stride		= msh->getTexCoord1Stride();
			const int tex_coord2_stride		= msh->getTexCoord2Stride();
			const int tex_coord3_stride		= msh->getTexCoord3Stride();
			const int tex_tangent0_stride	= msh->getTexTangent0Stride();
			const int tex_tangent1_stride	= msh->getTexTangent1Stride();
			const int tex_binormal0_stride	= msh->getTexBinormal0Stride();
			const int tex_binormal1_stride	= msh->getTexBinormal1Stride();
			const int weight_stride			= msh->getWeightStride();
			const int infl_stride			= msh->getInfluenceStride();

			if( !positions.empty() )
			{
				for(int i = 0; i < msh->vertex_count_; ++i)
				{
					math::vector4f pos(positions[i*pos_stride], positions[i*pos_stride+1], positions[i*pos_stride+2], 1.0f);
					pos = math::transform(ui.GLView_Widget->transformation().toMatrix4().getTransposed(), pos);
					file.write((const char*)pos.data, pos_stride * sizeof(float));

					if( !normals.empty() )
					{
						math::vector4f norm(normals[i*norm_stride], normals[i*norm_stride+1], normals[i*norm_stride+2], 0.0f);
						norm = math::transform(ui.GLView_Widget->transformation().toMatrix4().getTransposed(), norm);
						file.write((const char*)norm.data, norm_stride * sizeof(float));
					}

					if( !tex_coords0.empty() )
						file.write((const char*)&tex_coords0[i*tex_coord0_stride], tex_coord0_stride * sizeof(float));
					if( !tex_coords1.empty() )
						file.write((const char*)&tex_coords1[i*tex_coord1_stride], tex_coord1_stride * sizeof(float));
					if( !tex_coords2.empty() )
						file.write((const char*)&tex_coords2[i*tex_coord2_stride], tex_coord2_stride * sizeof(float));
					if( !tex_coords3.empty() )
						file.write((const char*)&tex_coords3[i*tex_coord3_stride], tex_coord3_stride * sizeof(float));

					if( !tex_tangents0.empty() )
						file.write((const char*)&tex_tangents0[i*tex_tangent0_stride], tex_tangent0_stride * sizeof(float));
					if( !tex_tangents1.empty() )
						file.write((const char*)&tex_tangents1[i*tex_tangent1_stride], tex_tangent1_stride * sizeof(float));
					
					if( !tex_binormals0.empty() )
						file.write((const char*)&tex_binormals0[i*tex_binormal0_stride], tex_binormal0_stride * sizeof(float));
					if( !tex_binormals1.empty() )
						file.write((const char*)&tex_binormals1[i*tex_binormal1_stride], tex_binormal1_stride * sizeof(float));
					*
					if( !weights.empty() )
						file.write((const char*)&weights[i*weight_stride], weight_stride * sizeof(float));

					if( !influences.empty() )
						file.write((const char*)&influences[i*infl_stride], infl_stride * sizeof(unsigned short));
						*
				}
			}
		}
	}
	*/
	ui.StatusBar->showMessage( QString("Exporting %1 ... DONE").arg(fileName) );
}
//--------------------------------------------------------------------------------------------------
