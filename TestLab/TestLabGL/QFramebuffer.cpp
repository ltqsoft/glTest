#include "QFramebuffer.h"
#include "qglew.h"
#include "test_trycatch.h"

#define QE_FRAMEBUFFER_TEXTURE_INTERNAL_FORMAT GL_RGB


QFramebuffer::QFramebuffer(uint32_t wWidth, uint32_t wHeight) : width_(wWidth), height_(wHeight)
{
_TRY_BLOCK
	glGenFramebuffers(1, &id_);
	glBindFramebuffer(GL_FRAMEBUFFER, id_);

	//$ Create 2 HDR color buffers (1 for normal rendering, other for brightness treshold values)
	glGenTextures(2, colorBuffers_);
	for (size_t i=0; i<2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers_[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB16F, wWidth, wHeight, 0,
			GL_RGB, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//$ attach texture to framebuffer
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D,
			colorBuffers_[i], 0);
	}

	/*/
	//$ create and attach depth-stencil buffer (renderbuffer).
	glGenRenderbuffers(1, &dephStencilBuffer_);
	glBindRenderbuffer(GL_RENDERBUFFER, dephStencilBuffer_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
	glFramebufferRenderbuffer(
			GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
			dephStencilBuffer_ );
	/*/
	
	//$ create and attach depth-stencil buffer (texture).
	glGenTextures(1, &dephStencilBuffer_);
	glBindTexture(GL_TEXTURE_2D, dephStencilBuffer_);
	//glTexImage2D(
		//GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width_, height_, 0,
		//GL_DEPTH24_STENCIL8, GL_FLOAT, nullptr);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, wWidth, wHeight, 0, GL_DEPTH_STENCIL,
		GL_UNSIGNED_INT_24_8, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	//$ attach depth texture to framebuffer
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
		dephStencilBuffer_, 0);

	//$ Tell OpenGL which attachments we'll use (of this framebuffer) for rendering.
	uint32_t attachments[3] =
	{
		GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_DEPTH_STENCIL_ATTACHMENT 
	};
	glDrawBuffers(2, attachments);	//$ Currently no drawing use for depth/stencil attachment.


	//$ Finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw std::runtime_error("Framebuffer is incomplete!");
	
	//$ Switch back to OpenGL's default framebuffer.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

_CATCH_BLOCK(const std::runtime_error& e)
#ifdef _DEBUG
	std::cerr << e.what() << "\n";
#else
	return;
#endif // _DEBUG

_TRY_CATCH_END
}


QFramebuffer::~QFramebuffer()
{
	glDeleteFramebuffers(1, &id_);
	glDeleteTextures(2, colorBuffers_);
	glDeleteRenderbuffers(1, &dephStencilBuffer_);
}
