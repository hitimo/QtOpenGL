#include "openglspotlightgroup.h"

#include <KMath>
#include <KMatrix4x4>
#include <OpenGLMesh>
#include <OpenGLElementType>
#include <OpenGLUniformBufferObject>
#include <OpenGLRenderBlock>

void OpenGLSpotLightGroup::initializeMesh(OpenGLMesh &mesh)
{
  mesh.vertexAttribPointerDivisor(1, 4,     OpenGLElementType::Float, false, sizeof(DataType), DataType::TranslationOffset() , 1);
  mesh.vertexAttribPointerDivisor(2, 4,     OpenGLElementType::Float, false, sizeof(DataType), DataType::DirectionOffset()   , 1);
  mesh.vertexAttribPointerDivisor(4, 4,     OpenGLElementType::Float, false, sizeof(DataType), DataType::AttenuationOffset() , 1);
  mesh.vertexAttribPointerDivisor(5, 4,     OpenGLElementType::Float, false, sizeof(DataType), DataType::DiffuseOffset()     , 1);
  mesh.vertexAttribPointerDivisor(6, 3,     OpenGLElementType::Float, false, sizeof(DataType), DataType::SpecularOffset()    , 1);
  mesh.vertexAttribPointerDivisor(7, 4, 4,  OpenGLElementType::Float, false, sizeof(DataType), DataType::PerpectiveOffset()  , 1);
}

void OpenGLSpotLightGroup::translateBuffer(const OpenGLRenderBlock &stats, DataPointer data, ConstLightIterator begin, ConstLightIterator end)
{
  // Upload data to GPU
  DataPointer lightDest;
  ConstLightPointer lightSource;
  while (begin != end)
  {
    lightDest   = data;
    lightSource = *begin;
    lightDest->m_innerAngle   = lightSource->innerAngle();
    lightDest->m_outerAngle   = lightSource->outerAngle();
    lightDest->m_diffAngle    = lightSource->outerAngle() - lightSource->innerAngle();
    lightDest->m_attenuation  = Karma::ToGlm(lightSource->attenuation(), lightSource->depth());
    lightDest->m_diffuse      = Karma::ToGlm(lightSource->diffuse());
    lightDest->m_direction    = glm::vec3(glm::normalize(stats.worldToView() * Karma::ToGlm(lightSource->direction(), 0.0f)));
    lightDest->m_perspTrans   = stats.worldToPersp() * Karma::ToGlm(lightSource->toMatrix());
    lightDest->m_specular     = Karma::ToGlm(lightSource->specular());
    lightDest->m_viewTrans    = glm::vec3(stats.worldToView() * Karma::ToGlm(lightSource->translation(), 1.0f));
    ++data;
    ++begin;
  }
}

void OpenGLSpotLightGroup::translateUniforms(const OpenGLRenderBlock &stats, Byte *data, OpenGLLightGroup::SizeType step, OpenGLLightGroup::ConstLightIterator begin, OpenGLLightGroup::ConstLightIterator end)
{
  // Upload data to GPU
  DataPointer lightDest;
  ConstLightPointer lightSource;
  while (begin != end)
  {
    lightDest   = reinterpret_cast<DataType*>(data);
    lightSource = *begin;
    lightDest->m_innerAngle   = lightSource->innerAngle();
    lightDest->m_outerAngle   = lightSource->outerAngle();
    lightDest->m_diffAngle    = lightSource->outerAngle() - lightSource->innerAngle();
    lightDest->m_attenuation  = Karma::ToGlm(lightSource->attenuation(), lightSource->depth());
    lightDest->m_diffuse      = Karma::ToGlm(lightSource->diffuse());
    lightDest->m_direction    = glm::vec3(glm::normalize(stats.worldToView() * Karma::ToGlm(lightSource->direction(), 0.0f)));
    lightDest->m_perspTrans   = stats.worldToPersp() * Karma::ToGlm(lightSource->toMatrix());
    lightDest->m_specular     = Karma::ToGlm(lightSource->specular());
    lightDest->m_viewTrans    = glm::vec3(stats.worldToView() * Karma::ToGlm(lightSource->translation(), 1.0f));
    data += step;
    ++begin;
  }
}