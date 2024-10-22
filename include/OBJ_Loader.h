#pragma once

#include <vector>
#include <algorithm>

#include "BVH_Node.h"
#include "utilities.h"
#include "Matrix4.h"
#include "Point3.h"
#include "Primitive.h"
#include "Texture.h"
#include "Triangle.h"

class OBJ_Loader {
public:
	// Constructors and Destructor
	// ---------------------------
	OBJ_Loader(std::vector<std::shared_ptr<Primitive>>* p);
	~OBJ_Loader();

	// Other functions
	// ---------------
	void read_obj(const char* obj_filepath, 
				  const int information_level,
				  std::shared_ptr<Material> m,
				  std::shared_ptr<Texture>  t,
				  const double scale_X     = 1.0,   const double scale_Y     = 1.0,   const double scale_Z     = 1.0,
				  const double shear_XY    = 0.0,   const double shear_Xz    = 0.0,
				  const double shear_YX    = 0.0,   const double shear_YZ    = 0.0,
				  const double shear_ZX    = 0.0,   const double shear_ZY    = 0.0,
				  const double rotate_X    = 0.0,   const double rotate_Y    = 0.0,   const double rotate_Z    = 0.0,
				  const bool   reflect_X   = false, const bool   reflect_Y   = false, const bool   reflect_Z   = false,
				  const bool   reflect_YOZ = false, const bool   reflect_XOZ = false, const bool   reflect_XOY = false,
				  const double translate_X = 0.0,   const double translate_Y = 0.0,   const double translate_Z = 0.0);

private:
	void configurate(std::shared_ptr<Material> m,
					 std::shared_ptr<Texture>  t,
					 const double scale_X     = 1.0,   const double scale_Y     = 1.0,   const double scale_Z     = 1.0,
				     const double shear_XY    = 0.0,   const double shear_Xz    = 0.0,
				     const double shear_YX    = 0.0,   const double shear_YZ    = 0.0,
				     const double shear_ZX    = 0.0,   const double shear_ZY    = 0.0,
				     const double rotate_X    = 0.0,   const double rotate_Y    = 0.0,   const double rotate_Z    = 0.0,
				     const bool   reflect_X   = false, const bool   reflect_Y   = false, const bool   reflect_Z   = false,
				     const bool   reflect_YOZ = false, const bool   reflect_XOZ = false, const bool   reflect_XOY = false,
				     const double translate_X = 0.0,   const double translate_Y = 0.0,   const double translate_Z = 0.0);
	
	void add_triangle(const int  v1 = 0, const int  v2 = 0, const int  v3 = 0,
					  const int vt1 = 0, const int vt2 = 0, const int vt3 = 0,
					  const int vn1 = 0, const int vn2 = 0, const int vn3 = 0);

private:
	//   Face information        information level
	//   vertex index                   0
	//   above + texture vertex         1
	//   above + vertex normal          2
	int                                        information_level;

	Matrix4                                    transformation;
	Matrix4                                    affine_matrix;

	std::vector<Point3>                        vertices;
	std::vector<Point3>                        texture_vertices;
	std::vector<Vector3>                       vertex_normals;

	std::vector<std::shared_ptr<Primitive>>*   primitives;

	std::shared_ptr<Material>                  material;
	std::shared_ptr<Texture>                   texture;
};