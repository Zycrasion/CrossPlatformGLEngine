#include "Material.hpp"

StandardMaterial::StandardMaterial(Camera* cam, Window* window)
{

}

StandardMaterial::~StandardMaterial()
{
	delete this->shader;
}

void StandardMaterial::bind()
{
	this->shader->use();
	this->shader->SetUniformInt("diffuse_texure", 0);
	this->shader->SetUniformMatrix4("projection", this->camera->GetProjection(this->window));
}

void StandardMaterial::SetDiffuse(Texture *texture)
{
	texture->Bind(0);
}
