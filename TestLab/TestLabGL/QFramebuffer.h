/****************************************************************************************
** QFramebuffer.h by KQL2 (ltqsoft@gmail.com)
** An OpenGL (textured) framebuffer with 2
** (texture) color buffers and a depth-stencil buffer.
****************************************************************************************/
#pragma once
#include "qglew.h"
///#include <cstdint>

#define QE_COLOR_ATTCH0		0
#define QE_COLOR_ATTCH0		1
#define QE_DEPTH_STENCIL	2

struct TexAttachment
{
	GLuint colorBuffers_[2] = { 0, 0 };	//$ Color buffer textures.
	GLuint dephStencilBuffer_;			//$ Stencil attachment not used yet.
};


class QFramebuffer
{
public:
	QFramebuffer() = default;
	QFramebuffer(uint32_t wWidth, uint32_t wHeight);
	~QFramebuffer();

	inline void bind() { glBindFramebuffer(GL_FRAMEBUFFER, id_); }
	inline void bindDefault() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
	inline void bindColorBuffer(uint8_t wAttachment, GLenum wUnit)
	{
		/*/
		if (wAttachment) glActiveTexture(GL_TEXTURE1);
		else glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorBuffers_[wAttachment]);
		/*/
		glActiveTexture(wUnit);
		glBindTexture(GL_TEXTURE_2D, colorBuffers_[wAttachment]);
	}
	inline void bindDepthStencilBuffer()
	{
		
		///glBindTexture(GL_TEXTURE_2D, depthStencilBuffer_);
		glBindTexture(GL_TEXTURE_2D, dephStencilBuffer_);
	}

	//$ Getter
	inline GLuint colorBuffer(size_t wAttachment) const { return colorBuffers_[wAttachment]; }

private:
	GLuint width_ = 0, height_ = 0;		//$ Texture size.
	GLuint id_ = 0;						//$ ID.
	GLuint colorBuffers_[2] = {0,0};	//$ Color buffer textures.
	GLuint dephStencilBuffer_;			//$ Stencil attachment not used yet.
};
