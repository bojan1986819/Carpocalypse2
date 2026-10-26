#ifndef C2_VITA_GL_H
#define C2_VITA_GL_H
void* C2VitaGLGetProcAddress(const char* name);
void C2VitaGLResetBindings(void);
void C2VitaGLInvalidateState(void); /* after direct VitaGL state calls */
int C2VitaGLValidate(void);
void C2VitaGLTraceTextures(int enabled);
void C2VitaGLFrameStats(unsigned int frame, int world, unsigned long long interval, unsigned long long swap_ms);
unsigned long long C2VitaGLTicksNS(void);
void C2VitaGLRecordOverlayCheck(int empty, unsigned long long elapsed_ns);
void C2VitaGLRecordOverlayRows(int rows);
void C2VitaGLRecordOverlayScope(void);
void C2VitaGLLogOverlayBenchmark(unsigned pixels, unsigned long long scalar_ns, unsigned long long vector_ns, int valid);
#endif
