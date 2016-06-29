#include "preHeader.h"
#include "renderer/renderSystem.h"

using namespace TextureSynthesis;

int main(int argc, char** argv)
{

	CRenderSystem::Instance()->initRenderSystem();

	CRenderSystem::Instance()->render();

	CRenderSystem::Instance()->cleanSystem();

	return 0;
}