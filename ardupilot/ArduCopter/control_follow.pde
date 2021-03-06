/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

/*
 * control_follow.cpp - init and run calls for follow flight mode
 */
int last_altitude = 0;

// follow_init - initialise follow mode
static bool follow_init(bool ignore_checks)
{
    // do any required initialisation of the flight mode here
    // this code will be called whenever the operator switches into this mode

    // return true initialisation is successful, false if it fails
    // if false is returned here the vehicle will remain in the previous flight mode


    //g.rc_3.control_in
    //values between 0 and 1000

    follow_throttle = g.rc_3.control_in;

    hal.console->println("debug: init follow_run");
    return true;
}

// follow_run - runs the main follow controller
// will be called at 100hz or more
static void follow_run()
{
    float distance_error;
    float velocity_correction;
    int16_t target_roll, target_pitch;
    float target_yaw_rate;

    // if not armed or throttle at zero, set throttle to zero and exit immediately
    if(!motors.armed() || g.rc_3.control_in <= 0) {
        attitude_control.relax_bf_rate_controller();
        attitude_control.set_yaw_target_to_current_heading();
        attitude_control.set_throttle_out(0, false);
        return;
    }

     get_pilot_desired_lean_angles(g.rc_1.control_in, g.rc_2.control_in, target_roll, target_pitch);

    // get pilot's desired yaw rate
    target_yaw_rate = get_pilot_desired_yaw_rate(g.rc_4.control_in);
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
    if(last_altitude != follow_sonar_height){
        distance_error =  follow_target_height - follow_sonar_height;
        follow_throttle = (int16_t) (follow_throttle + distance_error/3);
        follow_throttle = constrain_int16(follow_throttle,0,1000);
        last_altitude = follow_sonar_height;
    }

    attitude_control.angle_ef_roll_pitch_rate_ef_yaw_smooth(target_roll, target_pitch, target_yaw_rate, get_smoothing_gain());
    attitude_control.set_throttle_out(follow_throttle, true);

}
