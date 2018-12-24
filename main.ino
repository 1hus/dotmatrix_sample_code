#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

int arrow_count = 0;

uint8_t sign_dot[256] = {
  0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,
  0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,0,
  0,0,2,2,2,2,1,1,1,2,2,2,2,0,0,0,
  0,2,2,2,1,1,1,1,1,1,1,2,2,2,0,0, //04
  0,2,2,1,1,1,1,1,1,1,1,1,2,2,0,0,
  2,2,2,1,1,1,1,1,1,1,1,1,2,2,2,0,
  2,2,1,1,1,1,1,1,1,1,1,1,1,2,2,0,
  2,2,1,1,1,1,1,1,1,1,1,1,1,2,2,0, //08
  2,2,1,1,1,1,1,1,1,1,1,1,1,2,2,0,
  2,2,2,1,1,1,1,1,1,1,1,1,2,2,2,0,
  0,2,2,1,1,1,1,1,1,1,1,1,2,2,0,0,
  0,2,2,2,1,1,1,1,1,1,1,2,2,2,0,0, //12
  0,0,2,2,2,2,1,1,1,2,2,2,2,0,0,0,
  0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,0,
  0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0  //16
};

uint8_t park_dot[256] = {
  0,1,0,0,0,0,1,3,2,0,0,1,1,0,0,0,
  0,1,1,0,0,0,1,3,2,0,0,1,1,0,0,0,
  2,2,3,3,2,3,3,3,3,3,3,3,3,3,3,2,
  2,2,2,2,2,2,3,3,2,2,2,3,3,2,2,2, //04
  3,2,0,0,0,0,1,1,0,0,0,1,1,0,2,2,
  3,3,0,0,2,2,3,1,0,2,2,1,1,0,3,2,
  0,1,1,0,3,3,1,1,1,3,3,1,1,1,1,1,
  0,0,0,2,2,0,0,0,0,3,2,2,2,2,2,0, //08
  0,0,2,2,2,0,0,0,0,1,2,2,2,2,0,0,
  0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,
  0,0,1,2,3,3,2,2,2,3,2,2,2,3,1,0,
  0,1,1,2,3,3,2,3,2,3,2,3,2,3,1,0, //12
  0,1,1,0,1,1,0,3,2,1,0,1,0,1,1,0,
  1,1,0,0,1,1,0,3,3,1,1,1,0,1,1,0,
  3,3,2,2,3,3,2,2,2,2,2,2,2,3,3,2,
  3,2,2,2,3,3,2,2,2,2,2,2,3,3,3,2  //16
};

Adafruit_NeoPixel p = Adafruit_NeoPixel(256, 1, NEO_GRB + NEO_KHZ800);

void setup() {
  delay(100);
  p.begin();
  
  //randomSeed(analogRead(0));
}

void loop() {
  
  signInit(0);
  
  for(int i=0; i<100; i++){
    eraseNumber(4,4);
    showNumber(4,4,i);
    delay(70);
  }
  
  clearCanvas();
  delay(500);
  
  for(int i=0; i<200; i++){
    arrowNext();
    delay(50);
  }

  clearCanvas();
  delay(500);

  for(int i=0; i<3; i++){
    signInit(1);
    delay(850);
    clearCanvas();
    delay(150);
  }
  
  parking(0);
  delay(3000);
  parking(1);
  delay(3000);
  
}

int posToStrip(int x, int y){
  int result = y * 16 + x;
  return result;
}


void stripToPos(int p, int *x, int *y){
  *x = p % 16;
  *y = p / 16;
}


void clearCanvas(){
  for(int i=0; i<256; i++){
    p.setPixelColor(i,p.Color(0,0,0));
  }
  p.show();
}

void signInit(int type){

  if(type == 0){  //速度標識
    for(int i=0; i<256; i++){
      switch (sign_dot[i]) {
        case 0:
          p.setPixelColor(i,p.Color(0,0,0));
          break;
        case 1:
          p.setPixelColor(i,p.Color(255,255,255));
          break;
        case 2:
          p.setPixelColor(i,p.Color(255,0,0));
          break;
        default:
          p.setPixelColor(i,p.Color(0,0,0));
          break;
      }
    }
  }
  else if(type == 1){  //通行禁止
    for(int i=0; i<256; i++){
      if(sign_dot[i] == 0){
        p.setPixelColor(i,p.Color(0,0,0));
      }
      else{
        p.setPixelColor(i,p.Color(255,0,0));
      }
    }

    for(int iy=0; iy<3; iy++){
      for(int ix=0; ix<13; ix++){
        int n = posToStrip(ix+1, iy+6);
        p.setPixelColor(n,p.Color(255,255,255));
      }
    }
  }

  p.show();
}


void parking(int man){
  
  if(man == 1){
    for(int i=0; i<256; i++){
      if (park_dot[i] == 1 || park_dot[i] == 3){
        p.setPixelColor(i,p.Color(255,0,0));
      }
      else {
        p.setPixelColor(i,p.Color(0,0,0));
      }
    }
  }
  else{
    for(int i=0; i<256; i++){
      if (park_dot[i] == 2 || park_dot[i] == 3){
        p.setPixelColor(i,p.Color(0,255,80));
      }
      else {
        p.setPixelColor(i,p.Color(0,0,0));
      }
    }
  }
  
  p.show();
}



void showNumber(int x, int y, int n){
  int nA,nB;
  int pos = posToStrip(x, y);
  int target;
  
  if (n<0 || n>=100){
    return;
  }

  if (pos > 96) {
    return;
  }

  nA = floor(n/10);
  nB = n%10;

  for(int i=0; i<2; i++){

    if(i==0){
      target = nA;
    }
    else{
      pos = posToStrip(x+4, y);
      target = nB;
    }
    
    switch (target){
      case 0:
        p.setPixelColor(pos,p.Color(0,0,255));    //1L
        p.setPixelColor(pos+1,p.Color(0,0,255));  //1C
        p.setPixelColor(pos+2,p.Color(0,0,255));  //1R
        p.setPixelColor(pos+16,p.Color(0,0,255)); //2L
        p.setPixelColor(pos+18,p.Color(0,0,255)); //2R
        p.setPixelColor(pos+32,p.Color(0,0,255)); //3L
        p.setPixelColor(pos+34,p.Color(0,0,255)); //3R
        p.setPixelColor(pos+48,p.Color(0,0,255)); //4L
        p.setPixelColor(pos+50,p.Color(0,0,255)); //4R
        p.setPixelColor(pos+64,p.Color(0,0,255)); //5L
        p.setPixelColor(pos+66,p.Color(0,0,255)); //5R
        p.setPixelColor(pos+80,p.Color(0,0,255)); //6L
        p.setPixelColor(pos+82,p.Color(0,0,255)); //6R
        p.setPixelColor(pos+96,p.Color(0,0,255)); //7L
        p.setPixelColor(pos+97,p.Color(0,0,255)); //7C
        p.setPixelColor(pos+98,p.Color(0,0,255)); //7R
        break;
  
      case 1:
        p.setPixelColor(pos+2,p.Color(0,0,255));  //1R
        p.setPixelColor(pos+18,p.Color(0,0,255)); //2R
        p.setPixelColor(pos+34,p.Color(0,0,255)); //3R
        p.setPixelColor(pos+50,p.Color(0,0,255)); //4R
        p.setPixelColor(pos+66,p.Color(0,0,255)); //5R
        p.setPixelColor(pos+82,p.Color(0,0,255)); //6R
        p.setPixelColor(pos+98,p.Color(0,0,255)); //7R
        break;
  
      case 2:
        p.setPixelColor(pos,p.Color(0,0,255));    //1L
        p.setPixelColor(pos+1,p.Color(0,0,255));  //1C
        p.setPixelColor(pos+2,p.Color(0,0,255));  //1R
        p.setPixelColor(pos+18,p.Color(0,0,255)); //2R
        p.setPixelColor(pos+34,p.Color(0,0,255)); //3R
        p.setPixelColor(pos+48,p.Color(0,0,255)); //4L
        p.setPixelColor(pos+49,p.Color(0,0,255)); //4C
        p.setPixelColor(pos+50,p.Color(0,0,255)); //4R
        p.setPixelColor(pos+64,p.Color(0,0,255)); //5L
        p.setPixelColor(pos+80,p.Color(0,0,255)); //6L
        p.setPixelColor(pos+96,p.Color(0,0,255)); //7L
        p.setPixelColor(pos+97,p.Color(0,0,255)); //7C
        p.setPixelColor(pos+98,p.Color(0,0,255)); //7R
        break;
  
      case 3:
        p.setPixelColor(pos,p.Color(0,0,255));    //1L
        p.setPixelColor(pos+1,p.Color(0,0,255));  //1C
        p.setPixelColor(pos+2,p.Color(0,0,255));  //1R
        p.setPixelColor(pos+18,p.Color(0,0,255)); //2R
        p.setPixelColor(pos+34,p.Color(0,0,255)); //3R
        p.setPixelColor(pos+48,p.Color(0,0,255)); //4L
        p.setPixelColor(pos+49,p.Color(0,0,255)); //4C
        p.setPixelColor(pos+50,p.Color(0,0,255)); //4R
        p.setPixelColor(pos+66,p.Color(0,0,255)); //5R
        p.setPixelColor(pos+82,p.Color(0,0,255)); //6R
        p.setPixelColor(pos+96,p.Color(0,0,255)); //7L
        p.setPixelColor(pos+97,p.Color(0,0,255)); //7C
        p.setPixelColor(pos+98,p.Color(0,0,255)); //7R
        break;
  
      case 4:
        p.setPixelColor(pos,p.Color(0,0,255));    //1L
        p.setPixelColor(pos+2,p.Color(0,0,255));  //1R
        p.setPixelColor(pos+16,p.Color(0,0,255)); //2L
        p.setPixelColor(pos+18,p.Color(0,0,255)); //2R
        p.setPixelColor(pos+32,p.Color(0,0,255)); //3L
        p.setPixelColor(pos+34,p.Color(0,0,255)); //3R
        p.setPixelColor(pos+48,p.Color(0,0,255)); //4L
        p.setPixelColor(pos+49,p.Color(0,0,255)); //4C
        p.setPixelColor(pos+50,p.Color(0,0,255)); //4R
        p.setPixelColor(pos+66,p.Color(0,0,255)); //5R
        p.setPixelColor(pos+82,p.Color(0,0,255)); //6R
        p.setPixelColor(pos+98,p.Color(0,0,255)); //7R
        break;


      case 5:
        p.setPixelColor(pos,p.Color(0,0,255));    //1L
        p.setPixelColor(pos+1,p.Color(0,0,255));  //1C
        p.setPixelColor(pos+2,p.Color(0,0,255));  //1R
        p.setPixelColor(pos+16,p.Color(0,0,255)); //2L
        p.setPixelColor(pos+32,p.Color(0,0,255)); //3L
        p.setPixelColor(pos+48,p.Color(0,0,255)); //4L
        p.setPixelColor(pos+49,p.Color(0,0,255)); //4C
        p.setPixelColor(pos+50,p.Color(0,0,255)); //4R
        p.setPixelColor(pos+66,p.Color(0,0,255)); //5R
        p.setPixelColor(pos+82,p.Color(0,0,255)); //6R
        p.setPixelColor(pos+96,p.Color(0,0,255)); //7L
        p.setPixelColor(pos+97,p.Color(0,0,255)); //7C
        p.setPixelColor(pos+98,p.Color(0,0,255)); //7R
        break;
  
      case 6:
        p.setPixelColor(pos,p.Color(0,0,255));    //1L
        p.setPixelColor(pos+1,p.Color(0,0,255));  //1C
        p.setPixelColor(pos+2,p.Color(0,0,255));  //1R
        p.setPixelColor(pos+16,p.Color(0,0,255)); //2L
        p.setPixelColor(pos+32,p.Color(0,0,255)); //3L
        p.setPixelColor(pos+48,p.Color(0,0,255)); //4L
        p.setPixelColor(pos+49,p.Color(0,0,255)); //4C
        p.setPixelColor(pos+50,p.Color(0,0,255)); //4R
        p.setPixelColor(pos+64,p.Color(0,0,255)); //5L
        p.setPixelColor(pos+66,p.Color(0,0,255)); //5R
        p.setPixelColor(pos+80,p.Color(0,0,255)); //6L
        p.setPixelColor(pos+82,p.Color(0,0,255)); //6R
        p.setPixelColor(pos+96,p.Color(0,0,255)); //7L
        p.setPixelColor(pos+97,p.Color(0,0,255)); //7C
        p.setPixelColor(pos+98,p.Color(0,0,255)); //7R
        break;
  
      case 7:
        p.setPixelColor(pos,p.Color(0,0,255));    //1L
        p.setPixelColor(pos+1,p.Color(0,0,255));  //1C
        p.setPixelColor(pos+2,p.Color(0,0,255));  //1R
        p.setPixelColor(pos+18,p.Color(0,0,255)); //2R
        p.setPixelColor(pos+34,p.Color(0,0,255)); //3R
        p.setPixelColor(pos+50,p.Color(0,0,255)); //4R
        p.setPixelColor(pos+66,p.Color(0,0,255)); //5R
        p.setPixelColor(pos+82,p.Color(0,0,255)); //6R
        p.setPixelColor(pos+98,p.Color(0,0,255)); //7R
        break;
  
      case 8:
        p.setPixelColor(pos,p.Color(0,0,255));    //1L
        p.setPixelColor(pos+1,p.Color(0,0,255));  //1C
        p.setPixelColor(pos+2,p.Color(0,0,255));  //1R
        p.setPixelColor(pos+16,p.Color(0,0,255)); //2L
        p.setPixelColor(pos+18,p.Color(0,0,255)); //2R
        p.setPixelColor(pos+32,p.Color(0,0,255)); //3L
        p.setPixelColor(pos+34,p.Color(0,0,255)); //3R
        p.setPixelColor(pos+48,p.Color(0,0,255)); //4L
        p.setPixelColor(pos+49,p.Color(0,0,255)); //4C
        p.setPixelColor(pos+50,p.Color(0,0,255)); //4R
        p.setPixelColor(pos+64,p.Color(0,0,255)); //5L
        p.setPixelColor(pos+66,p.Color(0,0,255)); //5R
        p.setPixelColor(pos+80,p.Color(0,0,255)); //6L
        p.setPixelColor(pos+82,p.Color(0,0,255)); //6R
        p.setPixelColor(pos+96,p.Color(0,0,255)); //7L
        p.setPixelColor(pos+97,p.Color(0,0,255)); //7C
        p.setPixelColor(pos+98,p.Color(0,0,255)); //7R
        break;
  
      case 9:
        p.setPixelColor(pos,p.Color(0,0,255));    //1L
        p.setPixelColor(pos+1,p.Color(0,0,255));  //1C
        p.setPixelColor(pos+2,p.Color(0,0,255));  //1R
        p.setPixelColor(pos+16,p.Color(0,0,255)); //2L
        p.setPixelColor(pos+18,p.Color(0,0,255)); //2R
        p.setPixelColor(pos+32,p.Color(0,0,255)); //3L
        p.setPixelColor(pos+34,p.Color(0,0,255)); //3R
        p.setPixelColor(pos+48,p.Color(0,0,255)); //4L
        p.setPixelColor(pos+49,p.Color(0,0,255)); //4C
        p.setPixelColor(pos+50,p.Color(0,0,255)); //4R
        p.setPixelColor(pos+66,p.Color(0,0,255)); //5R
        p.setPixelColor(pos+82,p.Color(0,0,255)); //6R
        p.setPixelColor(pos+96,p.Color(0,0,255)); //7L
        p.setPixelColor(pos+97,p.Color(0,0,255)); //7C
        p.setPixelColor(pos+98,p.Color(0,0,255)); //7R
        break;
  
      default:
        break;
    }
    
  }
  p.show();
}

void eraseNumber(int x, int y){
  
  for(int iy=0; iy<7; iy++){
    for(int ix=0; ix<7; ix++){
      int n = posToStrip(x+ix, y+iy);
      p.setPixelColor(n,p.Color(255,255,255));
    }
  }
}


void arrowNext(){
  int a = 0;
  
  for(int y=0; y<15; y++){
    
    if (y <= 7){
      a = y;
    }
    else {
      a = 7 - abs(7-y);     
    }
    
    for(int x=0; x<16; x++){
      int sur = (x + a + arrow_count) % 8;
      int pos = posToStrip(x,y);
      
      if(sur<5){
        p.setPixelColor(pos,p.Color(255,100,0));
      }
      else{
        p.setPixelColor(pos,p.Color(0,0,0));
      }
    }
  }

  p.show();
  arrow_count++;
  if(arrow_count >= 8){
    arrow_count = 0;
  }
}
