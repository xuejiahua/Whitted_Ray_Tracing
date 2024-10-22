#include "../include/Material.h"

#include "../include/utilities.h"

Material::Material(double ambient_reflection_coefficient, 
				   double diffuse_reflection_coefficient, 
				   double specular_reflection_coefficient,
				   double refraction_coefficient, 
				   double specular_fuzz,
				   double refractive_index, 
				   double gloss, 
				   double emission_coefficient) {
	this->ambient_reflection_coefficient  = clamp(ambient_reflection_coefficient, 0.0, 1.0);
	this->diffuse_reflection_coefficient  = clamp(diffuse_reflection_coefficient, 0.0, 1.0);
	this->specular_reflection_coefficient = clamp(specular_reflection_coefficient, 0.0, 1.0);
	this->refraction_coefficient          = clamp(refraction_coefficient, 0.0, 1.0);
	this->specular_fuzz                   = clamp(specular_fuzz, 0.0, 1.0);
	this->refractive_index                = refractive_index;
	this->gloss                           = clamp(gloss, 0.0, INF);
	this->emission_coefficient            = clamp(emission_coefficient, 0.0, INF);
}

Material::Material(const Material& m) {
	this->ambient_reflection_coefficient  = m.ambient_reflection_coefficient;
	this->diffuse_reflection_coefficient  = m.diffuse_reflection_coefficient;
	this->specular_reflection_coefficient = m.specular_reflection_coefficient;
	this->refraction_coefficient          = m.refraction_coefficient;
	this->specular_fuzz                   = m.specular_fuzz;
	this->refractive_index                = m.refractive_index;
	this->gloss                           = m.gloss;
	this->emission_coefficient            = m.emission_coefficient;
}

Material::~Material() {
}

double Material::get_ambient_reflection_coefficient() const {
	return this->ambient_reflection_coefficient;
}

double Material::get_diffuse_reflection_coefficient() const {
	return this->diffuse_reflection_coefficient;
}

double Material::get_specular_reflection_coefficient() const {
	return this->specular_reflection_coefficient;
}

double Material::get_refraction_coefficient() const {
	return this->refraction_coefficient;
}

double Material::get_refractive_index() const {
	return this->refractive_index;
}

double Material::get_specular_fuzz() const {
	return this->specular_fuzz;
}

double Material::get_gloss() const {
	return this->gloss;
}

double Material::get_emission_coefficient() const {
	return this->emission_coefficient;
}