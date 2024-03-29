#ifndef BB_RUNTIME_VRAPI_COMMANDS_H
#define BB_RUNTIME_VRAPI_COMMANDS_H

#include <bb/blitz/module.h>
#include <bb/runtime.vrapi/runtime.vrapi.h>

#ifdef __cplusplus
extern "C" {
#endif

// AUTOGENERATED. DO NOT EDIT.
// RUN `make` TO UPDATE.
bb_int_t BBCALL bbHMDPivot(  );
bb_int_t BBCALL bbHMDHead(  );
bb_int_t BBCALL bbHMDLeftEye(  );
bb_int_t BBCALL bbHMDRightEye(  );
void BBCALL bbUpdateHMD(  );

#ifdef __cplusplus
}
#endif


#endif
