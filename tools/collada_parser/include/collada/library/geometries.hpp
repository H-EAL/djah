#ifndef COLLADA_LIBRARY_GEOMETRIES_HPP
#define COLLADA_LIBRARY_GEOMETRIES_HPP

#include "../fwd_decl.hpp"

namespace collada { namespace library {

	//----------------------------------------------------------------------------------------------
	class mesh;
	class vertices;
	class triangles;
	//----------------------------------------------------------------------------------------------
	typedef std::vector<triangles*>	triangles_list_t;
	//----------------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------------
	class geometries
	{
	public:
		void deserialize(const TiXmlElement &element);
		~geometries();

		geometry_list_t	geometries_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class geometry
	{
	public:
		geometry() : mesh_(0) {}
		void deserialize(const TiXmlElement &element);
		~geometry();

		std::string	id_;
		std::string name_;
		mesh		*mesh_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class mesh
	{
	public:
		mesh() : vertices_(0) {}
		void deserialize(const TiXmlElement &element);
		~mesh();

		source_list_t		sources_;
		vertices			*vertices_;
		triangles_list_t	triangles_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class vertices
	{
	public:
		void deserialize(const TiXmlElement &element);
		~vertices();

		std::string  id_;
		input_list_t inputs_;
	};
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class triangles
	{
	public:
		triangles() : indices_(0), vertex_count_(0), elements_count_(0) {}
		void deserialize(const TiXmlElement &element);
		~triangles();

		unsigned int   count_;
		unsigned int   vertex_count_;
		unsigned int   elements_count_;
		std::string    material_;
		input_list_t   inputs_;
		unsigned int  *indices_;
	};
	//----------------------------------------------------------------------------------------------

} /*library*/ } /*collada*/

#endif /* COLLADA_LIBRARY_GEOMETRIES_HPP */