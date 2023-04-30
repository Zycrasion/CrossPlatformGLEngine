#pragma once
#include "External.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

class Material
{
public:
	virtual void bind() = 0;
};

class StandardMaterial : public Material
{
private:
	Texture* Diffuse;
	Shader* shader;
	Camera* camera;

public:
	StandardMaterial();
	~StandardMaterial();
	void bind();
};