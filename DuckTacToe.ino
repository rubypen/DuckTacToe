//~~~~~~~~~~~~~~~~~~~~~~~~~~GLOBAL VARS~~~~~~~~~~~~~~~~~~~~~~~~~
//for shift registers & RGB
int clockPin1, clockPin2,
	dataPin1, dataPin2,
	latchPin;
int redRGB, blueRGB;
byte currentState1, currentState2;

//to know which bit to change for each RGB color
int redR1R5 = 7, redR2R6 = 5, redR3R7 = 3, redR4R8 = 1,
	blueR1R5 = 6, blueR2R6 = 4, blueR3R7 = 2, blueR4R8 = 0;

//Variables to control RGBs and know which should be ON || OFF
int board[3][3] = {{0, 0, 0},
                   {0, 0, 0},
				   {0, 0, 0}};//get an element using [r][c]*/

int whichOn[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};//know which !toTraverse
int ind = 0;

//buzzer control
int buzzer;
int cSharp5; 
int b4;
int a4;
int gSharp4;
int fSharp4;
int non;


 int c;
 int d;
 int e;
 int g;

//Button control
bool bPressed1, bPressed2, bPressed3, cycle;
int button1, button2, button3;
int bPCount, currLED;

//Print vars
int RGB;

//GAME CONTROL VARS
int turn = 1; long randomNum; bool stop = false;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~SETUP~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void setup()
{
  Serial.begin(9600);
  
  //ensures random number each time
  randomSeed(analogRead(0));
  randomNum = random(1, 3);
  
  //shift register + RGBs
  clockPin1 = 4; clockPin2 = 7;
  dataPin1 = 3; dataPin2 = 5;  
  latchPin = 2;
  redRGB = 9; blueRGB = 10;
  
  //BUZZER + NOTES
  buzzer = 11;
  c = 262;
  d = 294;
  e = 330;
  g = 392;
  
  a4 = 440.00;
  non = 0;
  cSharp5 = 554.37;
  gSharp4 = 415.30;
  b4 = 493.88;
  fSharp4 = 369.99;

  
  //Buttons + control
  button1 = 12; button2 = 13; button3 = 8;
  bPressed1 = false; bPressed2 = false; bPressed3 = false;
  currLED = 0;
  bPCount = 0;
  cycle = false;
  
  //For printing
  RGB = 0;
  
  //SHIFT REGISTERS + RGBs
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin1, OUTPUT);
  pinMode(clockPin1, OUTPUT);
  pinMode(dataPin2, OUTPUT);
  pinMode(clockPin2, OUTPUT);
  pinMode(redRGB, OUTPUT);
  pinMode(blueRGB, OUTPUT);
  
  //BUZZER
  pinMode(buzzer, OUTPUT);
  
  //BUTTONS
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  
  //INITIALIZING BYTES FOR SHIFT REGISTER
  currentState1 = 0B00000000;//RGBs 1-4
                  //76543210
  currentState2 = 0B00000000;//RGBs 5-8
                  //76543210
  
  
  initialStateOfSR();
  
  //Printing my initial important var values
  //myVars();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~LOOP~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void loop()
{
  play();
  
  initialStateOfSR();  
  
  delay(30);
}
void playHappySong()
{
  int playHappySong[] = {c,   non,	c,   g,   non,  g,   e,   non, e,   d,   non,  d,   c,   non,  c,  g,   non,  g,   e,    d,     c,   non, c};
  int notesDuration[] = {500, 200,  500, 500, 200,  500, 500, 200, 500, 500, 200,  500, 500, 200, 500, 500, 200,  500, 500,  500,   500, 200, 500};
  playTones(buzzer, playHappySong, notesDuration, 36);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~PLAY~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void play()
{
  //toTraverseThroughLEDsBasedOnTurn
  if(!stop){
    if((randomNum == 1 && turn%2 == 1) || (randomNum == 2 && turn%2 == 0))
    	manualCycleBlue();
    else if((randomNum == 1 && turn%2 == 0) || (randomNum == 2 && turn%2 == 1))
      	manualCycleRed();
  }
   
  //toEndGameWhenSum1Won
  if(checkWin() == 1){
    stop = true; blueWins();
    //playHappySong();
  }
  else if(checkWin() == 2){
    stop = true; redWins();
    //playHappySong();
  }
  
  //toReset
  bool b3State = digitalRead(button3);  
  
  if(b3State == 1 && !bPressed3)
  {   
    bPressed3 = true;
    reset();
    play();
  }
  else if(b3State == 0)
    bPressed3 = false;
}

//~~~~~~~~~~~~~~~~~~~~~~~GAME METHODS~~~~~~~~~~~~~~~~~~~~~~~~~~
void reset()
{
  stop = false;
  currLED = 0; RGB = 0; turn = 1;
  clearBoardand1D();
  randomNum = random(1, 3);
  noTone(buzzer);
  allOff();
}
void clearBoardand1D()
{
  //2D CLEAR
  for(int r=0; r<3; r++)
  {
    for(int c=0; c<3; c++)
    {
      board[r][c] = 0;
    }
  }
  
  //1D CLEAR
  for(int i=0; i<9; i++)
    whichOn[i] = 0;
}
int checkWin()
{
  if(majorDiagnol(1) || minorDiagnol(1) || colsWin(1) || rowsWin(1))
    return 1;//1 is blue
  else if(majorDiagnol(2) || minorDiagnol(2) || colsWin(2) || rowsWin(2))
	return 2;//2 is red
  else
    return 0;//0 means noone won...yet
}
bool majorDiagnol(int num)//left top --> bottom right
{
  if(board[0][0] == num && board[1][1] == num && board[2][2] == num)
    return true;//1 is true
  return false;//0 is false
}  
bool minorDiagnol(int num)//right top --> left bottom 
{
  if(board[0][2] == num && board[1][1] == num && board[2][0] == num)
    return true;//1 is true
  return false;//0 is false
}  

bool colsWin(int num)
{
  if(checkColWin(0, num) || checkColWin(1, num) || checkColWin(2, num))
    return true;
  return false;
}
bool checkColWin(int col, int num)
{
  for(int r=0; r<3; r++){
    if(board[r][col] != num)
      return false;
  }
  return true;
}

bool rowsWin(int num)
{
  if(checkRowWin(0, num) || checkRowWin(1, num) || checkRowWin(2, num))
    return true;
  return false;
}
bool checkRowWin(int row, int num)
{
  for(int c=0; c<3; c++){
    if(board[row][c] != num)
      return false;
  }
  return true;
}

void blueWins()
{//make cool pattern
  long sec = millis()/500;
 
  if(sec%3 == 0)
    allBlue();
  else if(sec%3 == 1)
  	allPink();
  else 
  	allBlue();
}
void redWins()
{//make cool pattern
  long sec = millis()/500;
 
  if(sec%3 == 0)
    allRed();
  else if(sec%3 == 1)
  	allPink();
  else 
  	allRed();
}
void updateBoard(int ledNum, int color)
{
  // leds: 1 2 3 4 5 6 7 8 9
  //       0 1 2 3 4 5 6 7 8
  int whatLED = ledNum+1;
  int row = 0, col = 0;
  if((float)whatLED/3 <= 1)
    row = 0;
  else if((float)whatLED/3 <= 2)
    row = 1;
  else if((float)whatLED/3 <= 3)
    row = 2;
  
  if(whatLED%3 == 0)
    col = 2;
  else if(whatLED%3 == 1)
    col = 0;
  else if(whatLED%3 == 2)
    col = 1;
    
  board[row][col] = color;
}

//~~~~~~~~~~~~~~~~~~~~RGB CONTROL  METHODS~~~~~~~~~~~~~~~~~~~~~
void manualCycleRed()
{
  bool b1State = digitalRead(button1),
  	   b2State = digitalRead(button2);  
  
  if(b1State == 1 && !bPressed1)
  {   
    bPressed1 = true; currLED++; 
  }
  else if(b2State == 1 && !bPressed2)
  {   
    bPressed2 = true;
    whichOn[currLED%9] = 2;	turn++; updateBoard(currLED%9, 2);
  }
  else if(b1State == 0)
    bPressed1 = false;
  else if(b2State == 0)
    bPressed2 = false;
    
  for(int i = 0; i < 9; i++)
  {
    //vars :keep track of array RGB and actual RGB
    int whichLED = currLED%9, theLED = whichLED+1;
    
    //adjusting previous LEDs affected
    if(whichLED == i){
      if(i == 0){
        if(whichOn[8] == 0)
        turnOff(9);
      }
      else if( i > 0){
        if(whichOn[i-1] == 0)
        turnOff(i);
      }
    }
    
    //actually updating LEDs
    if(whichLED == i && whichOn[whichLED] == 0){
      turnRed(theLED); RGB = theLED;
    }
    else if(whichLED == i && whichOn[whichLED] != 0)
      currLED++;   
  }
}
void manualCycleBlue()
{
  bool b1State = digitalRead(button1),
  	   b2State = digitalRead(button2);  
  
  if(b1State == 1 && !bPressed1)
  {   
    bPressed1 = true; currLED++; 
  }
  else if(b2State == 1 && !bPressed2)
  {   
    bPressed2 = true;
    whichOn[currLED%9] = 1;	turn++; updateBoard(currLED%9, 1);
  }
  else if(b1State == 0)
    bPressed1 = false;
  else if(b2State == 0)
    bPressed2 = false;
    
  for(int i = 0; i < 9; i++)
  {
    int whichLED = currLED%9, theLED = whichLED+1;
    
    if(whichLED == i){
      if(i == 0){
        if(whichOn[8] == 0)
        turnOff(9);
      }
      else if( i > 0){
        if(whichOn[i-1] == 0)
        turnOff(i);
      }
    }
    
    if(whichLED == i && whichOn[whichLED] == 0){
      turnBlue(theLED); RGB = theLED;
    }
    else if(whichLED == i && whichOn[whichLED] != 0)
      currLED++;   
  }
}

//~~~~~~~~~~~~~~~~~~~~~RGB COLOR METHODS~~~~~~~~~~~~~~~~~~~~~~~
void allBlue()
{
  allOff();
  turnBlue(1); turnBlue(2); turnBlue(3); turnBlue(4);
  turnBlue(5); turnBlue(6); turnBlue(7); turnBlue(8);
  turnBlue(9);
}
void allRed()
{
  allOff();
  turnRed(1); turnRed(2); turnRed(3); turnRed(4);
  turnRed(5); turnRed(6); turnRed(7); turnRed(8);
  turnRed(9);
}
void allPink()
{
  allOff();
  turnPink(1); turnPink(2); turnPink(3); turnPink(4);
  turnPink(5); turnPink(6); turnPink(7); turnPink(8);
  turnPink(9);
}
void allOff()
{
  currentState1 = 0B00000000; currentState2 = 0B00000000;
  digitalWrite(redRGB, 0); digitalWrite(blueRGB, 0);
}
void turnOff(int RGB)
{
  if(RGB == 1){
    bitWrite(currentState1, redR1R5, 0);
    bitWrite(currentState1, blueR1R5, 0);}    
  else if(RGB == 2){
    bitWrite(currentState1, redR2R6, 0);
  	bitWrite(currentState1, blueR2R6, 0);}
  else if(RGB == 3){
    bitWrite(currentState1, redR3R7, 0);
  	bitWrite(currentState1, blueR3R7, 0);}
  else if(RGB == 4){
    bitWrite(currentState1, redR4R8, 0);
  	bitWrite(currentState1, blueR4R8, 0);}
  else if(RGB == 5){    
    bitWrite(currentState2, redR1R5, 0);
  	bitWrite(currentState2, blueR1R5, 0);}
  else if(RGB == 6){
    bitWrite(currentState2, redR2R6, 0);
  	bitWrite(currentState2, blueR2R6, 0);}
  else if(RGB == 7){
    bitWrite(currentState2, redR3R7, 0);
  	bitWrite(currentState2, blueR3R7, 0);}
  else if(RGB == 8){
    bitWrite(currentState2, redR4R8, 0);
  	bitWrite(currentState2, blueR4R8, 0);}
  else if(RGB == 9){
    digitalWrite(redRGB, LOW);
    digitalWrite(blueRGB, LOW);}
}
void turnRed(int RGB)
{
  if(RGB == 1)
    bitWrite(currentState1, redR1R5, 1);
  else if(RGB == 2)  
    bitWrite(currentState1, redR2R6, 1);
  else if(RGB == 3)
    bitWrite(currentState1, redR3R7, 1);
  else if(RGB == 4)
    bitWrite(currentState1, redR4R8, 1);
  else if(RGB == 5)
    bitWrite(currentState2, redR1R5, 1);
  else if(RGB == 6)
    bitWrite(currentState2, redR2R6, 1);
  else if(RGB == 7)
  	bitWrite(currentState2, redR3R7, 1);
  else if(RGB == 8)
    bitWrite(currentState2, redR4R8, 1);
  else if(RGB == 9)
    digitalWrite(redRGB, HIGH);
}
void turnBlue(int RGB)
{
  if(RGB == 1)
    bitWrite(currentState1, blueR1R5, 1);
  else if(RGB == 2)
    bitWrite(currentState1, blueR2R6, 1);
  else if(RGB == 3)
    bitWrite(currentState1, blueR3R7, 1);
  else if(RGB == 4)
    bitWrite(currentState1, blueR4R8, 1);
  else if(RGB == 5)
    bitWrite(currentState2, blueR1R5, 1);
  else if(RGB == 6)
    bitWrite(currentState2, blueR2R6, 1);
  else if(RGB == 7)
  	bitWrite(currentState2, blueR3R7, 1);
  else if(RGB == 8)
    bitWrite(currentState2, blueR4R8, 1);
  else if(RGB == 9)
    digitalWrite(blueRGB, HIGH);
}
void turnPink(int RGB)
{
  if(RGB == 1){
    turnRed(1); turnBlue(1);}
  else if(RGB == 2){
    turnRed(2); turnBlue(2);}
  else if(RGB == 3){
    turnRed(3); turnBlue(3);}
  else if(RGB == 4){
    turnRed(4); turnBlue(4);}
  else if(RGB == 5){
    turnRed(5); turnBlue(5);}
  else if(RGB == 6){
    turnRed(6); turnBlue(6);}
  else if(RGB == 7){
    turnRed(7); turnBlue(7);}
  else if(RGB == 8){
    turnRed(8); turnBlue(8);}
  else if(RGB == 9){
    turnRed(9); turnBlue(9);}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~MUSIC~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void playTones(int buzzerPIN, int tones[], int durations[], int length){
  if(sizeof(tones)/sizeof(int) == sizeof(durations)/sizeof(int))
  {
    long sum = 0;
    for(int i = 0; i<length;i++)
    {
      sum += durations[i];
    }
 
    //calculate which index we should be working with
    int musicI = 0;
    for(musicI = 0; millis()%sum>=sumUntil(durations,musicI,length);musicI++)//super bananas
    {    }
    if(millis()%sum<sumUntil(durations,musicI,length))    //bananas
    {
      if(tones[musicI]!= 0)
      {
        tone(buzzerPIN, tones[musicI]);
      }
      else
      {
        noTone(buzzerPIN);
      }
    }
  }
}//end playtones()
 
long sumUntil(int arr[], int index,int length)
{
  long sum = 0;
  for(int i = 0;i<=index;i++)
  {
    sum+=arr[i];
  }
 
  return sum;
 
}//end sumUntil()

//~~~~~~~~~~~~~~~~~~~~~~~Fundamental method~~~~~~~~~~~~~~~~~~~~~
void initialStateOfSR()
{
  //allows us to edit 
  digitalWrite(latchPin, LOW);  

  //edits the shift register.
  //MSBFIRST Most Significant Bit first,  left to right
  //LSBFIRST Least Significant Bit first,  right to left
  shiftOut(dataPin1, clockPin1, LSBFIRST, currentState1);  
  shiftOut(dataPin2, clockPin2, LSBFIRST, currentState2);
  
  //stops the editing.
  digitalWrite(latchPin, HIGH);
}

/*~~~~~~~~~~~~~~~~~~~~~~Print Method~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void myVars()
{
  Serial.println("---------------------------------");
  Serial.print("turn = "); Serial.println(turn);
  
  Serial.print("RGB = "); Serial.println(RGB);
  
  //PRINTING 1D ARRAY
  Serial.println("whichOn[]: ");
  for(int i=0; i<8; i++)
  {
    Serial.print(whichOn[i]); Serial.print(", ");
  }
  Serial.println(whichOn[8]); Serial.println();
  
  //PRINTING 2D ARRAY
  Serial.println("board[]: ");
  for(int r=0; r<3; r++)
  {
    for(int c=0; c<3; c++)
    {
      Serial.print(board[r][c]); Serial.print(", ");
    }
    Serial.println();
  }
  Serial.println("---------------------------------");
  Serial.println();
}*/
