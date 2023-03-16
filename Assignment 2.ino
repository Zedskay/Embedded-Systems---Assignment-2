#include <B31DGMonitor.h>                     // Enables monitor function
#include <Ticker.h>                           // Enables Ticker function
#include <Arduino.h>                          // Includes basic Arduino library


#define FRAME_DURATION_MS 2                   // 2ms frame duration

#define Task1_signal 4                        // Pin setup. Assigns Task1_signal to Digital Pin 4. This is the square wave generated for task 1
#define Task2_signal 5                        // Pin setup. Assigns Task2_signal to Digital Pin 5. This is the square wave generated externally for task 2
#define Task3_signal 6                        // Pin setup. Assigns Task3_signal to Digital Pin 6. This is the square wave generated externally for task 3
#define Task4_signal 7                        // Pin setup. Assigns Task4_signal to Digital Pin 7. This is the analogue signal generated externally for task 4
#define Task4_error 0                         // Pin setup. Assigns Task4_error to Digital Pin 0. This is the error LED that turns on when average_analogue_in > half of maximum range


B31DGCyclicExecutiveMonitor monitor;          // Name the monitor
Ticker FrameTick;                             // Name the frame ticker

unsigned long frameTime = 0;                  // Initialises Frame Timer, set to 0          
unsigned long frameCounter = 0;               // Initialises Frame Counter, set to 0


int f1 = 0;                                   // First frequency is declared as an integer, set to 0 as default
int f2 = 0;                                   // Second frequency is declared as an integer, set to 0 as default


int fvalue1 = 0;                              // First frequency value is declared as an integer, set to 0 as default
int fvalue2 = 0;                              // Second frequency value is declared as an integer, set to 0 as default




void setup(void)

{
  Serial.begin(9600);                         // Initialises the serial communication to communicate with the PC, set the baud rate to 9600
  while(!Serial);                             // Waits for an active USB connection to be established by the PC
  Serial.println("Ready");                    // Prints "Ready" to serial monitor when complete 

  pinMode(Task1_signal, OUTPUT);              // Pin modes are set, Task1_signal is set as an output
  pinMode(Task2_signal, INPUT);               // Pin modes are set, Task2_signal is set as an input
  pinMode(Task3_signal, INPUT);               // Pin modes are set, Task3_signal is set as an input
  pinMode(Task4_signal, INPUT);               // Pin modes are set, Task4_signal is set as an input
  pinMode(Task4_error, OUTPUT);               // Pin modes are set, Task4_error is set as an output

  
  FrameTick.attach_ms(4, frame);              // Ticker size is set to 4ms for the "frame" function 
  JobTask1();                                 // Run task 1 followed by fixed delay
  delayMicroseconds(2000);                    // Delay is neccessary to allow monitoring to start
  monitor.startMonitoring();                  // Start monitoring scheme 
}





void frame() {                                // Frame function, which organises the job tasks based on their seqeunce. This stops tasks interfering with one another

  unsigned int position = frameCounter % 10;  // Frame positions range from case 0 to 9
                                              //Iterates through the case statement to operate the Job Tasks based their respected case position 

   switch (position) {                       // Different cases based on the Tasks                   

     case 0: JobTask1();              JobTask3(); JobTask4(); JobTask5(); break;
     case 1: JobTask1();  JobTask2();                                     break;
     case 2: JobTask1();              JobTask3();                         break;
     case 3: JobTask1();                                                  break;
     case 4: JobTask1();              JobTask3();                         break;
     case 5: JobTask1();                          JobTask4(); JobTask5(); break;
     case 6: JobTask1();              JobTask3();                         break;
     case 7: JobTask1(); JobTask2();                                      break;
     case 8: JobTask1();              JobTask3();                         break;
     case 9: JobTask1();                                                  break;
  }
  frameCounter = frameCounter + 1;            // Frame counter increments
}



void loop(void)                               // Single time slot function of the Cyclic Executive (repeating)
{
  unsigned long bT = micros();                

  for (int i=0; i<1000; i++) {                // int is set to 0, i increments and finishes when i = 1000
    JobTask1();                               // When i < 1000, JobTask1 is happening
  }

  unsigned long timetaken = micros()-bT;      

  Serial.print("Duration SerialOutput Job = ");     // Serial print the duration of the job previously completed
  Serial.print(timetaken);                          // Serial print the time taken for said task

  exit(0);                                          // Terminate loop
}




void JobTask1(void)                           // Function for Task 1
{
  monitor.jobStarted(1);                      // Job has started and is monitored

  digitalWrite(Task1_signal, HIGH);           // digitalWrite writes a digital value to pin 6, in this case it is high
  delayMicroseconds(200);                     // Delay. The code will wait X value before going to the next step, in this case it waits 200 microseconds
  digitalWrite(Task1_signal, LOW);            // digitalWrite writes a digital value to pin 6, in this case it is low
  delayMicroseconds(50);                      // Delay. The code will wait X value before going to the next step, in this case it waits 50 microseconds
  digitalWrite(Task1_signal, HIGH);           // digitalWrite writes a digital value to pin 6, in this case it is high
  delayMicroseconds(30);                      // Delay. The code will wait X value before going to the next step, in this case it waits 30 microseconds
  digitalWrite(Task1_signal, LOW);            // digitalWrite writes a digital value to pin 6, in this case it is low
  delayMicroseconds(3720);                    // Delay. The code will wait X value before going to the next step, in this case it waits 3720 microseconds

  monitor.jobEnded(1);                        // Job has ended and is no longer monitored
} 



void JobTask2(void)                                  // Function for Task 2
{


  monitor.jobStarted(2);                            // Job has started and is monitored

  int bT1 = micros();
  int Cycle1 = pulseIn(Task2_signal, HIGH, 4000);   // Cycle 1, Task2 input signal, high with a limit of 4 ms

  if(Cycle1 > 0){
    f1 = 1/(2*Cycle1*0.000001);                     // Calculates the frequency of the Task 2 signal, if Cycle 1 > 0
  }
  else{ 
    f1 = 333;                                       // If Cycle 1 is not greater than 0, the frequency for Task 2 is 333Hz. The lower limit of the frequency 
  }
  
  Serial.println(f1);                               // Serial print this frequency 
  monitor.jobEnded(2);                              // Job has ended and is no longer monitored
} 



void JobTask3(void)                                  // Function for Task 3
{
   monitor.jobStarted(3);                            // Job has started and is monitored

  int bT2 = micros();                                             
  int Cycle2 = pulseIn(Task3_signal, HIGH, 3000);   // Cycle 1, Task2 input signal, high with a limit of 3 ms

  if(Cycle2 > 0){
    f2 = 1/(2*Cycle2*0.000001);                     // Calculates the frequency of the Task 3 signal, if Cycle 2 > 0
  }else{
    f2 = 500;                                       // If Cycle 2 is not greater than 0, the frequency for Task 3 is 500Hz. The lower limit of the frequency 
  }

  Serial.println(f2);                               // Serial print this frequency         
  monitor.jobEnded(3);                              // Job has ended and is no longer monitored
} 



void JobTask4(void)                              // Function for Task 4
{

  monitor.jobStarted(4);                         // Job has started and is monitored

  float Read1 = analogRead(Task4_signal);        // Read the signal from Task4_signal and store it as a float called "Read1"
  float Read2 = analogRead(Task4_signal);        // Read the signal from Task4_signal and store it as a float called "Read2"
  float Read3 = analogRead(Task4_signal);        // Read the signal from Task4_signal and store it as a float called "Read3"
  float Read4 = analogRead(Task4_signal);        // Read the signal from Task4_signal and store it as a float called "Read4"
  float Average = (Read1 + Read2 + Read3 + Read4)/(4*4095/3.3);                 // Calculate the average of these all and convert to a voltage signal

  if (Average >= 1.65){                         // If the average is less than or equal to half 3.3V (since 3.3/2 = 1.65). Value based on position of potentiometer
    digitalWrite(Task4_error, HIGH);            // Write digital value to Task4_error, this is an LED. This turns on the LED                

  } else {                                      // Else, if the above is not true
    digitalWrite(Task4_error, LOW);             // Write digital value and turn the error LED off
  }
   Serial.println(Average);                     // Serial print this average value
   monitor.jobEnded(4);                         // Job has ended and is no longer monitored
} 



void JobTask5(void)                             // Function for Task 5
{
   if(frameCounter % 25 == 0){                  // Remainder after divison    

    monitor.jobStarted(5);                      // Job has started and is monitored

    fvalue1 = map(f1,333,1000, 0, 99);          // Value of Task 2 frequency, lower limit of 333Hz is 0% and 1000Hz is 99%
    fvalue2 = map(f2,500,1000, 0, 99)           // Value of Task 3 frequency, lower limit of 500Hz is 0% and 1000Hz is 99%; 
    fvalue1 = constrain(fvalue1,0,99);          // Constrains these numbers to the range 0-99
    fvalue2 = constrain(fvalue2,0,99);          // Constrains these numbers to the range 0-99 

    Serial.println(fvalue1);                    // Serial print the value of the frequency for Task 2
    Serial.println(fvalue2);;                   // Serial print the value of the frequency for Task 3

    Serial.printf("%d, %d \n", fvalue2, fvalue1);         // Print these aforementioned frequencies, in the form A, B
    delay(1000);                                          // Delay to help make the Serial print more readable 

 
    monitor.jobEnded(5);                        // Job has ended and is no longer monitored
   }
}