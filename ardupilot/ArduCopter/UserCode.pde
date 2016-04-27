/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#ifdef USERHOOK_INIT
void userhook_init()
{
    // put your initialisation code here
    // this will be called once at start-up

	g.flight_mode1 = STABILIZE;
	g.flight_mode2 = FOLLOW;
	g.flight_mode3 = LAND;
	g.flight_mode4 = OF_LOITER;
	g.flight_mode5 = OF_LOITER;
	g.flight_mode6 = LAND;

	gps.lock_port(0, 1);
//	hal.uartB->begin(57600, 256, 16);
	hal.uartB->begin(115200, 256, 16);

}
#endif

//	int16_t height_error_T = 0;
//	int16_t previousHeightError_T = 0;
//	int16_t roll_error_T = 0;
//	int16_t previousRollError_T = 0;
//	int16_t pitch_error_T = 0;
//	int16_t previousPitchError_T = 0;
	int16_t yaw_error_T = 0;
	int16_t previousYawError_T = 0;
	uint32_t lastTime_T = 0;
    int32_t dt_T = 0;
float int16_t e;
float e1;
float e2;
float u;
float delta_u;
float y;
float  k1 = throttleP + throttleI + throttleD;
float k2 = -throttleP-2*throttleD;
float k3 = throttleD;
int16_t UMAX = 1000;
int16_t UMIN = -1000;



#ifdef USERHOOK_FASTLOOP
void userhook_FastLoop()
{
    // put your 100Hz code here
    if(tracking_updated){
        //update throttle
        //temporary
        //follow_target_height = 200;

        //update roll with oculus yaw        
        tracking_updated = 0;
        uint32_t currentTime = millis();
        dt_T = currentTime - lastTime_T;

        
        // Test av annan PID Filip Saltvik

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
        
        follow_throttle = (int16_t) (u+baseThrottle);
        
        /*
        roll_error_T = follow_oculus_yaw - wrap_180_cd(ahrs.yaw_sensor);
        if(roll_error_T > 18000){
        	roll_error_T = roll_error_T - 36000;
        }else if(roll_error_T < -18000){
        	roll_error_T = 36000 + roll_error_T;
        }
        rollIntegral += roll_error_T * dt_T;
        rollDerivative = ((float)(roll_error_T - previousRollError_T)) / dt_T;
        follow_roll = (int16_t)constrain_float(0 + -1*rollP * roll_error_T + rollI * rollIntegral + rollD * rollDerivative, -4500, 4500);
        previousRollError_T = roll_error_T;
		*/
/*
        pitch_error_T = follow_distance_to_user;
        pitchIntegral += pitch_error_T * dt_T;
        pitchDerivative = ((float)(pitch_error_T - previousPitchError_T)) / dt_T;
        follow_pitch = (int16_t)constrain_float(0 + -1*pitchP * pitch_error_T + pitchI * pitchIntegral + pitchD * pitchDerivative, -4500, 4500);
        previousPitchError_T = pitch_error_T;
*/
/*        yaw_error_T = follow_centerline_error;
        yawIntegral += yaw_error_T * dt_T;
        yawDerivative = ((float)(yaw_error_T - previousYawError_T)) / dt_T;
        follow_yaw = (int16_t)constrain_float(0 + -1*yawP * yaw_error_T + yawI * yawIntegral + yawD * yawDerivative, -4500, 4500);
        previousYawError_T = yaw_error_T;

*/

/*
        tracking_updated = 0;
        uint32_t currentTime = millis();
        dt_T = currentTime - lastTime_T;
        height_error_T =  follow_target_height - follow_sonar_height;
        throttleIntegral = throttleIntegral + height_error_T * dt_T;
        throttleIntegral = constrain_int32(throttleIntegral, -1000000, 1000000);
        throttleDerivative = ((float)(height_error_T - previousHeightError_T)) / dt_T;
        throttleDerivative = constrain_int32(throttleDerivative, -1000000, 1000000);
        follow_throttle = (int16_t)constrain_float(follow_target_distance + throttleP * height_error_T + throttleI * throttleIntegral + throttleD * throttleDerivative, 0, 1000);
        previousHeightError_T = height_error_T;
*/
        lastTime_T = currentTime;
	}}
#endif

//uint16_t i = 0;
//char values[256];
long receivedCount = 0;
char received[128];

#ifdef USERHOOK_50HZLOOP
void userhook_50Hz()
{
	// put your 50Hz code here
	if(receivedCount == 0){
		while(hal.uartB->available()){
			if(hal.uartB->read() == STX){
				received[receivedCount++] = STX;
				break;
			}
		}
	}

	while(hal.uartB->available() && receivedCount > 0 && received[receivedCount - 1] != ETX){
		received[receivedCount++] = hal.uartB->read();
	}

	if(receivedCount > 0 && received[receivedCount - 1] == ETX){
		char id = received[1];
		switch (id) {
			case TRACK_DATA_ID:
				updateTrackData();
				receivedCount = 0;
				break;
			case SETTINGS_ID:
				updateSettings();
				receivedCount = 0;
				break;
			case THROTTLE_PID_ID:
				updateThrottlePid();
				receivedCount = 0;
				break;
			case YAW_PID_ID:
				hal.console->printf("update yaw pid");
				updateYawPid();
				receivedCount = 0;
				break;
			case ROLL_PID_ID:
				hal.console->printf("update roll pid");
				updateRollPid();
				receivedCount = 0;
				break;
			case PITCH_PID_ID:
				updatePitchPid();
				receivedCount = 0;
				break;

		}
		tracking_updated = 1;
	}



	/*gammalt
	if(i == 0){
    //search for start token
		while(hal.uartB->available()){
			if(hal.uartB->read() == STX){
				values[i] = STX;
				i = 1;
				break;
			} 
		}
	}
	while(i > 0 && hal.uartB->available()){
		values[i] = hal.uartB->read();
		if(values[i] == ETX){
			i = i + 1;
			break;
		}
		i = i + 1;
	}
	
	
	if(values[i - 1] == ETX){
		values[i] = '\0';
		hal.console->printf("values %s\n",  values);
		//values[21] = '\0';
		//gcs_send_text_fmt(PSTR("values: %s"), values);
		//gcs_send_text_fmt(PSTR("value 1: %d\n value 4: %d"), values[1], values[4]);
	    //hal.console->printf("values: %s\n", values);
	    int index = 1;
	    for(int j = 0; j < 5; j++){
	    	readInt(values, &index, j);
	    }

		for(int j = 0; j < 12; j++){
	    	readFloat(values, &index, j);
	    }	    
	    
		i = 0;

		if(follow_oculus_yaw_offset == -1){
			follow_oculus_yaw_offset = follow_int_variables[OCULUS_YAW] - ahrs.yaw_sensor;
		}

		follow_int_variables[CURRENT_HEIGHT] = follow_int_variables[CURRENT_HEIGHT]/(sqrt( sq( tan( radians( abs((float)ahrs.roll_sensor) / 100 ))) + sq( tan(radians(abs((float)ahrs.pitch_sensor) / 100))) + 1));
		//follow_target_height = targetHeight;
		follow_int_variables[OCULUS_YAW] = follow_int_variables[OCULUS_YAW] + follow_oculus_yaw_offset;
		altitude_updated = 1;
		hal.console->printf("current height %d\n",  follow_int_variables[CURRENT_HEIGHT]);
		hal.console->printf("throttle P %f\n",  follow_float_variables[THROTTLE_P]);
		//float tmpHeight1 = 100/(sqrt( sq( tan( radians( abs((float)4500) / 100 ))) + sq( tan(radians(abs((float)0) / 100))) + 1));
		//float tmpHeight2 = 100/(sqrt( sq( tan( radians( abs((float)2500) / 100 ))) + sq( tan(radians(abs((float)2500) / 100))) + 1));
		//gcs_send_text_fmt(PSTR("compass: %d\ncurrent height: %d\nforward: %d\nrotate: %d\ntarget height: %d"), compass, currentHeight, forward, rotate, targetHeight);
		//hal.console->printf("tmp height 1: %f\n", tmpHeight1);
		//hal.console->printf("tmp height 2: %f\n", tmpHeight2);
		//hal.console->printf("compass: %d\ncurrent height: %d\ncurrent height2: %f\nforward: %d\nrotate: %d\ntarget height: %d\n", compass, currentHeight, currentHeight2, forward, rotate, targetHeight);		
	}
	*/
   
}
#endif



#ifdef USERHOOK_MEDIUMLOOP
void userhook_MediumLoop()
{
    // put your 10Hz code here


}
#endif

#ifdef USERHOOK_SLOWLOOP
void userhook_SlowLoop()
{
    
}
#endif


#ifdef USERHOOK_SUPERSLOWLOOP
void userhook_SuperSlowLoop()
{
/*
	hal.console->printf("follow_thottle %d\n", follow_throttle);
	hal.console->printf("follow_yaw %d\n", follow_yaw);
	hal.console->printf("throttle %d\n",  g.rc_3.control_in);
	hal.console->printf("yaw %d\n",  g.rc_4.control_in);
	hal.console->printf("integral %d\n",  throttleIntegral);

	hal.console->printf("target height %d\n",  follow_target_height);
	hal.console->printf("current height %d\n",  follow_sonar_height);
	hal.console->printf("oculus yaw %d\n",  follow_oculus_yaw);
	hal.console->printf("distance to user %d\n",  follow_distance_to_user);
	hal.console->printf("target distance %d\n",  follow_target_distance);
	hal.console->printf("filips variabel %d\n",  follow_roll_error);

	hal.console->printf("throttle p %f\n",  throttleP);
	hal.console->printf("throttle i %f\n",  throttleI);
	hal.console->printf("throttle d %f\n",  throttleD);

	hal.console->printf("yaw offset %d\n",  follow_oculus_yaw_offset);
	hal.console->printf("oculus yaw %d\n",  follow_oculus_yaw);
	hal.console->printf("pilot yaw 1 %d\n",  ahrs.yaw_sensor);
	hal.console->printf("pilot yaw 2 %d\n",   wrap_180_cd(ahrs.yaw_sensor));
*/

	//throttle prints
	/*
	hal.console->printf("thottle %d\n", follow_throttle);
//	hal.console->printf("target height %d\n",  follow_target_height);
//	hal.console->printf("target distance %d\n",  follow_target_distance);
	hal.console->printf("current height %d\n",  follow_sonar_height);
	hal.console->printf("derivative %ld\n",  throttleDerivative);
	hal.console->printf("integral %ld\n",  throttleIntegral);
	hal.console->printf("throttle p %f\n",  throttleP);
	hal.console->printf("throttle i %f\n",  throttleI);
	hal.console->printf("throttle d %f\n",  throttleD);
	hal.console->printf("jacob throttle %d\n",  g.rc_3.control_in);
*/
	//yaw prints
	
/*	hal.console->printf("roll %d\n", follow_roll);
	hal.console->printf("yaw offset %d\n",  follow_oculus_yaw_offset);
	hal.console->printf("oculus yaw %d\n",  follow_oculus_yaw);
	hal.console->printf("pilot yaw %d\n",  wrap_180_cd(ahrs.yaw_sensor));
	hal.console->printf("roll error %d\n",  roll_error_T);
	hal.console->printf("jacob pitch %d\n",  g.rc_1.control_in);
	hal.console->printf("current height %d\n",  follow_sonar_height);
*/

/*
	hal.console->printf("pitch %d\n", follow_pitch);
	hal.console->printf("distance to user %d\n",  follow_distance_to_user);
	hal.console->printf("centerline error %d\n", follow_centerline_error);
	hal.console->printf("pitch error %d\n",  pitch_error_T);
	hal.console->printf("jacob pitch %d\n",  g.rc_2.control_in);
	hal.console->printf("current height %d\n",  follow_sonar_height);
//	hal.console->printf("yaw p %f\n",  yawP);
//	hal.console->printf("yaw i %f\n",  yawI);
//	hal.console->printf("yaw d %f\n",  yawD);	
//	hal.console->printf("throttle p %f\n",  throttleP);
//	hal.console->printf("throttle i %f\n",  throttleI);
//	hal.console->printf("throttle d %f\n",  throttleD);	
	hal.console->printf("pitch p %f\n",  pitchP);
*/	
//	received[receivedCount] = '\0';
//	hal.console->printf("received %s\n",  received);

	hal.console->printf("current height %d\n",  follow_sonar_height);
	hal.console->printf("yaw %d\n", follow_yaw);
	hal.console->printf("centerline error %d\n", follow_centerline_error);
	hal.console->printf("roll %d\n", follow_roll);
	hal.console->printf("oculus yaw %d\n",  follow_oculus_yaw);
	hal.console->printf("pilot yaw %d\n",  wrap_180_cd(ahrs.yaw_sensor));
	hal.console->printf("pitch %d\n", follow_pitch);
	hal.console->printf("distance %d\n",  follow_distance_to_user);
	hal.console->printf("jacob pitch %d\n",  g.rc_2.control_in);
	hal.console->printf("yaw p %f\n",  yawP);


	//hal.console->printf("ahrs.pitch_sensor %d\n", ahrs.pitch_sensor);
	//hal.console->printf("ahrs.yaw_sensor %d\n", ahrs.yaw_sensor);

	// if(hal.uartB->available()){
	// if(hal.uartB->available()){
	//     hal.console->printf("available 1: %d\n", hal.uartB->available());		
	// 	int16_t tmp = hal.uartB->read();
	//     hal.console->printf("available 2: %d\n", hal.uartB->available());		
	//     gcs_send_text_fmt(PSTR("read: %c, %d"), tmp, tmp);
	//     hal.console->printf("read: %c, %d\n", tmp, tmp);		
	// }
	//|111222244....
    // put your 1Hz code here
  //   gcs_send_text_fmt(PSTR("super slow print"));
  //   if(hal.uartB->available() <= 0){
	 //    gcs_send_text_fmt(PSTR("No data available"));
	 //    return;
  //   }
  //  	uint8_t i = 0;
  //   uint8_t tmp;
  //   uint8_t values[21];
  //   while(i < 21){
  //   	tmp = hal.uartB->read();
  //   	if(tmp == 255){
  //   		hal.scheduler->delay(10);
  //   	}else{
	 //    	gcs_send_text_fmt(PSTR("value %d: %c"), i, tmp);
  //   		values[i] = tmp;
	 //    	i += 1;
  //   	}
  //   	hal.scheduler->delay(40);
  //   }
  //   //gcs_send_text_fmt(PSTR("values: %s"), values);

  //   if(values[0] == STX && values[20] == ETX){
  //   	int16_t compass = values[1] - '0';
  //   	compass *= 100;
  //   	compass += (values[2] - '0') * 10;
  //   	compass += values[3] - '0';
  //   	int16_t currentHeight = values[4] - '0';
  //   	currentHeight *= 1000;
  //   	currentHeight += (values[5] - '0') * 100;
  //   	currentHeight += (values[6] - '0') * 10;
  //   	currentHeight += values[7] - '0';
  //   	int16_t forward = values[8] - '0';
  //   	forward *= 1000;
  //   	forward += (values[9] - '0') * 100;
  //   	forward += (values[10] - '0') * 10;
  //   	forward += values[11] - '0';
  //   	int16_t rotate = values[12] - '0';
  //   	rotate *= 1000;
  //   	rotate += (values[13] - '0') * 100;
  //   	rotate += (values[14] - '0') * 10;
  //   	rotate += values[15] - '0';
  //   	int16_t targetHeight = values[16] - '0';
  //   	targetHeight *= 1000;
  //   	targetHeight += (values[17] - '0') * 100;
  //   	targetHeight += (values[18] - '0') * 10;
  //   	targetHeight += values[19] - '0';
		// gcs_send_text_fmt(PSTR("compass: %d"), compass);
		// gcs_send_text_fmt(PSTR("current height: %d"), currentHeight);
		// gcs_send_text_fmt(PSTR("forward: %d"), forward);
		// gcs_send_text_fmt(PSTR("rotate: %d"), rotate);
		// gcs_send_text_fmt(PSTR("target height: %d"), targetHeight);

  //   }else{
  //   	gcs_send_text_fmt(PSTR("wrong start or end value"));
  //   }

    //gcs_send_text_fmt(PSTR("debug: read = %d"), (uint8_t)hal.uartB->read());
    //gcs_send_text_fmt(PSTR("debug: current mode = %d"), (int)control_mode);

}
#endif


void updateTrackData(){
	long index = 3;
	follow_oculus_yaw = readInt(&index);
	if(follow_oculus_yaw_offset == -1 && follow_oculus_yaw != 0){
		follow_oculus_yaw_offset = follow_oculus_yaw - wrap_180_cd(ahrs.yaw_sensor);
	}
	follow_oculus_yaw = follow_oculus_yaw - follow_oculus_yaw_offset;
	follow_centerline_error = readInt(&index);
	follow_sonar_height = readInt(&index);
	follow_sonar_height = follow_sonar_height/(sqrt( sq( tan( radians( abs((float)ahrs.roll_sensor) / 100 ))) + sq( tan(radians(abs((float)ahrs.pitch_sensor) / 100))) + 1));
	follow_distance_to_user = readInt(&index);
}

void updateSettings(){
	long index = 3;
	follow_target_height = readInt(&index);
	follow_target_distance = readInt(&index);
}

void updateThrottlePid(){
	long index = 3;
	throttleP = readFloat(&index);
	throttleI = readFloat(&index);
	throttleD = readFloat(&index);
}

void updateYawPid(){
	long index = 3;
	yawP = readFloat(&index);
	yawI = readFloat(&index);
	yawD = readFloat(&index);
}
void updateRollPid(){
	long index = 3;
	rollP = readFloat(&index);
	rollI = readFloat(&index);
	rollD = readFloat(&index);
}
void updatePitchPid(){
	long index = 3;
	pitchP = readFloat(&index);
	pitchI = readFloat(&index);
	pitchD = readFloat(&index);
}

long readInt(long *index){
	char str[10];
	int i = 0;
	while(received[*index] != DIVIDER){
		str[i++] = received[*index];
		*index = *index + 1;
	}
	str[i] = '\0';
	*index = *index + 1;
	return atoi(str);
}

float readFloat(long *index){
	char str[15];
	int i = 0;
	while(received[*index] != DIVIDER){
		str[i++] = received[*index];
		*index = *index + 1;
	}
	str[i] = '\0';
	*index = *index + 1;
	return atof(str);
}

