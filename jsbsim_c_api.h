// jsbsim_c_api.h
#ifdef _WIN32
#define JSBSIM_API extern "C" __declspec(dllexport)
#else
#define JSBSIM_API extern "C"
#endif

// jsbsim_c_api.h
JSBSIM_API void* JSBSim_Create(const char* rootDir);
JSBSIM_API void  JSBSim_Destroy(void* fdm);
JSBSIM_API int   JSBSim_LoadModel(void* fdm, const char* modelName);
JSBSIM_API void  JSBSim_GetLastError(void* fdm, char* buffer, int bufferSize);
JSBSIM_API void  JSBSim_RunIC(void* fdm);
JSBSIM_API int   JSBSim_Run(void* fdm);
JSBSIM_API void  JSBSim_SetPropertyDouble(void* fdm, const char* property, double value);
JSBSIM_API double JSBSim_GetPropertyDouble(void* fdm, const char* property);
JSBSIM_API double JSBSim_GetSimTime(void* fdm);