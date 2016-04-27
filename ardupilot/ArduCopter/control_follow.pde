/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

/*
 * control_follow.cpp - init and run calls for follow flight mode
 */

int16_t previousHeightError = 0;
int16_t previousYawError = 0;
int16_t previousPitchError = 0;
int16_t previousRollError = 0;
uint32_t lastTime = 0;
int16_t baseThrottle = 600;

int16_t e;
int16_t e1;
int16_t e2;
float u;
float delta_u;
int16_t y;
float  k1 = throttleP + throttleI + throttleD;
float k2 = -throttleP-2*throttleD;
float k3 = throttleD;
int16_t UMAX = 1000;
int16_t UMIN = -1000;


// follow_init - initialise follow mode
static bool follow_init(bool ignore_checks)
{
    // do any required initialisation of the flight mode here
    // this code will be called whenever the operator switches into this mode

    // return true initialisation is successful, false if it fails
    // if false is returned here the vehicle will remain in the previous flight mode


    baseThrottle = g.rc_3.control_in;
    //values between 0 and 1000

    previousHeightError = 0;
    previousYawError = 0;
    previousPitchError = 0;
    previousRollError = 0;
    throttleIntegral = 0;
    throttleDerivative = 0;
    yawIntegral = 0;
    yawDerivative = 0;
    rollIntegral = 0;
    rollDerivative = 0;
    pitchIntegral = 0;
    pitchDerivative = 0;

    lastTime = millis();

    hal.console->println("debug: init follow_run");
    return true;
}

// follow_run - runs the main follow controller
// will be called at 100hz or more
static void follow_run()
{
    int16_t height_error, yaw_error, pitch_error, roll_error;
    int16_t target_roll, target_pitch;
    float target_yaw_rate;

    // if not armed or throttle at zero, set throttle to zero and exit immediately
    if(!motors.armed()) {
        attitude_control.relax_bf_rate_controller();
        attitude_control.set_yaw_target_to_current_heading();
        attitude_control.set_throttle_out(0, false);
        return;
    }


    // get pilot's desired yaw rate
    //follow_target_climb_rate = get_pilot_desired_climb_rate(g.rc_3.control_in);

    //hal.console->println("debug: follow_run");
    //hal.scheduler->delay(1000);

    // convert pilot input into desired vehicle angles or rotation rates
    //   g.rc_1.control_in : pilots roll input in the range -4500 ~ 4500
    //   g.rc_2.control_in : pilot pitch input in the range -4500 ~ 4500
    //   g.rc_3.control_in : pilot's throttle input in the range 0 ~ 1000
    //   g.rc_4.control_in : pilot's yaw input in the range -4500 ~ 4500

    // call one of attitude controller's attitude control functions like
    //   attitude_control.angle_ef_roll_pitch_rate_yaw(roll angle, pitch angle, yaw rate);

    // call position controller's z-axis controller or simply pass through throttle
    //   attitude_control.set_throttle_out(desired throttle, true);

    
    if(tracking_updated){
        //update throttle
        //temporary
        //follow_target_height = 200;

        // p = 0.7
        // i = 0.00004
        // d = 55

        tracking_updated = 0;
        uint32_t currentTime = millis();
        int32_t dt = currentTime - lastTime;
        /*
        height_error =  follow_target_height - follow_sonar_height;
        throttleIntegral += height_error * dt;
        //throttleIntegral = constrain_int32(throttleIntegral, -1500000, 1500000);
        throttleDerivative = ((float)(height_error - previousHeightError)) / dt;
        follow_throttle = (int16_t)constrain_float(baseThrottle + throttleP * height_error + throttleI * throttleIntegral + throttleD * throttleDerivative, 0, 1000);
        previousHeightError = height_error;
        */

        k1 = throttleP + throttleI + throttleD;
        k2 = -throttleP-2*throttleD;
        k3 = throttleD;
        // Values from two previous values.
        e2 = e1;
        e1 = e; 
        // Read sensors value
        y = follow_sonar_height;
        // Calculate current error
        e = follow_target_height - follow_sonar_height;
        //PID algorithm
        delta_u = k1*e + k2*e1 + k3*e2;
        // Update the u value.
        u = u + delta_u;
        // Limit throttle difference to max value
        if (u > UMAX) u = UMAX;
        // Limit throttle difference to min value
        if (u < UMIN) u = UMIN;
        // Send throttle with baseThrotthle value, i.e. basethrottle plus difference, 
        
        follow_throttle = (int16_t)constrain_float(u+baseThrottle, 0, 1000);
        // 100 560
        //146 513

        //update yaw with oculus
        /*
        yaw_error = follow_oculus_yaw - wrap_180_cd(ahrs.yaw_sensor);
        yawIntegral += yaw_error * dt;
        yawDerivative = ((float)(yaw_error - previousYawError)) / dt;
        follow_yaw = (int16_t)constrain_float(0 + yawP * yaw_error + yawI * yawIntegral + yawD * yawDerivative, -4500, 4500);
        previousYawError = yaw_error;
        */

        //update yaw with ultrasonic tracking
        
        yaw_error = follow_centerline_error;
        yawIntegral += yaw_error * dt;
        yawDerivative = ((float)(yaw_error - previousYawError)) / dt;
        follow_yaw = (int16_t)constrain_float(0 + -1*yawP * yaw_error + yawI * yawIntegral + yawD * yawDerivative, -4500, 4500);
        previousYawError = yaw_error;
        

        //update roll with oculus yaw
        
        roll_error = follow_oculus_yaw - wrap_180_cd(ahrs.yaw_sensor);
        if(roll_error > 18000){
            roll_error = roll_error - 36000;
        }else if(roll_error < -18000){
            roll_error = 36000 + roll_error;
        }
        rollIntegral += roll_error * dt;
        rollDerivative = ((float)(roll_error - previousRollError)) / dt;
        follow_roll = (int16_t)constrain_float(0 + -1*rollP * roll_error + rollI * rollIntegral + rollD * rollDerivative, -4500, 4500);
        previousRollError = roll_error;
        

        //update pitch with ultrasonic distance
        
        //pitch_error = follow_target_distance - follow_distance_to_user;
        pitch_error = follow_distance_to_user;
        pitchIntegral += pitch_error * dt;
        pitchDerivative = ((float)(pitch_error - previousPitchError)) / dt;
        follow_pitch = (int16_t)constrain_float(0 + -1*pitchP * pitch_error + pitchI * pitchIntegral + pitchD * pitchDerivative, -4500, 4500);
        previousPitchError = pitch_error;
        

        lastTime = currentTime;
    }


    target_yaw_rate = get_pilot_desired_yaw_rate(g.rc_4.control_in);
    get_pilot_desired_lean_angles(g.rc_1.control_in, g.rc_2.control_in, target_roll, target_pitch);
//enable full tracking    
//    target_yaw_rate = get_pilot_desired_yaw_rate(follow_yaw);
//    get_pilot_desired_lean_angles(g.rc_1.control_in, follow_pitch, target_roll, target_pitch);
//    get_pilot_desired_lean_angles(follow_roll, g.rc_2.control_in, target_roll, target_pitch);
//    get_pilot_desired_lean_angles(follow_roll, follow_pitch, target_roll, target_pitch);
    attitude_control.angle_ef_roll_pitch_rate_ef_yaw_smooth(target_roll, target_pitch, target_yaw_rate, get_smoothing_gain());
    attitude_control.set_throttle_out(follow_throttle, true);
    //attitude_control.set_throttle_out(get_pilot_desired_throttle(g.rc_3.control_in), true);


}
