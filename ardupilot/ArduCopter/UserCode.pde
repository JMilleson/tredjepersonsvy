/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
//#define STX '|'
//#define ETX '|'
#define STX '\002'
#define ETX '\003'

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
	hal.uartB->begin(57600, 256, 16);

}
#endif

#ifdef USERHOOK_FASTLOOP
void userhook_FastLoop()
{
    // put your 100Hz code here
}
#endif

uint16_t i = 0;
char values[256];

#ifdef USERHOOK_50HZLOOP
void userhook_50Hz()
{
    // put your 50Hz code here
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
	
	
	if(values[i] == ETX){
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
		//float tmpHeight1 = 100/(sqrt( sq( tan( radians( abs((float)4500) / 100 ))) + sq( tan(radians(abs((float)0) / 100))) + 1));
		//float tmpHeight2 = 100/(sqrt( sq( tan( radians( abs((float)2500) / 100 ))) + sq( tan(radians(abs((float)2500) / 100))) + 1));
		//gcs_send_text_fmt(PSTR("compass: %d\ncurrent height: %d\nforward: %d\nrotate: %d\ntarget height: %d"), compass, currentHeight, forward, rotate, targetHeight);
		//hal.console->printf("tmp height 1: %f\n", tmpHeight1);
		//hal.console->printf("tmp height 2: %f\n", tmpHeight2);
		//hal.console->printf("compass: %d\ncurrent height: %d\ncurrent height2: %f\nforward: %d\nrotate: %d\ntarget height: %d\n", compass, currentHeight, currentHeight2, forward, rotate, targetHeight);		
	}
   
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
    // put your 3.3Hz code here
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
	while(i > 0 && i < 20 && hal.uartB->available()){
		values[i] = hal.uartB->read();
		i = i + 1; 
	}
	if(i == 20){
		while(hal.uartB->available()){
			if(hal.uartB->read() == ETX){
				values[i] = ETX;
				i = 21;
				break;
			}     
		}
	}
	
	if(i == 21){
		values[21] = '\0';
		//gcs_send_text_fmt(PSTR("values: %s"), values);
		//gcs_send_text_fmt(PSTR("value 1: %d\n value 4: %d"), values[1], values[4]);
	    hal.console->printf("values: %s\n", values);		
		if(values[0] == STX && values[20] == ETX){
			int16_t compass = values[1] - '0';
			compass *= 10;
			compass += values[2] - '0';
			compass *= 10;
			compass += values[3] - '0';
			compass -= 200;
			int16_t currentHeight = values[4] - '0';
			currentHeight *= 10;
			currentHeight += values[5] - '0';
			currentHeight *= 10;    
			currentHeight += values[6] - '0';
			currentHeight *= 10;    
			currentHeight += values[7] - '0';
			int16_t forward = values[8] - '0';
			forward *= 10;
			forward += values[9] - '0';
			forward *= 10;
			forward += values[10] - '0';
			forward *= 10;
			forward += values[11] - '0';
			int16_t rotate = values[12] - '0';
			rotate *= 10;
			rotate += values[13] - '0';
			rotate *= 10;
			rotate += values[14] - '0';
			rotate *= 10;
			rotate += values[15] - '0';
			int16_t targetHeight = values[16] - '0';
			targetHeight *= 10;
			targetHeight += values[17] - '0';
			targetHeight *= 10;
			targetHeight += values[18] - '0';
			targetHeight *= 10;
			targetHeight += values[19] - '0';
			i = 0;

			if(follow_oculus_yaw_offset == -1){
				follow_oculus_yaw_offset = compass - ahrs.yaw_sensor;
			}

			float currentHeight2 = currentHeight/(sqrt( sq( tan( radians( abs((float)ahrs.roll_sensor) / 100 ))) + sq( tan(radians(abs((float)ahrs.pitch_sensor) / 100))) + 1));
			follow_sonar_height = currentHeight2;
			follow_target_height = targetHeight;
			follow_oculus_yaw = compass + follow_oculus_yaw_offset;
			//float tmpHeight1 = 100/(sqrt( sq( tan( radians( abs((float)4500) / 100 ))) + sq( tan(radians(abs((float)0) / 100))) + 1));
			//float tmpHeight2 = 100/(sqrt( sq( tan( radians( abs((float)2500) / 100 ))) + sq( tan(radians(abs((float)2500) / 100))) + 1));
			//gcs_send_text_fmt(PSTR("compass: %d\ncurrent height: %d\nforward: %d\nrotate: %d\ntarget height: %d"), compass, currentHeight, forward, rotate, targetHeight);
			//hal.console->printf("tmp height 1: %f\n", tmpHeight1);
			//hal.console->printf("tmp height 2: %f\n", tmpHeight2);
			hal.console->printf("compass: %d\ncurrent height: %d\ncurrent height2: %f\nforward: %d\nrotate: %d\ntarget height: %d\n", compass, currentHeight, currentHeight2, forward, rotate, targetHeight);		

		}else{
			hal.console->printf("wrong start or end value");
			//gcs_send_text_fmt(PSTR("wrong start or end value"));
		}
	}
    
}
#endif


#ifdef USERHOOK_SUPERSLOWLOOP
void userhook_SuperSlowLoop()
{

	hal.console->printf("follow_thottle %d\n", follow_throttle);
	hal.console->printf("follow_sonar_height %d\n", follow_sonar_height);
	hal.console->printf("follow_yaw %d\n", follow_yaw);
	hal.console->printf("throttle %d\n",  g.rc_3.control_in);
	hal.console->printf("yaw %d\n",  g.rc_4.control_in);
	hal.console->printf("integral %d\n",  integral);



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

void readInt(char inputs[], int16_t *start, int index){
	char str[8];

	int i = 0;
	while(inputs[*start] != '|' && inputs[*start] != ETX){
		str[i] = inputs[*start];
		*start = *start + 1;
		i = i + 1;
	}
	str[i] = '\0';
	*start = *start + 1;

	follow_int_variables[index] = atoi(str);

}

void readFloat(char inputs[], int16_t *start, int index){
	char str[8];

	int i = 0;
	while(inputs[*start] != '|' && inputs[*start] != ETX){
		str[i] = inputs[*start];
		*start = *start + 1;
		i = i + 1;
	}
	str[i] = '\0';
	*start = *start + 1;

	follow_float_variables[index] = (float)atof(str);

}