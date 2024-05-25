#include "Model.h"
OBJ_Viewer::Model::Model(std::vector<std::shared_ptr<Mesh>> meshes, ModelData data) :m_data(data)
{
	m_meshes = meshes;
}
