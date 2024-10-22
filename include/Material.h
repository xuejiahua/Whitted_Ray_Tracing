#pragma once

#include "Color.h"

class Material {
public:
	Material(double ambient_reflection_coefficient  = 0.0,
			 double diffuse_reflection_coefficient  = 0.0,
			 double specular_reflection_coefficient = 0.0,
			 double refraction_coefficient          = 0.0,
			 double specular_fuzz                   = 0.0,
			 double refractive_index                = 0.0,
			 double gloss                           = 0.0,
			 double emission_coefficient            = 0.0);
	Material(const Material& m);
	~Material();

	double get_ambient_reflection_coefficient()  const;
	double get_diffuse_reflection_coefficient()  const;
	double get_specular_reflection_coefficient() const;
	double get_refraction_coefficient()          const;
	double get_refractive_index()                const;
	double get_specular_fuzz()                   const;
	double get_gloss()                           const;
	double get_emission_coefficient()            const;

private:
	double ambient_reflection_coefficient;
	double diffuse_reflection_coefficient;          // Diffuse reflection  proportion of the surface to light.
	double specular_reflection_coefficient;         // Specular reflection proportion of the surface to lignt.
	double refraction_coefficient;                  // Refraction          proportion of the surface to light.

	double refractive_index;                        // Refractive index of the material.
	double specular_fuzz;
	double gloss;
	double emission_coefficient;
};