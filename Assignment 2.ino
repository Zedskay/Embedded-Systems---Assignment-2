

int OUT1 = 6;                                                                 // Pin 6 on the arduino is set as an output, this is the output of specification 1. (Square wave)





void setup() {                                                                // Code setup, initialises components

  pinMode(OUT1, OUTPUT);                                                      // Sets the pin mode of pin 6, this sets it up as an output 
  
  
}


void loop () {                                                                // Main loop. This piece of code loops forever 
  Spec1();                                                                    // Calls the function "Spec1"



}


int Spec1(){                                                                  // This is the function for Specification 1, which generates a digital signal that is high for 200us, low for 50us, then high again for 30us. This repeats every 4 ms
  digitalWrite(OUT1, HIGH);                                                   // digitalWrite writes a digital value to pin 6, in this case it is high 
  delayMicroseconds(200);                                                     // Delay. The code will wait X value before going to the next step, in this case it waits 200 microseconds
  digitalWrite(OUT1, LOW);                                                    // digitalWrite writes a digital value to pin 6, in this case it is low 
  delayMicroseconds(50);                                                      // Delay. The code will wait X value before going to the next step, in this case it waits 50 microseconds
  digitalWrite(OUT1, HIGH);                                                   // digitalWrite writes a digital value to pin 6, in this case it is high 
  delayMicroseconds(30);                                                      // Delay. The code will wait X value before going to the next step, in this case it waits 30 microseconds
  digitalWrite(OUT1, LOW);                                                    // digitalWrite writes a digital value to pin 6, in this case it is low
  delayMicroseconds(3720);                                                    // Delay. The code will wait X value before going to the next step, in this case it waits 3720 microseconds

}