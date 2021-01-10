// defines pins numbers

// Motor
const int stepPin = 6; 
const int dirPin = 5;

// UI
const int potentiometerPin = 5;
const int stepButtonPin = 7;
const int startStopButtonPin = 2;
const int redLedPin = 11;

// Status
int startStop = 0; // 0 stop, 1 start

// Timers
long interrupt_debouncing_time = 1000; // Millis
unsigned long last_interrupt_time = 0;

long step_debouncing_time = 500; // Millis
unsigned long last_step_time = 0;

// Rotation controls
unsigned long last_rotation_time = 0;
unsigned long selected_rotation_time = 0;

void setup() 
{
  // Configure Pins
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(stepButtonPin, INPUT);
  pinMode(startStopButtonPin, INPUT);
  pinMode(redLedPin, OUTPUT);

  // Setup interrupt
  attachInterrupt(digitalPinToInterrupt(startStopButtonPin), start_stop_trigger, RISING);

  // Enables the motor to move in a particular direction
  digitalWrite(dirPin,HIGH);

  Serial.begin(9600);
}

void loop() 
{

  // read potentiometer value for time selection
  int val = analogRead(potentiometerPin);
  selected_rotation_time = calculate_time(val);

  if(startStop)
  {
    // rotation
    
    unsigned long time_rotation = millis();

    if (time_rotation - last_rotation_time >= selected_rotation_time)
    {
      for(int x = 0; x < 10; x++) {
        
        digitalWrite(stepPin,HIGH); 
        delayMicroseconds(10000); 
        digitalWrite(stepPin,LOW); 
        delayMicroseconds(10000); 
      }

      Serial.print("Rotation executed: ");
      Serial.print("time selected: ");
      Serial.print(selected_rotation_time);
      Serial.print("time execution: ");
      Serial.print(time_rotation - last_rotation_time);
      Serial.println();

      last_rotation_time = millis();
    }
  }
  else
  {    
    // configuration

    if (digitalRead(stepButtonPin) == HIGH) {

      unsigned long step_time = millis();
  
      if (step_time - last_step_time > step_debouncing_time)
      {
        digitalWrite(stepPin,HIGH); 
        delayMicroseconds(10000); 
        digitalWrite(stepPin,LOW); 
        delayMicroseconds(10000);       
  
        last_step_time = millis();
  
        Serial.println("One step rotation executed");
      }
    }  
  }
}

unsigned long calculate_time(int input_value)
{
  unsigned long option = 0;
  
  if(input_value > 0 && input_value <= 204)
  {
    option = 60000; // 1 minute
  }
  else if(input_value > 204 && input_value <= 408)
  {
    option = 120000; // 2 minute
  }
  else if(input_value > 408 && input_value <= 612)
  {
    option = 180000; // 3 minute
  }
  else if(input_value > 612 && input_value <= 816)
  {
    option = 240000; // 4 minute
  }
  else
  {
    option = 300000; // 5 minute
  }

  return option;
}

void start_stop_trigger() {

  unsigned long interrupt_time = millis();
  
  if (interrupt_time - last_interrupt_time > interrupt_debouncing_time)  
  {
    startStop = !startStop;
    digitalWrite(redLedPin, startStop);

    Serial.print("Interrupt received, start/stop value: ");

    if(startStop)
    {
      Serial.print("Start! - Selected time: ");
      Serial.print(selected_rotation_time);
    }
    else
    {
      Serial.print("Stop!");
    }
    
    Serial.println();

    last_interrupt_time = millis();
  }
}
