/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

// user defined variables

// example variables used in Wii camera testing - replace with your own
// variables
#ifdef USERHOOK_VARIABLES

int16_t follow_throttle = 0;
int16_t follow_yaw = 0;
int16_t follow_roll = 0;
int16_t follow_pitch = 0;
int16_t follow_target_height = 200;
int16_t follow_sonar_height = 200;
int16_t follow_oculus_yaw = 0;
int16_t follow_oculus_yaw_offset = -1;
int16_t follow_distance_to_user = -1;
int16_t follow_target_distance = 300;
int16_t follow_centerline_error = -1;
int tracking_updated = 0;

float throttleP = 0.25f, throttleI = 0.0002f, throttleD = 0;
float yawP = 0, yawI = 0, yawD = 0;
float rollP = 0, rollI = 0, rollD = 0;
float pitchP = 0, pitchI = 0, pitchD = 0;

int32_t throttleIntegral = 0, throttleDerivative = 0;
int32_t yawIntegral = 0, yawDerivative = 0;
int32_t rollIntegral = 0, rollDerivative = 0;
int32_t pitchIntegral = 0, pitchDerivative = 0;

#define STX '\002'
#define ETX '\003'
#define DIVIDER '|'

#define TRACK_DATA_ID '1'
#define SETTINGS_ID '6'
#define THROTTLE_PID_ID '2'
#define YAW_PID_ID '3'
#define ROLL_PID_ID '4'
#define PITCH_PID_ID '5'


#if WII_CAMERA == 1
WiiCamera           ircam;
int                 WiiRange=0;
int                 WiiRotation=0;
int                 WiiDisplacementX=0;
int                 WiiDisplacementY=0;
#endif  // WII_CAMERA

#endif  // USERHOOK_VARIABLES


