#pragma once

#ifndef ES_CORE_RENDERER_OPENGL21_H
#define ES_CORE_RENDERER_OPENGL21_H

#if defined(USE_OPENGL_21)

#define RENDERER_OPENGL_21

#include "Renderer.h"

namespace Renderer
{
	class OpenGL21Renderer : public IRenderer
	{
	public:
		std::string getDriverName() override;
		std::vector<std::pair<std::string, std::string>> getDriverInformation() override;

		unsigned int getWindowFlags() override;
		void         setupWindow() override;

		void         createContext() override;
		void         destroyContext() override;

		void		 resetCache() override;

		unsigned int createTexture(const Texture::Type _type, const bool _linear, const bool _repeat, const unsigned int _width, const unsigned int _height, void* _data) override;
		void         destroyTexture(const unsigned int _texture) override;
		void         updateTexture(const unsigned int _texture, const Texture::Type _type, const unsigned int _x, const unsigned _y, const unsigned int _width, const unsigned int _height, void* _data) override;
		void         bindTexture(const unsigned int _texture) override;

		void         drawLines(const Vertex* _vertices, const unsigned int _numVertices, const Blend::Factor _srcBlendFactor = Blend::SRC_ALPHA, const Blend::Factor _dstBlendFactor = Blend::ONE_MINUS_SRC_ALPHA) override;
		void         drawTriangleStrips(const Vertex* _vertices, const unsigned int _numVertices, const Blend::Factor _srcBlendFactor = Blend::SRC_ALPHA, const Blend::Factor _dstBlendFactor = Blend::ONE_MINUS_SRC_ALPHA, bool verticesChanged = true) override;
		void		 drawTriangleFan(const Vertex* _vertices, const unsigned int _numVertices, const Blend::Factor _srcBlendFactor = Blend::SRC_ALPHA, const Blend::Factor _dstBlendFactor = Blend::ONE_MINUS_SRC_ALPHA) override;
		void		 drawSolidRectangle(const float _x, const float _y, const float _w, const float _h, const unsigned int _fillColor, const unsigned int _borderColor, float borderWidth = 1, float cornerRadius = 0) override;

		void         setProjection(const Transform4x4f& _projection) override;
		void         setMatrix(const Transform4x4f& _matrix) override;
		void         setViewport(const Rect& _viewport) override;
		void         setScissor(const Rect& _scissor) override;

		void         setStencil(const Vertex* _vertices, const unsigned int _numVertices) override;
		void		 disableStencil() override;

		void         setSwapInterval() override;
		void         swapBuffers() override;
	};
}

#endif // USE_OPENGL_21_OLD

#endif