#include "MeshStrider.h"

#include <OgreMesh.h>
#include <OgreSubMesh.h>

MeshStrider::MeshStrider(Ogre::Mesh* m) : mMesh(m)
{
}

void MeshStrider::set(Ogre::Mesh* m)
{
	if (m != nullptr)
		mMesh = m;
}

int MeshStrider::getNumSubParts() const
{
	int ret = mMesh->getNumSubMeshes();
	return ret;
}

void MeshStrider::getLockedReadOnlyVertexIndexBase(const unsigned char** vertexbase, int& numverts, PHY_ScalarType& type, int& stride, const unsigned char** indexbase, int& indexstride, int& numfaces, PHY_ScalarType& indicestype, int subpart/*=0*/) const
{
	Ogre::SubMesh* submesh = mMesh->getSubMesh(subpart);

	Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mMesh->sharedVertexData : submesh->vertexData;

	const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

	Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

	*vertexbase = reinterpret_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

	float* pReal;
	posElem->baseVertexPointerToElement((void*)*vertexbase, &pReal);
	*vertexbase = (unsigned char*)pReal;

	stride = (int)vbuf->getVertexSize();

	numverts = (int)vertex_data->vertexCount;

	type = PHY_FLOAT;

	Ogre::IndexData* index_data = submesh->indexData;
	Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

	if (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT) {
		indicestype = PHY_INTEGER;
	}
	else {
		indicestype = PHY_SHORT;
	}

	if (submesh->operationType == Ogre::RenderOperation::OT_TRIANGLE_LIST) {
		numfaces = (int)index_data->indexCount / 3;
		indexstride = (int)ibuf->getIndexSize() * 3;
	}
	else
		if (submesh->operationType == Ogre::RenderOperation::OT_TRIANGLE_STRIP) {
			numfaces = (int)index_data->indexCount - 2;
			indexstride = (int)ibuf->getIndexSize();
		}

	*indexbase = reinterpret_cast<unsigned char*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
}

void MeshStrider::getLockedVertexIndexBase(unsigned char** vertexbase, int& numverts, PHY_ScalarType& type, int& stride, unsigned char** indexbase, int& indexstride, int& numfaces, PHY_ScalarType& indicestype, int subpart/*=0*/)
{
}

void MeshStrider::unLockReadOnlyVertexBase(int subpart) const
{
	Ogre::SubMesh* submesh = mMesh->getSubMesh(subpart);

	Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mMesh->sharedVertexData : submesh->vertexData;

	const Ogre::VertexElement* posElem =
		vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

	Ogre::HardwareVertexBufferSharedPtr vbuf =
		vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

	vbuf->unlock();

	Ogre::IndexData* index_data = submesh->indexData;
	Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
	ibuf->unlock();
}

void MeshStrider::unLockVertexBase(int subpart)
{
}

void MeshStrider::preallocateVertices(int numverts)
{
}

void MeshStrider::preallocateIndices(int numindices)
{
}