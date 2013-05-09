#include "bl_collada.h"

void ColladaWriter::doIt()
{
        COLLADASaxFWL::Loader loader;
        COLLADAFW::Root root(&loader, this);
}

void ColladaWriter::load()
{

}

void ColladaWriter::cancel(const COLLADAFW::String& errorMessage)
{

}

void ColladaWriter::start()
{

}

void ColladaWriter::finish()
{

}

bool ColladaWriter::writeGlobalAsset(const COLLADAFW::FileInfo* asset)
{
        return true;
}

bool ColladaWriter::writeScene(const COLLADAFW::Scene* scene)
{
        return true;
}

bool ColladaWriter::writeVisualScene(const COLLADAFW::VisualScene* visualScene)
{
        return true;
}

bool ColladaWriter::writeLibraryNodes(const COLLADAFW::LibraryNodes* libraryNodes)
{
        return true;
}

bool ColladaWriter::writeGeometry(const COLLADAFW::Geometry* geometry)
{
        return true;
}

bool ColladaWriter::writeMaterial(const COLLADAFW::Material* material)
{
        return true;
}

bool ColladaWriter::writeEffect(const COLLADAFW::Effect* effect)
{
        return true;
}

bool ColladaWriter::writeCamera(const COLLADAFW::Camera* camera)
{
        return true;
}

bool ColladaWriter::writeImage(const COLLADAFW::Image* image)
{
        return true;
}

bool ColladaWriter::writeLight(const COLLADAFW::Light* light)
{
        return true;
}

bool ColladaWriter::writeAnimation(const COLLADAFW::Animation* animation)
{
        return true;
}

bool ColladaWriter::writeAnimationList(const COLLADAFW::AnimationList* animationList)
{
        return true;
}

bool ColladaWriter::writeSkinControllerData(const COLLADAFW::SkinControllerData* skinControllerData)
{
        return true;
}

bool ColladaWriter::writeController(const COLLADAFW::Controller* controller)
{
        return true;
}

bool ColladaWriter::writeFormulas(const COLLADAFW::Formulas* formulas)
{
        return true;
}

bool ColladaWriter::writeKinematicsScene(const COLLADAFW::KinematicsScene* kinematicsScene)
{
        return true;
}
