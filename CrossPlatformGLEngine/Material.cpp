#include "Material.hpp"

StandardMaterial::StandardMaterial(Camera* cam, Window* window)
{
	this->camera = cam;
	this->window = window;
	this->shader = ShaderFromFiles("res/Shaders/3D_Lit.vert", "res/Shaders/3D_Lit.frag");
}

StandardMaterial::~StandardMaterial()
{
	delete this->shader;
}

void StandardMaterial::bind(Node* p_node)
{
	this->shader->use();
	this->shader->SetUniformInt("diffuse_texure", 0);
	this->shader->SetUniformMatrix4("projection", this->camera->GetProjection(this->window));
	this->shader->SetUniformMatrix4("model", p_node->GetTransform());
	this->shader->SetUniformMatrix4("view", this->camera->GetTransform());
	this->shader->SetUniformVec3("LightPosition", *this->LightPosition);
}

void StandardMaterial::SetDiffuse(Texture *texture)
{
	texture->Bind(0);
}

void StandardMaterial::BindLightPosition(glm::vec3* LightPosition)
{
	this->LightPosition = LightPosition;
}
