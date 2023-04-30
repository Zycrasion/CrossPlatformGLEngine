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
	Shader* shader;
	Camera* camera;
	Window* window;

public:
	StandardMaterial(Camera* cam, Window* win);
	~StandardMaterial();
	void bind();
	void SetDiffuse(Texture* texture);
};