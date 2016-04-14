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
//int16_t follow_roll_error = -1;
int altitude_updated = 0;

float throttleP = 0.25f, throttleI = 0.0001f, throttleD = 0;
float yawP = 0.25f, yawI = 0.0001f, yawD = 0;
//float rollP = 0.25f, rollI = 0.0001f, rollD = 0;
float pitchP = 0.25f, pitchI = 0.0001f, pitchD = 0;

int32_t throttleIntegral = 0, throttleDerivative = 0;
int32_t yawIntegral = 0, yawDerivative = 0;
int32_t rollIntegral = 0, rollDerivative = 0;
int32_t pitchIntegral = 0, pitchDerivative = 0;

int16_t follow_int_variables[5];
float follow_float_variables[12];

#define OCULUS_YAW 0
#define CURRENT_HEIGHT 1
#define TARGET_HEIGHT 2
#define DISTANCE_TO_USER 3
#define ROLL_VALUE 4

#define THROTTLE_P 0
#define THROTTLE_I 1
#define THROTTLE_D 2
#define YAW_P 3
#define YAW_I 4
#define YAW_D 5
#define ROLL_P 6
#define ROLL_I 7
#define ROLL_D 8
#define PITCH_P 9
#define PITCH_I 10
#define PITCH_D 11


#if WII_CAMERA == 1
WiiCamera           ircam;
int                 WiiRange=0;
int                 WiiRotation=0;
int                 WiiDisplacementX=0;
int                 WiiDisplacementY=0;
#endif  // WII_CAMERA

#endif  // USERHOOK_VARIABLES


