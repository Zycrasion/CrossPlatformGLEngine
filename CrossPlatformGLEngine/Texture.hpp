#pragma once
#include "External.hpp"

class Texture
{
private:
	unsigned int texture;
public:
	int width, height;

	Texture(const char* filename);
	Texture(int width, int height);
	~Texture();

	void Bind(int target);

	unsigned int GetTexture() { return this->texture; }
	operator unsigned int() { return this->texture; }
};

class Framebuffer
{
private:
	Texture* RenderTexture;
	unsigned int framebuffer;
public:
	int width, height;

	Framebuffer(int width, int height);

	Texture* GetRenderTexture() { return this->RenderTexture; }
	unsigned int GetTexture() { return this->RenderTexture->GetTexture(); }
	void Bind();
	void Unbind();
};