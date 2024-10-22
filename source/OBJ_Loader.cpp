#include "../include/OBJ_Loader.h"

#include <fstream>
#include <istream>
#include <sstream>

OBJ_Loader::OBJ_Loader(std::vector<std::shared_ptr<Primitive>>* p) {
	this->primitives = p;
}

OBJ_Loader::~OBJ_Loader() {

}

void OBJ_Loader::configurate(std::shared_ptr<Material> m,
							 std::shared_ptr<Texture> t,
							 const double scale_X,     const double scale_Y,     const double scale_Z, 
							 const double shear_XY,    const double shear_XZ,
							 const double shear_YX,    const double shear_YZ, 
							 const double shear_ZX,    const double shear_ZY,
							 const double rotate_X,    const double rotate_Y,    const double rotate_Z,
							 const bool   reflect_X,   const bool   reflect_Y,   const bool   reflect_Z,
							 const bool   reflect_YOZ, const bool   reflect_XOZ, const bool   reflect_XOY, 
							 const double translate_X, const double translate_Y, const double translate_Z) {
	this->material = m;
	this->texture  = t;

	affine_matrix.indentify();

	// handle the "scale_*" parameters
	if (   fabs(scale_X - 1.0) > TOLERANCE
		|| fabs(scale_Y - 1.0) > TOLERANCE
		|| fabs(scale_Z - 1.0) > TOLERANCE) {
		transformation.indentify();                                                  // transformation = [scale_X,       0,       0, 0]
		transformation.set_item(0, 0, scale_X);                                      //                  [      0, scale_Y,       0, 0]
		transformation.set_item(1, 1, scale_Y);                                      //                  [      0,       0, scale_Z, 0]
		transformation.set_item(2, 2, scale_Z);                                      //                  [      0,       0,       0, 1]
		affine_matrix = transformation * affine_matrix;
	}

	// handle the "shear_**" parameters
	if (   fabs(shear_XY) > TOLERANCE
		|| fabs(shear_XZ) > TOLERANCE
		|| fabs(shear_YX) > TOLERANCE
		|| fabs(shear_YZ) > TOLERANCE
		|| fabs(shear_ZX) > TOLERANCE
		|| fabs(shear_ZY) > TOLERANCE) {
		transformation.indentify();                                                  // transformation = [       1, shear_YX, shear_ZX, 0]
	    transformation.set_item(0, 1, shear_YX);                                     //                  [shear_XY,        1, shear_ZY, 0]
		transformation.set_item(0, 2, shear_ZX);                                     //                  [shear_XZ, shear_YZ,        1, 0]
		transformation.set_item(1, 1, shear_XY);                                     //                  [       0,        0,        0, 1]
		transformation.set_item(1, 2, shear_ZY);
		transformation.set_item(2, 1, shear_XZ);
		transformation.set_item(2, 2, shear_YZ);
		affine_matrix = transformation * affine_matrix;
	}

	// handle the "rotate_X" parameter
	if (fabs(rotate_X) > TOLERANCE) {
		double rotate_degree = rotate_X * PI / 180.0;
		transformation.indentify();                                                  // transformation = [1,          0,           0, 0]
		transformation.set_item(1, 1,  cos(rotate_degree));                          //                  [0, cos(angle), -sin(angle), 0]
		transformation.set_item(1, 2, -sin(rotate_degree));                          //                  [0, sin(angle),  cos(angle), 0]
		transformation.set_item(2, 1,  sin(rotate_degree));                          //                  [0,          0,           0, 1]
		transformation.set_item(2, 2,  cos(rotate_degree));
		affine_matrix = transformation * affine_matrix;
	}

	// handle the "rotate_Y" parameter
	if (fabs(rotate_Y) > TOLERANCE) {
		double rotate_degree = rotate_Y * PI / 180.0;
		transformation.indentify();                                                  // transformation = [ cos(angle), 0, sin(angle), 0]
		transformation.set_item(0, 0,  cos(rotate_degree));                          //                  [          0, 1,          0, 0]
		transformation.set_item(0, 2,  sin(rotate_degree));                          //                  [-sin(angle], 0, cos(angle), 0]
		transformation.set_item(2, 0, -sin(rotate_degree));                          //                  [          0, 0,          0, 1]
		transformation.set_item(2, 2,  cos(rotate_degree));
		affine_matrix = transformation * affine_matrix;
	}

	// handle the "rotate_Z" parameter
	if (fabs(rotate_Z) > TOLERANCE) {
		double rotate_degree = rotate_Z * PI / 180.0;
		transformation.indentify();                                                  // transformation = [cos(angle), -sin(angle), 0, 0]
		transformation.set_item(0, 0,  cos(rotate_degree));                          //                  [sin(angle),  cos(angle), 0, 0]
		transformation.set_item(0, 1, -sin(rotate_degree));                          //                  [         0,           0, 1, 0]
		transformation.set_item(1, 0,  sin(rotate_degree));                          //                  [         0,           0, 0, 1]
		transformation.set_item(1, 1,  cos(rotate_degree));
		affine_matrix = transformation * affine_matrix;
	}


	// handle the "reflect_X" parameter
	if (reflect_X) {
		transformation.indentify();                                                  // transformation = [1,  0,  0, 0]
		transformation.set_item(1, 1, -1.0);                                         //                  [0, -1,  0, 0]
		transformation.set_item(2, 2, -1.0);                                         //                  [0,  0, -1, 0]
		affine_matrix = transformation * affine_matrix;                              //                  [0,  0,  0, 1]
	}

	// handle the "reflect_Y" parameter
	if (reflect_Y) {
		transformation.indentify();                                                  // transformation = [-1, 0,  0, 0]
		transformation.set_item(0, 0, -1.0);                                         //                  [ 0, 1,  0, 0]
		transformation.set_item(2, 2, -1.0);                                         //                  [ 0, 0, -1, 0]
		affine_matrix = transformation * affine_matrix;                              //                  [ 0, 0,  0, 1]
	}

	// handle the "reflect_Z" parameter
	if (reflect_Z) {
		transformation.indentify();                                                  // transformation = [-1,  0, 0, 0]
		transformation.set_item(0, 0, -1.0);                                         //                  [ 0, -1, 0, 0]
		transformation.set_item(1, 1, -1.0);                                         //                  [ 0,  0, 1, 0]
		affine_matrix = transformation * affine_matrix;                              //                  [ 0,  0, 0, 1]
	}

	// handle the "reflect_YOZ" parameter
	if (reflect_YOZ) {
		transformation.indentify();                                                  // transformation = [-1, 0, 0, 0]
		transformation.set_item(0, 0, -1.0);                                         //                  [ 0, 1, 0, 0]
		                                                                             //                  [ 0, 0, 1, 0]
		affine_matrix = transformation * affine_matrix;                              //                  [ 0, 0, 0, 1]
	}

	// handle the "reflect_XOZ" parameter
	if (reflect_XOZ) {
		transformation.indentify();                                                  // transformation = [1,  0, 0, 0]
		transformation.set_item(1, 1, -1.0);                                         //                  [0, -1, 0, 0]
																					 //                  [0,  0, 1, 0]
		affine_matrix = transformation * affine_matrix;                              //                  [0,  0, 0, 1]
	}

	// handle the "reflect_XOY" parameter
	if (reflect_XOY) {
		transformation.indentify();                                                  // transformation = [1, 0,  0, 0]
		transformation.set_item(2, 2, -1.0);                                         //                  [0, 1,  0, 0]
																					 //                  [0, 0, -1, 0]
		affine_matrix = transformation * affine_matrix;                              //                  [0, 0,  0, 1]
	}

	// handle the "translate_*" parameters
	if (   fabs(translate_X) > TOLERANCE 
		|| fabs(translate_Y) > TOLERANCE 
		|| fabs(translate_Z) > TOLERANCE) {
		transformation.indentify();                                                  // transformation = [1, 0, 0, translate_X]
		transformation.set_item(0, 3, translate_X);                                  //                  [0, 1, 0, translate_Y]
		transformation.set_item(1, 3, translate_Y);                                  //                  [0, 0, 1, translate_Z]
		transformation.set_item(2, 3, translate_Z);                                  //                  [0, 0, 0,           1]
		affine_matrix = transformation * affine_matrix;
	}
}

void OBJ_Loader::read_obj(const char* obj_filepath, const int information_level,
						  std::shared_ptr<Material> m,
						  std::shared_ptr<Texture> t,
						  const double scale_X,     const double scale_Y,     const double scale_Z, 
						  const double shear_XY,    const double shear_XZ,
						  const double shear_YX,    const double shear_YZ, 
						  const double shear_ZX,    const double shear_ZY,
						  const double rotate_X,    const double rotate_Y,    const double rotate_Z,
						  const bool   reflect_X,   const bool   reflect_Y,   const bool   reflect_Z,
						  const bool   reflect_YOZ, const bool   reflect_XOZ, const bool   reflect_XOY, 
						  const double translate_X, const double translate_Y, const double translate_Z) {
	this->information_level = information_level;
	this->configurate(m, t,
					  scale_X,     scale_Y,     scale_Z,
					  shear_XY,    shear_XZ,
					  shear_YX,    shear_YZ,
					  shear_ZX,    shear_ZY,
					  rotate_X,    rotate_Y,    rotate_Z,
					  reflect_X,   reflect_Y,   reflect_Z,
					  reflect_YOZ, reflect_XOZ, reflect_XOY,
					  translate_X, translate_Y, translate_Z);

    std::ifstream file(obj_filepath);
    std::string line;
    if (!file.is_open()) {
        std::cout << "ERROR::FILE\"" << obj_filepath << "\"FAILED TO OPEN." << std::endl;
        exit(-1);
    }

    while (std::getline(file, line)) {
        std::istringstream sin(line);
        std::string type;
        double x, y, z;
        int v1 = -1, vt1 = -1, vn1 = -1;
        int v2 = -1, vt2 = -1, vn2 = -1;
        int v3 = -1, vt3 = -1, vn3 = -1;
        char slash;

        sin >> type;
        if (type == "v") {
            sin >> x >> y >> z;
            this->vertices.push_back(Point3(x, y, z));
        }
        if (type == "vt") {
            sin >> x >> y;
            this->texture_vertices.push_back(Point3(x, y, 0.0));
        }
        if (type == "vn") {
            sin >> x >> y >> z;
            this->vertex_normals.push_back(Vector3(x, y, z));
        }
        if (type == "f") {
            switch (this->information_level) {
                case 0:
                    sin >> v1;
                    sin >> v2;
                    sin >> v3;
                    this->add_triangle(v1, v2, v3);
                    break;
                case 1:
                    sin >> v1 >> slash >> vt1;
                    sin >> v2 >> slash >> vt2;
                    sin >> v3 >> slash >> vt3;
                    this->add_triangle(v1, v2, v3,
                                       vt1, vt2, vt3);
                    break;
				case 2:
					sin >> v1 >> slash >> slash >> vn1;
					sin >> v2 >> slash >> slash >> vn2;
					sin >> v3 >> slash >> slash >> vn3;
					this->add_triangle(v1, v2, v3,
									   vt1, vt2, vt3,
									   vn1, vn2, vn3);
					break;
                case 3:
                    sin >> v1 >> slash >> vt1 >> slash >> vn1;
                    sin >> v2 >> slash >> vt2 >> slash >> vn2;
                    sin >> v3 >> slash >> vt3 >> slash >> vn3;
                    this->add_triangle(v1, v2, v3,
                                       vt1, vt2, vt3,
                                       vn1, vn2, vn3);
                    break;
            }
        }
    }

	file.close();
    this->vertices.clear();
    this->texture_vertices.clear();
    this->vertex_normals.clear();
}

void OBJ_Loader::add_triangle(const int  v1, const int  v2, const int  v3, 
							  const int vt1, const int vt2, const int vt3, 
							  const int vn1, const int vn2, const int vn3) {
	switch (this->information_level) {
		case 0: {
			this->primitives->push_back(std::make_shared<Triangle>(this->affine_matrix.affine_transform(this->vertices[v1 - 1]),
																   this->affine_matrix.affine_transform(this->vertices[v2 - 1]),
																   this->affine_matrix.affine_transform(this->vertices[v3 - 1]),
																   this->material,
																   this->texture));
			break;
		}
		case 1:	{
			this->primitives->push_back(std::make_shared<Triangle>(this->vertices[v1 - 1],
																   this->vertices[v2 - 1],
																   this->vertices[v3 - 1],
																   this->texture_vertices[vt1 - 1],
																   this->texture_vertices[vt2 - 1],
																   this->texture_vertices[vt3 - 1],
																   this->material,
																   this->texture));
			break;
		}
		case 2: {
			this->primitives->push_back(std::make_shared<Triangle>(this->vertices[v1 - 1],
																   this->vertices[v2 - 1],
																   this->vertices[v3 - 1],
																   Point3(),
																   Point3(),
																   Point3(),
																   this->vertex_normals[vn1 - 1],
																   this->vertex_normals[vn2 - 1],
																   this->vertex_normals[vn3 - 1],
																   this->material,
																   this->texture));
			break;
		}
		case 3: {
			this->primitives->push_back(std::make_shared<Triangle>(this->vertices[v1 - 1],
																   this->vertices[v2 - 1],
																   this->vertices[v3 - 1],
																   this->texture_vertices[vt1 - 1],
																   this->texture_vertices[vt2 - 1],
																   this->texture_vertices[vt3 - 1],
																   this->vertex_normals[vn1 - 1],
																   this->vertex_normals[vn2 - 1],
																   this->vertex_normals[vn3 - 1],
																   this->material,
																   this->texture));
			break;
		}
           
    }
}

