#pragma once

#ifndef SHADER_TECHNIQUE_RESOLVER_H
#define SHADER_TECHNIQUE_RESOLVER_H

#include "OgreRTShaderSystem.h"
#include "OgreMaterialManager.h"

class ShaderTechniqueResolver : public Ogre::MaterialManager::Listener
{
public:
    explicit ShaderTechniqueResolver(Ogre::RTShader::ShaderGenerator* shaderGenerator);

    Ogre::Technique* handleSchemeNotFound(unsigned short schemeIndex,
        const std::string& schemeName,
        Ogre::Material* originalMaterial, unsigned short lodIndex,
        const Ogre::Renderable* rend);

    bool afterIlluminationPassesCreated(Ogre::Technique* tech);

    bool beforeIlluminationPassesCleared(Ogre::Technique* tech);

protected:
    Ogre::RTShader::ShaderGenerator* shaderGenerator; // The shader generator instance.
};


#endif
