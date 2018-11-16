int circleR;
PGraphics img;
void setup() {
  size(270, 270, P3D);   // set any size here as long as it is square!
  println(width,',',height);
  circleR = width;
  exportImage();
}

void draw() {
  image(img, 0, 0);
}

void exportImage(){
  img = createGraphics(circleR, circleR, P3D);
  img.beginDraw();
  img.smooth();
  img.noStroke();
  img.translate(width/2, height/2);    // setting the center of the rotation to middle of the window
  img.noFill();
  //colorMode(HSB, 360, circleR, circleR); 
  for(int r = 0;r <= circleR; r++){
    for(int theta=0;theta <= 360; theta++){
      //img.stroke(color(theta, r, circleR));
      img.stroke(convertHSBtoRGB(theta, r/(float)circleR, 1));
      img.arc(0,0,r, r,radians(theta), radians(theta+1)); 
    }
  }
  img.endDraw();
  img.save("colorwheel.png");
}

void mouseClicked() {
  float correctedX = mouseX - (circleR/2);
  float correctedY = mouseY - (height/2);
  println(correctedX, ',', correctedY);
  float r = sqrt(pow(correctedX, 2) + pow(correctedY, 2));
  float theta = degrees(atan(correctedY/correctedX));
  println(r, ',', theta);
  //println("0x"+hex(convertHSBtoRGB(theta, r, 360)));
}

color convertHSBtoRGB(float H, float S, float B){
  // theta, r/circleR, circleR/circleR
  // HSB to RGB Formula
  // https://www.rapidtables.com/convert/color/hsv-to-rgb.html
  float C = S * B;
  float X = C * (1 - abs((H/60)%2 - 1));
  float m = B - C;
  float rp, gp, bp;
  if(H < 60){
    rp = C;
    gp = X;
    bp = 0;
  }
  else if(H < 120){
    rp = X;
    gp = C;
    bp = 0;
  }
  else if(H < 180){
    rp = 0;
    gp = C;
    bp = X;
  }
  else if(H < 240){
    rp = 0;
    gp = X;
    bp = C;
  }
  else if(H < 300){
    rp = X;
    gp = 0;
    bp = C;
  }
  else{
    rp = C;
    gp = 0;
    bp = X;
  }
  float red, green, blue;
  red = (rp+m)*255;
  green = (gp+m)*255;
  blue = (bp+m)*255;
  return color(red, green, blue);
}
