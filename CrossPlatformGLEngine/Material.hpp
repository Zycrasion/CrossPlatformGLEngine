#pragma once
#include "External.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Node.hpp"

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
	Node* p_Node;

public:
	StandardMaterial(Camera* cam, Window* win, Node* p_Node);
	~StandardMaterial();
	void bind();
	void SetDiffuse(Texture* texture);
};