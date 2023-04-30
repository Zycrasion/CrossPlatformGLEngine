#include "Material.hpp"

StandardMaterial::StandardMaterial()
{

}

StandardMaterial::~StandardMaterial()
{
	this->Diffuse->~Texture();
}

void StandardMaterial::bind()
{
	this->shader->use();
	this->shader->SetUniformInt("diffuse_texure", this->Diffuse->Bind(0));
	// this->shader->SetUniformMatrix4("projection", this->camera->GetProjection());
}