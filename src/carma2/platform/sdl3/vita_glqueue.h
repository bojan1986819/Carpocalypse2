#ifndef VITA_GLQUEUE_H
#define VITA_GLQUEUE_H

/* Returns the queue's wrapper for a GL entry point (remembering pOriginal), or pOriginal. */
void* C2VitaGLQueueWrap(const char* pName, void* pOriginal);

/* Starts / stops the render thread. While it is stopped every wrapper calls straight through. */
int C2VitaGLQueueStart(void);
void C2VitaGLQueueStop(void);
int C2VitaGLQueueActive(void);

/* Waits until the render thread executed everything queued so far (before direct vitaGL use). */
void C2VitaGLQueueSync(void);

/* Queues pFunction(pArgument) to run on the render thread in order with the GL calls. It must call
 * vitaGL directly, never the wrapped entry points. */
void C2VitaGLQueueCall(void (*pFunction)(void*), void* pArgument);
void C2VitaGLQueueDirectFrame(void);

/* Queues the frame swap; returns once the previous frame's swap has run (one frame in flight). */
void C2VitaGLQueueFrame(void (*pSwap)(void*), void* pArgument);

/* Logs and resets the queue statistics (main thread). */
void C2VitaGLQueueFrameLog(unsigned int pFrame);

#endif
