#pragma once

#ifndef MESH_STRIDER_H
#define MESH_STRIDER_H

#include <BulletCollision/CollisionShapes/btStridingMeshInterface.h>

namespace Ogre {
	class Mesh;
}

/// Shares vertices/indexes between Ogre and Bullet
class MeshStrider : public btStridingMeshInterface {

public:
	MeshStrider(Ogre::Mesh* m);

	void set(Ogre::Mesh* m);

	// inherited interface
	virtual int getNumSubParts() const;

	virtual void getLockedVertexIndexBase(unsigned char** vertexbase, int& numverts, PHY_ScalarType& type, int& stride, unsigned char** indexbase, int& indexstride, int& numfaces, PHY_ScalarType& indicestype, int subpart = 0);
	virtual void getLockedReadOnlyVertexIndexBase(const unsigned char** vertexbase, int& numverts, PHY_ScalarType& type, int& stride, const unsigned char** indexbase, int& indexstride, int& numfaces, PHY_ScalarType& indicestype, int subpart = 0) const;

	virtual void unLockVertexBase(int subpart);
	virtual void unLockReadOnlyVertexBase(int subpart) const;

	virtual void preallocateVertices(int numverts);
	virtual void preallocateIndices(int numindices);
private:
	Ogre::Mesh* mMesh;
};

#endif