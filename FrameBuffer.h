#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();
	bool Start(int width, int heigth);
	void PreUpdate();
	void PostUpdate();
	const uint GetTexture();

	void CleanUp();

private:
	unsigned int fbo;
	unsigned int texture;
	unsigned int texture1;
};
#endif
