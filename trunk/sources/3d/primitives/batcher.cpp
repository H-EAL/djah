#include "djah/3d/primitives/batcher.hpp"
#include "djah/system/gl.hpp"

namespace djah { namespace d3d { namespace primitives {

		//------------------------------------------------------------------------------------------
		batcher::batcher()
			: pPrimitiveVertexBuffer_(nullptr)
			, pTransformationsBuffer_(nullptr)
			, batchShader_("batch")
			, dirtyFlag_(true)
		{
			vertexFormat_.record()
				<< opengl::format::position<3,float>()
				<< opengl::format::normal<3,float>()
				<< opengl::format::tex_coord<2,float>();

			transformationFormat_.record()
				<< opengl::format::vertex_attrib<3,float>("Translation",1)
				<< opengl::format::vertex_attrib<3,float>("Scale",1)
				<< opengl::format::vertex_attrib<4,float>("Rotation",1)
				<< opengl::format::vertex_attrib<3,float>("Color",1);
		}
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		batcher::~batcher()
		{
			delete pPrimitiveVertexBuffer_;
			delete pTransformationsBuffer_;
		}
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		batcher::prim_id_t batcher::add(const math::transform_f &t, const math::vector3f &color)
		{
			const trans tr = { t, color };
			prim_id_t primId = 0;
			if( availableSlots_.empty() )
			{
				transformations_.push_back( tr );
				primId = static_cast<u32>(transformations_.size() - 1);
			}
			else
			{
				primId = *(availableSlots_.rbegin());
				transformations_[primId] = tr;
				availableSlots_.erase(primId);
			}

			dirtyFlag_ = true;
			return primId;
		}
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		void batcher::set(batcher::prim_id_t primId, float h)
		{
			if( transformations_[primId].c.z != h )
			{
				transformations_[primId].c.z = h;
				dirtyFlag_ = true;
			}
		}
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		void batcher::remove(prim_id_t primId)
		{
			DJAH_ASSERT(primId < transformations_.size());
			availableSlots_.insert(primId);
			dirtyFlag_ = true;
		}
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		int batcher::count() const
		{
			return transformations_.size() - availableSlots_.size();
		}
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		void batcher::init(const std::vector<triangle> &triangles)
		{
			clean();

			pPrimitiveVertexBuffer_ = new opengl::vertex_buffer(triangles.size() * sizeof(triangle), opengl::eBU_StaticDraw);
			pPrimitiveVertexBuffer_->write(&triangles[0], triangles.size());
			vertexArray_.addVertexBuffer(pPrimitiveVertexBuffer_, vertexFormat_);
			vertexArray_.setVertexCount(triangles.size()*3);
		}
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		void batcher::init(const resources::mesh_ptr &pMesh)
		{
			clean();
			pMesh_ = pMesh;
		}
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		void batcher::clean()
		{
			if( pPrimitiveVertexBuffer_ )
			{
				vertexArray_.removeVertexBuffer(pPrimitiveVertexBuffer_);
				delete pPrimitiveVertexBuffer_;
				pPrimitiveVertexBuffer_ = nullptr;
			}

			if( pTransformationsBuffer_ )
			{
				vertexArray_.removeVertexBuffer(pTransformationsBuffer_);
				delete pTransformationsBuffer_;
				pTransformationsBuffer_ = nullptr;
			}

			transformations_.clear();
		}
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		void batcher::resetTransformationsBuffer()
		{
			if( pTransformationsBuffer_ )
			{
				if( pMesh_ )
				{
					pMesh_->removeVertexBuffer(pTransformationsBuffer_);
				}
				else
				{
					vertexArray_.removeVertexBuffer(pTransformationsBuffer_);
				}
				delete pTransformationsBuffer_;
				pTransformationsBuffer_ = nullptr;
			}

			const int nbTrans = transformations_.size();
			int offset = 0;
			pTransformationsBuffer_ = new opengl::vertex_buffer(count() * sizeof(trans), opengl::eBU_DynamicDraw);
			for(int i = 0; i < nbTrans; ++i)
			{
				if( availableSlots_.find(i) == availableSlots_.end() )
				{
					offset += pTransformationsBuffer_->write(&transformations_[i], 1, offset);
				}
			}

			if(pMesh_)
			{
				pMesh_->addVertexBuffer(pTransformationsBuffer_, transformationFormat_);
				pMesh_->init(batchShader_.program());
			}
			else
			{
				vertexArray_.addVertexBuffer(pTransformationsBuffer_, transformationFormat_);
				vertexArray_.init(batchShader_.program());
			}

			dirtyFlag_ = false;
		}
		//------------------------------------------------------------------------------------------


		//------------------------------------------------------------------------------------------
		void batcher::draw()
		{
			if( transformations_.empty() )
				return;

			if( dirtyFlag_ )
				resetTransformationsBuffer();

			if( pMesh_ )
				pMesh_->drawInstanced(transformations_.size());
			else
				vertexArray_.drawInstanced(transformations_.size());
		}
		//------------------------------------------------------------------------------------------

} /*primitives*/ } /*d3d*/ } /*djah*/