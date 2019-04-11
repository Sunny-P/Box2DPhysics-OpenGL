#include "Mesh.h"
#include "texture.h"
#include "Camera.h"

Mesh::Mesh()
{
	texture = new Texture("Resources/Textures/AwesomeFace.png");
	texture->init();
}

Mesh::~Mesh()
{
	delete texture;
}
