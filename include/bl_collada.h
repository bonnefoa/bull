#ifndef BL_COLLADA_H
#define BL_COLLADA_H

#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-Wunused-parameter"

#include "COLLADAFW.h"
#include <COLLADASaxFWLLoader.h>

#pragma GCC diagnostic pop


class ColladaWriter : public COLLADAFW::IWriter
{
        public:
		ColladaWriter();
		virtual ~ColladaWriter();

                bool readFile(const char *filename);
                virtual void load();
                virtual void cancel(const COLLADAFW::String& errorMessage);
                virtual void start();
                virtual void finish();
                virtual bool writeGlobalAsset ( const COLLADAFW::FileInfo* asset );
                virtual bool writeScene ( const COLLADAFW::Scene* scene );
                virtual bool writeVisualScene ( const COLLADAFW::VisualScene* visualScene );
                virtual bool writeLibraryNodes ( const COLLADAFW::LibraryNodes* libraryNodes );
                virtual bool writeGeometry ( const COLLADAFW::Geometry* geometry );
                virtual bool writeMaterial( const COLLADAFW::Material* material );
                virtual bool writeEffect( const COLLADAFW::Effect* effect );
                virtual bool writeCamera( const COLLADAFW::Camera* camera ); virtual bool writeImage( const COLLADAFW::Image* image );
                virtual bool writeLight( const COLLADAFW::Light* light );
                virtual bool writeAnimation( const COLLADAFW::Animation* animation );
                virtual bool writeAnimationList( const COLLADAFW::AnimationList* animationList );
                virtual bool writeSkinControllerData( const COLLADAFW::SkinControllerData* skinControllerData );
                virtual bool writeController( const COLLADAFW::Controller* controller );
                virtual bool writeFormulas( const COLLADAFW::Formulas* formulas );
                virtual bool writeKinematicsScene( const COLLADAFW::KinematicsScene* kinematicsScene );

};

#endif
