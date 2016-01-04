// rotate star, test of conept to make rotating dispay controlled by keyboard inputs
// 's' toggles spin, 'd' toggles direction, the mouse controls colors
// 12/28/15

int inc=0;
boolean direction = true;
boolean spin = false;
float w, h, size, indexsize;
void setup(){

  size(400, 400);
  background(255);
  smooth();
  //fill(192);
  noStroke();
  
  //rect(width/2-20, height/2-20, 40, 40);
}

void drawStar(int nbrPoints, float cx, float cy, float odiam, 
    int skipPoints, boolean direction, boolean spin)
{
  float orad = odiam / 2.0;
  
  pushMatrix();
  translate(cx, cy);
  if(spin == true){      
    if(direction == false)
       rotate(radians(-inc));
    else
       rotate(radians(inc));
    }
    println("spin : " + spin + "direction : " + direction);
  float  a = TWO_PI / nbrPoints;
  fill((mouseX + inc) % 255, (mouseY + 100 + inc) % 255, (mouseX + mouseY) % 255, 160);
  beginShape();
  int  n = 0;
  for (int i = 0; i < nbrPoints; ++i) {
    vertex( cos( a * n) * orad, sin( a * n) * orad);
    n += skipPoints;
  }
  endShape(CLOSE);
  popMatrix();
  fill(0);
  ellipse(width/2, height/2, width/6, height/6);
  
}

void toggledirection(){
    if(direction  == true){
      direction = false;
      }
    else{
      direction = true;
      }
    }
  
void togglespin(){
    if(spin  == true){
      spin = false;
      }
    else{
      spin = true;
      }
    }
    
void checkkey() {
  if (key == 's' ) 
    togglespin();
  if(key == 'd')
    toggledirection();
  }


void draw(){
  int r,g,b;
  //pushMatrix();
  //translate(width/2, height/2);
  //rotate(radians(inc));
  background(0);
  if(keyPressed)
    checkkey();
  r = mouseX % 255;
  g = mouseY % 255;
  b = (mouseX + mouseY) % 255;
  fill(r, g, b);
  ellipse(width/2, height/2, width, height);
  //fill(0);
  //ellipse(width/2, height/2, width/6, height/6);
  //triangle(0,-20,20,20,-20,20);
  //triangle(-20,-20,20,-20,0,-20);
  w = width / 2;
  h = height / 2;
  indexsize = inc % ( 5 * width / 6);
  size = width / 6 + indexsize;
  drawStar(5, w, h, size, 17, direction, spin);
  //rect(-20, -20, 40, 40);
  //popMatrix();
  inc++;
  println(inc);
 
}



  