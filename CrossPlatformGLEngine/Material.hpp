#pragma once
#include "External.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Node.hpp"

class Material
{
public:
	virtual void bind(Node* p_node) = 0;
};

class StandardMaterial : public Material
{
private:
	Shader* shader;
	Camera* camera;
	Window* window;
	glm::vec3* LightPosition;

public:
	StandardMaterial(Camera* cam, Window* win);
	~StandardMaterial();
	void bind(Node* p_node);
	void SetDiffuse(Texture* texture);
	void BindLightPosition(glm::vec3* LightPosition);
};