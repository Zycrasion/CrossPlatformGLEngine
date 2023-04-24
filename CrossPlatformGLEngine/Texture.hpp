#pragma
#include "External.hpp"

class Texture
{
private:
	unsigned int texture;
public:
	int width, height;

	Texture(const char* filename);
	~Texture();

	void Bind(int target);

	unsigned int GetTexture() { return this->texture; }
	operator unsigned int() { return this->texture; }
};