// jsbsim_c_api.h
#ifdef _WIN32
#define JSBSIM_API extern "C" __declspec(dllexport)
#else
#define JSBSIM_API extern "C"
#endif

struct JSBSimState {
    // Posizione nello spazio
    double latitude_deg;
    double longitude_deg;
    double altitude_asl_ft;   // above sea level
    double altitude_agl_ft;   // above ground level

    // Velocità
    double vcas_kts;          // calibrated airspeed
    double vtrue_kts;         // true airspeed
    double vground_kts;       // groundspeed
    double vN_fps, vE_fps, vD_fps; // velocità nel frame locale NED (body->local)

    // Assetto (Eulero)
    double roll_rad;    // phi
    double pitch_rad;   // theta
    double heading_rad; // psi

    // Velocità angolari body-frame (utili quasi sempre insieme all'assetto)
    double p_rad_s, q_rad_s, r_rad_s;
};

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
JSBSIM_API void  JSBSim_GetState(void* fdm, JSBSimState* state);