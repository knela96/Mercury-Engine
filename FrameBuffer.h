#pragma once
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();
	bool Start();
	void PreUpdate();
	void PostUpdate();
	uint GetTexture();

private:
	unsigned int fbo;
	unsigned int texture;
};
#endif
