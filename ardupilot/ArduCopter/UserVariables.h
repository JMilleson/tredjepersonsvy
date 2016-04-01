/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

// user defined variables

// example variables used in Wii camera testing - replace with your own
// variables
#ifdef USERHOOK_VARIABLES

int16_t follow_throttle = 0;
int16_t follow_target_height = 200;
int16_t follow_sonar_height = 200;
int16_t follow_oculus_yaw = 0;
int16_t follow_oculus_yaw_offset = -1;
int16_t follow_yaw = 0;
int altitude_updated = 0;

float kp = 0.25f, ki = 0.1f, kd = 0;
int32_t integral = 0, derivative = 0;

#if WII_CAMERA == 1
WiiCamera           ircam;
int                 WiiRange=0;
int                 WiiRotation=0;
int                 WiiDisplacementX=0;
int                 WiiDisplacementY=0;
#endif  // WII_CAMERA

#endif  // USERHOOK_VARIABLES


