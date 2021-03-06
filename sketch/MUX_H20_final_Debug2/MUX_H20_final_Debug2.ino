// Author: ____Bartek Zuranski_____Reading analog sensors with MUX____
// Measurring water level with analog MUX channels, I'm using 3 with 4051, can be used as many chanells as on your Mux. 
// Design is as simple as possible, if water drops wire on Mux channel loses ground and trigers alarm.
// Script design in 3 blocks, Mux read, waterLvl check, and analog summarized read for improved accuracy,-> this is used with pre-read & 10nF cap.  
// Resistance value vary on lenght of cables/exposed wires in the water. 

// storage for Value of MuX Channels:
int a,b,c;  // y0, y1, y2
// value of select Mux pins at: 
int r[] = { 0, 0, 0 }; // 1st, 2nd, 3rd read
// selectPins esp to MUX A,B,C
int selPin[] = { 2, 4, 16 };
int count = 0;    // count Mux'Y' pin selection

void setup(){  
  Serial.begin(115200);
  for(int pin = 0; pin < 3; pin++){      // setup of Mux select pins
    pinMode(selPin[pin], OUTPUT);
  } 
}

int readH2O(){
  // select the read sequence bit for 3 channels of water level sensor
  for (count=0; count<=2; count++) {
    r[0] = bitRead(count,0);         
    r[1] = bitRead(count,1);         
    r[2] = bitRead(count,2);         
  // truth table / channel selection 
   digitalWrite(selPin[0], r[0]);
   digitalWrite(selPin[1], r[1]);
   digitalWrite(selPin[2], r[2]);
  // store Mux channel value (returned sum of 10 readings)
  switch(count){
  case 0:
    a = suma();
    break;
  case 1:
    b = suma();
    break;
  case 2:
    c = suma();
    break;
  }
    Serial.println(a);
    Serial.println(b);
    Serial.println(c);
   h2oCheck();  
  }
}

int h2oCheck(){
   if(a < 10200){
    Serial.println("Full");  // replace debug with return full
   }else{
    if(b < 10200){
      Serial.println("Half");   // return half
    }else{
      if(c < 10200){
        Serial.println("Low");  // return low
      }else{
        Serial.println("EMPTY");  // return empty
      }
    }
   }
}  

int suma(){   
    analogRead(A0);  
    delay(200);
    int sum = 0;
    for (int i = 0; i < 10; i++) {
    sum += analogRead(A0);
    }
    return sum;
}


void loop () {

   readH2O();

}

