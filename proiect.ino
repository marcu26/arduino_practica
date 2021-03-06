#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <IRremote.h>
LiquidCrystal_I2C lcd(0x20,16,2);
Servo capRobot; 

#define echoPin 2 
#define trigPin 3
#define greenLed 10
#define redLed 9
#define servoPin 5
#define buttonPin 7
const int micPin  = A0;

int RECV_PIN=12; 
IRrecv irrecv(RECV_PIN); 


long duration;
int distance; 
int angle = 90;
bool way=0;
String line1;
String line2;
int option=0;
int buttonState = 0;  



void setup(){
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(micPin, INPUT);

  digitalWrite(greenLed,LOW);
  digitalWrite(redLed,LOW);
  
  
  lcd.init();                      
  lcd.backlight();
  
  lcd.home();

  capRobot.attach(servoPin);
  capRobot.write(angle);


  irrecv.enableIRIn(); 
}


  
void loop()
{
  showMenu();
}


//############################# Distanta 

int getDistance()
{

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  return distance;
}

//############################# 


//############################# mai cauta

void maiCauta()
{
  if(way==true && angle <170)
  {
  angle+=20;
  return;
  }

  else if(way==true && angle==170)
  {
    way=false;
    return;
  }

  else if(way==false && angle>10)
  {
    angle-=20;
    return;
  }

    else if(way==false && angle==10)
  {
    way=true;
    return;
  }
}
//############################# 

//############################# Change led text

void changeLedText(String text1, String text2)
{
 if(line1 != text1 || line2 != text2)
 {
  
  int xPos=8-text1.length()/2;
  if(text1.length()%2==1)
  xPos-=1;
  lcd.clear();
  lcd.setCursor(xPos,0);
  lcd.print(text1);
  line1=text1;
  
  xPos=8-text2.length()/2;
  if(text2.length()%2==1)
  xPos-=1;
  lcd.setCursor(xPos,1);
  lcd.print(text2);
  line2=text2;
 }      
}

//############################# 

//############################# input telecomanda
String getInput()
{
  String recived="";
  delay(100);
  if (irrecv.decode())
  {
  recived=String(irrecv.decodedIRData.decodedRawData, HEX);
  irrecv.resume(); 
  }
  Serial.println(recived);
  return recived;
}
//############################# 


//############################# distanta cap

void pastreazaDistanta()
{
  while(1)
  {
  if(getInput()=="ad52ff00")
  break;
  
  String s="Dist: ";
  int dist=getDistance();
  s=s+String(dist);
  delay(100);
  if(dist>=100)
  {
    changeLedText(s,"Distanta ok");
    maiCauta();
    capRobot.write(angle);
    aprindeVerde();
    delay(500);
  }

  else
  {
  int distance=getDistance();
  
  changeLedText(s,"Pastreada dist.");
  aprindeRosu();
  delay(500);
  
  }
  }
  showMenu();
}

//############################# 


//############################# 

void showMenu()
{
  capRobot.write(90);
  inchideLeduri();
 if(option==0)
 {
   String input=getInput();
   changeLedText("Pastreaza","Distanta >");
   if(input=="e31cff00")
   pastreazaDistanta();

   if(input=="a55aff00")
   {
   option++;
   }
 }

  if(option==1)
  {
     String input=getInput();
   changeLedText("Pastreaza","< Linistea >");
   if(input=="e31cff00")
   pastreazaLinistea();

   if(input=="a55aff00")
   {
   option++;
   }

   if(input=="f708ff00")
   {
    option--;
   }
 
  }

  if(option==2)
  {
     String input=getInput();
    changeLedText("Joc","< Reflex >");
    if(input=="e31cff00")
    jocReflex();
   
   if(input=="a55aff00"){
   option++;
   }

   if(input=="f708ff00")
   {
    option--;
   
   }
  }

   if(option==3)
  {
     String input=getInput();
    changeLedText("Roll","< Dice >");
    if(input=="e31cff00")
    rollDice();

    if(input=="a55aff00"){
      option++;
     
      }

   if(input=="f708ff00")
   {
    option--;
    
   }
    
  }

  
   if(option==4)
  {
    String input=getInput();
    changeLedText("6","< Din 49 ");
    if(input=="e31cff00")
    {
      _6din49();
    }

   if(input=="f708ff00")
   {
    option--;
   }
  }
}

//############################# 

//############################# Joc reflex

void jocReflex()
{
  changeLedText("Wait","Till green");
  int x=random(2000,10000);
  aprindeRosu();
  delay(x);
  aprindeVerde();
  changeLedText("Push!!!","");
  int i=0;
  int j=0;
  while(1)
  {
    buttonState = digitalRead(buttonPin);
    if(buttonState == HIGH)
    {
      break;
    }
    j++;
    if(j==1000)
    {
    j=0;
    i++;
    }
  }
  String scor=String(i);
  changeLedText("Scor",scor);
  delay(2000);
  showMenu();
}

//############################# 

//#############################  LINISTE
void pastreazaLinistea()
{
  while(1)
  {
  if(getInput()=="ad52ff00")
  break;
  int var=analogRead(micPin);
  delay(200);
  if(var>=85)
  {
  aprindeRosu();
  changeLedText("E galagie!",String(var));
  }
  else {
  aprindeVerde();
   changeLedText("E liniste!",String(var));}
  
  }
  showMenu(); 
}
//############################# 


//############################# led control
void aprindeRosu()
{
  digitalWrite(redLed,HIGH);
  digitalWrite(greenLed,LOW);
}


void aprindeVerde()
{
  digitalWrite(greenLed,HIGH);
  digitalWrite(redLed,LOW);
}

void inchideLeduri()
{
  digitalWrite(greenLed,LOW);
  digitalWrite(redLed,LOW);
}

//############################# 

//#############################

void rollDice()
{
  int nrZaruri=1;
  String s="";
  int caz;

  while(1)
  {
  changeLedText("Nr zaruri:",String(nrZaruri));
  String input=getInput();
  if(input=="e31cff00"){
  caz=0;
  break;}

  if(input=="e619ff00"){
  caz=1;
  break;}
  

  if(input=="a55aff00" && nrZaruri<6)
  nrZaruri++;

  if(input=="f708ff00" && nrZaruri>=2)
  nrZaruri--;

  }
   changeLedText("3...","");
   delay(1000);
   changeLedText("2...","");
    delay(1000);
   changeLedText("1...","");
    delay(1000);
   

if(caz==0)
{
  for(int i=0;i<nrZaruri-1;i++)
  {
   s=s+String(random(1,7))+" ";
  }
   s=s+String(random(1,7));
   
   changeLedText("Rezultat: ",s);
   delay(2000);
}

if(caz==1)
{

   for(int i=0;i<nrZaruri-1;i++)
  {
   if(i%3!=0)
   {
   s=s+String(6)+" ";
   }
   else
   {
    s=s+String(5)+" ";
   }
  }
   s=s+String(6);
   
   changeLedText("Rezultat: ",s);
   delay(2000);
}
}

//#############################

//#############################
void _6din49()
{
  int  v[6]={0};
  int j=0;
  String s1="";
  String s2="";
  int nr=0;

  for(int i=0;i<6;i++)
  {
    while(existsnr(v,nr))
    {
      nr=random(1,50);
    }
    v[i]=nr;
  }

  s1=String(v[0])+" "+String(v[1])+" "+String(v[2]);
  s2=String(v[3])+" "+String(v[4])+" "+String(v[5]);
  
   
  changeLedText("3...","");
   delay(1000);
   changeLedText("2...","");
    delay(1000);
   changeLedText("1...","");
    delay(1000);


changeLedText(s1,s2);
while(1)
{
  if(getInput()=="ad52ff00")
  break;
}
    
}
//#############################


//#############################

bool existsnr(int v[6], int nr)
{
  for(int i=0;i<6;i++)
{
  if(nr==v[i])
  return 1;
}
return 0;
}
