#include "ShaderTechniqueResolver.h"
#include "OgreTechnique.h"


ShaderTechniqueResolver::ShaderTechniqueResolver(Ogre::RTShader::ShaderGenerator* shaderGenerator) : shaderGenerator(shaderGenerator)
{

}


Ogre::Technique* ShaderTechniqueResolver::handleSchemeNotFound(unsigned short schemeIndex, const std::string& schemeName, Ogre::Material* originalMaterial, unsigned short lodIndex, const Ogre::Renderable* rend) {
    if (!shaderGenerator->hasRenderState(schemeName))
    {
        return nullptr;
    }
    // Case this is the default shader generator scheme.

    // Create shader generated technique for this material.
    bool techniqueCreated = shaderGenerator->createShaderBasedTechnique(
        *originalMaterial,
        Ogre::MaterialManager::DEFAULT_SCHEME_NAME,
        schemeName);

    if (!techniqueCreated)
    {
        return nullptr;
    }
    // Case technique registration succeeded.

    // Force creating the shaders for the generated technique.
    shaderGenerator->validateMaterial(schemeName, originalMaterial->getName(), originalMaterial->getGroup());

    // Grab the generated technique.
    Ogre::Material::Techniques::const_iterator it;
    for (it = originalMaterial->getTechniques().begin(); it != originalMaterial->getTechniques().end(); ++it)
    {
        Ogre::Technique* curTech = *it;

        if (curTech->getSchemeName() == schemeName)
        {
            return curTech;
        }
    }

    return nullptr;
}


bool ShaderTechniqueResolver::afterIlluminationPassesCreated(Ogre::Technique* tech)
{
    if (shaderGenerator->hasRenderState(tech->getSchemeName()))
    {
        Ogre::Material* mat = tech->getParent();
        shaderGenerator->validateMaterialIlluminationPasses(tech->getSchemeName(),
            mat->getName(), mat->getGroup());
        return true;
    }
    return false;
}


bool ShaderTechniqueResolver::beforeIlluminationPassesCleared(Ogre::Technique* tech)
{
    if (shaderGenerator->hasRenderState(tech->getSchemeName()))
    {
        Ogre::Material* mat = tech->getParent();
        shaderGenerator->invalidateMaterialIlluminationPasses(tech->getSchemeName(),
            mat->getName(), mat->getGroup());
        return true;
    }
    return false;
}