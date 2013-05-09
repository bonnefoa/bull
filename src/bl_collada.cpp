#include "bl_collada.h"
#include <bl_log.h>

ColladaWriter::ColladaWriter()
{

}

ColladaWriter::~ColladaWriter()
{

}


bool ColladaWriter::readFile(const char* filename)
{
        COLLADASaxFWL::Loader loader;
        COLLADAFW::Root root(&loader, this);
        loader.setObjectFlags(COLLADASaxFWL::Loader::ASSET_FLAG
                        | COLLADASaxFWL::Loader::EFFECT_FLAG
                        | COLLADASaxFWL::Loader::MATERIAL_FLAG
                        | COLLADASaxFWL::Loader::LIBRARY_NODES_FLAG
                        | COLLADASaxFWL::Loader::VISUAL_SCENES_FLAG
                        | COLLADASaxFWL::Loader::SCENE_FLAG
                        | COLLADASaxFWL::Loader::GEOMETRY_FLAG);
        if ( !root.loadDocument(filename) )
                return false;

        return true;
}

void ColladaWriter::load()
{

}

void ColladaWriter::cancel(const COLLADAFW::String& errorMessage)
{
        std::cout << errorMessage;
        INFO("Collada writer cancel : %s\n", errorMessage.c_str());
}

void ColladaWriter::start()
{
        INFO("Collada writer start\n");
}

void ColladaWriter::finish()
{
        INFO("Collada writer finished\n");
}

bool ColladaWriter::writeGlobalAsset(const COLLADAFW::FileInfo* asset)
{
        (void)asset;
        return true;
}

bool ColladaWriter::writeScene(const COLLADAFW::Scene* scene)
{
        (void)scene;
        return true;
}

bool ColladaWriter::writeVisualScene(const COLLADAFW::VisualScene* visualScene)
{
        (void)visualScene;
        return true;
}

bool ColladaWriter::writeLibraryNodes(const COLLADAFW::LibraryNodes* libraryNodes)
{
        (void)libraryNodes;
        return true;
}

bool ColladaWriter::writeGeometry(const COLLADAFW::Geometry* geometry)
{
        (void)geometry;
        INFO("Got geometry %s\n", geometry->getName().c_str());
        return true;
}

bool ColladaWriter::writeMaterial(const COLLADAFW::Material* material)
{
        (void)material;
        return true;
}

bool ColladaWriter::writeEffect(const COLLADAFW::Effect* effect)
{
        (void)effect;
        return true;
}

bool ColladaWriter::writeCamera(const COLLADAFW::Camera* camera)
{
        (void)camera;
        return true;
}

bool ColladaWriter::writeImage(const COLLADAFW::Image* image)
{
        (void)image;
        return true;
}

bool ColladaWriter::writeLight(const COLLADAFW::Light* light)
{
        (void)light;
        return true;
}

bool ColladaWriter::writeAnimation(const COLLADAFW::Animation* animation)
{
        (void)animation;
        return true;
}

bool ColladaWriter::writeAnimationList(const COLLADAFW::AnimationList* animationList)
{
        (void)animationList;
        return true;
}

bool ColladaWriter::writeSkinControllerData(const COLLADAFW::SkinControllerData* skinControllerData)
{
        (void)skinControllerData;
        return true;
}

bool ColladaWriter::writeController(const COLLADAFW::Controller* controller)
{
        (void)controller;
        return true;
}

bool ColladaWriter::writeFormulas(const COLLADAFW::Formulas* formulas)
{
        (void)formulas;
        return true;
}

bool ColladaWriter::writeKinematicsScene(const COLLADAFW::KinematicsScene* kinematicsScene)
{
        (void)kinematicsScene;
        return true;
}
