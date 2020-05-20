#pragma once

#include <CEGUI\CEGUI.h>
#include <Ogre.h>


class BrightnessEffect : public CEGUI::RenderEffect
{
public:
	BrightnessEffect(CEGUI::Window* window);
	~BrightnessEffect();

	virtual int getPassCount() const;
	virtual void performPreRenderFunctions(const int pass);
	virtual void performPostRenderFunctions();
	virtual bool realiseGeometry(CEGUI::RenderingWindow& window, CEGUI::GeometryBuffer& geometry);
	virtual bool update(const float elapsed, CEGUI::RenderingWindow& window);

private:
	CEGUI::Window* window;
	Ogre::Root* root;
	Ogre::Material* material;
	std::shared_ptr<Ogre::GpuProgram> vertexShader;
	std::shared_ptr<Ogre::GpuProgram> fragmentShader;
	float* brightness;
};

