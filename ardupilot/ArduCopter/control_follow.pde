/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

/*
 * control_follow.cpp - init and run calls for follow flight mode
 */

int16_t previousHeightError = 0;
int16_t previousYawError = 0;
int16_t previousPitchError = 0;
int16_t previousRollError = 0;
uint32_t lastTime = 0;

// follow_init - initialise follow mode
static bool follow_init(bool ignore_checks)
{
    // do any required initialisation of the flight mode here
    // this code will be called whenever the operator switches into this mode

    // return true initialisation is successful, false if it fails
    // if false is returned here the vehicle will remain in the previous flight mode


    //g.rc_3.control_in
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

        tracking_updated = 0;
        uint32_t currentTime = millis();
        int32_t dt = currentTime - lastTime;
        height_error =  follow_target_height - follow_sonar_height;
        throttleIntegral += height_error * dt;
        throttleIntegral = constrain_int32(throttleIntegral, -1500000, 1500000);
        throttleDerivative = ((float)(height_error - previousHeightError)) / dt;
        throttleDerivative = constrain_int32(throttleDerivative, -1000000, 1000000);
        follow_throttle = (int16_t)constrain_float(follow_target_distance + throttleP * height_error + throttleI * throttleIntegral + throttleD * throttleDerivative, 0, 1000);
        previousHeightError = height_error;


        //update yaw with oculus
        /*
        yaw_error = follow_oculus_yaw - wrap_180_cd(ahrs.yaw_sensor);
        yawIntegral += yaw_error * dt;
        yawDerivative = ((float)(yaw_error - previousYawError)) / dt;
        follow_yaw = (int16_t)constrain_float(0 + yawP * yaw_error + yawI * yawIntegral + yawD * yawDerivative, -4500, 4500);
        previousYawError = yaw_error;
        */

        //update yaw with ultrasonic tracking
        /*
        yaw_error = follow_centerline_error;
        yawIntegral += yaw_error * dt;
        yawDerivative = ((float)(yaw_error - previousYawError)) / dt;
        follow_yaw = (int16_t)constrain_float(0 + yawP * yaw_error + yawI * yawIntegral + yawD * yawDerivative, -4500, 4500);
        previousYawError = yaw_error;
        */

        //update roll with oculus yaw
        /*
        roll_error = follow_oculus_yaw - wrap_180_cd(ahrs.yaw_sensor);
        rollIntegral += roll_error * dt;
        rollDerivative = ((float)(roll_error - previousRollError)) / dt;
        follow_roll = (int16_t)constrain_float(0 + rollP * roll_error + rollI * rollIntegral + rollD * rollDerivative, -4500, 4500);
        previousRollError = roll_error;
        */

        //update pitch with ultrasonic distance
        /*
        //pitch_error = follow_target_distance - follow_distance_to_user;
        pitch_error = follow_distance_to_user;
        pitchIntegral += pitch_error * dt;
        pitchDerivative = ((float)(pitch_error - previousPitchError)) / dt;
        follow_pitch = (int16_t)constrain_float(0 + pitchP * pitch_error + pitchI * pitchIntegral + pitchD * pitchDerivative, -4500, 4500);
        previousPitchError = pitch_error;
        */

        lastTime = currentTime;
    }


    target_yaw_rate = get_pilot_desired_yaw_rate(g.rc_4.control_in);
    get_pilot_desired_lean_angles(g.rc_1.control_in, g.rc_2.control_in, target_roll, target_pitch);
//enable full tracking    
//    target_yaw_rate = get_pilot_desired_yaw_rate(follow_yaw);
//    get_pilot_desired_lean_angles(follow_roll, follow_pitch, target_roll, target_pitch);
    attitude_control.angle_ef_roll_pitch_rate_ef_yaw_smooth(target_roll, target_pitch, target_yaw_rate, get_smoothing_gain());
    attitude_control.set_throttle_out(follow_throttle, true);

}
