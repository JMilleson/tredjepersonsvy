


#define STX 'a'
#define ETX 'b'
#define DIVIDER '|'

#define TRACK_DATA_ID '0'
#define SETTINGS_ID '1'
#define THROTTLE_PID_ID '2'
#define YAW_PID_ID '3'
#define ROLL_PID_ID '4'
#define PITCH_PID_ID '5'

long follow_target_height = -1;
long follow_target_distance = -1;
long follow_sonar_height = -1;
long follow_oculus_yaw = -1;
long follow_oculus_yaw_offset = -1;
long follow_distance_to_user = -1;
long follow_roll_error = -1;
int altitude_updated = -1;

float throttleP = 0.0f, throttleI = 0.0f, throttleD = 0.0f;
float yawP = 0.0f, yawI = 0.0f, yawD = 0.0f;
float rollP = 0.0f, rollI = 0.0f, rollD = 0.0f;
float pitchP = 0.0f, pitchI = 0.0f, pitchD = 0.0f;



void setup() {
  Serial.begin(57600);
}

long receivedCount = 0;
char received[128];

void loop() {
	delay(2000);
	readSerial();
	printVars();
}

void printVars(){
	Serial.print("target height: ");
	Serial.print(follow_target_height);
	Serial.print("\n");
	Serial.print("target distance: ");
	Serial.print(follow_target_distance);
	Serial.print("\n");	
	Serial.print("sonar height: ");
	Serial.print(follow_sonar_height);
	Serial.print("\n");	
	Serial.print("oculus yaw: ");
	Serial.print(follow_oculus_yaw);
	Serial.print("\n");	
	Serial.print("distance to user: ");
	Serial.print(follow_distance_to_user);
	Serial.print("\n");	
	Serial.print("roll error: ");
	Serial.print(follow_roll_error);
	Serial.print("\n");

	Serial.print("throttleP: ");
	Serial.print(throttleP);
	Serial.print("\n");		
	Serial.print("throttleI: ");
	Serial.print(throttleI);
	Serial.print("\n");	
	Serial.print("throttleD: ");
	Serial.print(throttleD);
	Serial.print("\n");

	Serial.print("yawP: ");
	Serial.print(yawP);
	Serial.print("\n");		
	Serial.print("yawI: ");
	Serial.print(yawI);
	Serial.print("\n");	
	Serial.print("yawD: ");
	Serial.print(yawD);
	Serial.print("\n");

	Serial.print("rollP: ");
	Serial.print(rollP);
	Serial.print("\n");		
	Serial.print("rollI: ");
	Serial.print(rollI);
	Serial.print("\n");	
	Serial.print("rollD: ");
	Serial.print(rollD);
	Serial.print("\n");

	Serial.print("pitchP: ");
	Serial.print(pitchP);
	Serial.print("\n");		
	Serial.print("pitchI: ");
	Serial.print(pitchI);
	Serial.print("\n");	
	Serial.print("pitchD: ");
	Serial.print(pitchD);
	Serial.print("\n");


}

void readSerial(){
	if(receivedCount == 0){
		while(Serial.available()){
			if(Serial.read() == STX){
				received[receivedCount++] = STX;
				break;
			}
		}
	}

	while(Serial.available() && receivedCount > 0 && received[receivedCount - 1] != ETX){
		received[receivedCount++] = Serial.read();
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
				updateYawPid();
				receivedCount = 0;
				break;
			case ROLL_PID_ID:
				updateRollPid();
				receivedCount = 0;
				break;
			case PITCH_PID_ID:
				updatePitchPid();
				receivedCount = 0;
				break;

		}
	}
}

void updateTrackData(){
	long index = 3;
	follow_sonar_height = readInt(&index);
	//follow_sonar_height = compensate for tilt
	follow_oculus_yaw = readInt(&index);
	//follow_oculus_yaw = follow_oculus_yaw +- offset compensate for offset
	follow_distance_to_user = readInt(&index);
	follow_roll_error = readInt(&index);
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


