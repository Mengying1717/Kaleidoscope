let serial;
let portName = '/dev/tty.usbmodemHIDGD1';
let inData;
let splitString = [2,0,0,0,0];
let drawMode = 'DrawLine';
let symmetry = 6;
let angleTest = 1;
let angle = 360 / symmetry;
let showStrokeWeight;
let extraCanvas;
let freq = 400;

function setup(){
  cursor('images/paintBrush.png');

  serial = new p5.SerialPort('172.16.75.26');
  serial.on('data', serialEvent);
  serial.open(portName);
  angleMode(DEGREES);
  colorMode(HSB,255, 255, 255);

  createCanvas(900, 600);

  extraCanvas = createGraphics(180,200);
  extraCanvas.clear();

  background(0);
}

function serialEvent(){
  let currentString = serial.readLine();
  trim(currentString);
  if(!currentString) return;
  splitString = split(currentString, ',');
  splitString[0] = Number(splitString[0]);
  splitString[1] = Number(splitString[1]);
  splitString[2] = Number(splitString[2]);
  splitString[3] = Number(splitString[3]);
  splitString[4] = Number(splitString[4]);
}

function saveImage(){
  save('design.jpg');
  // serial.write('..*' + freq + ';');
  // console.log('..*' + freq + ';');
}

function draw(){
  image(extraCanvas,0,0);
  extraCanvas.background(0);
  extraCanvas.textSize(15);
  extraCanvas.noStroke();
  extraCanvas.fill(255,200,220);
  extraCanvas.text('Draw Mode: ' + drawMode, 20, 40);
  extraCanvas.noStroke();
  extraCanvas.text('Line Weight: '+ splitString[0], 20, 65);
  extraCanvas.fill(255,200,220);
  extraCanvas.rect(20, 80, 100, splitString[0], 20);

  if(splitString[1] == 1){
    saveImage();
    splitString[1] == 0;
  }
  if(splitString[2] == 1) {
    clean();
    splitString[2] == 0;
  }
  if(splitString[3] == 1) {
    drawShape();
  }
  if(splitString[4] == 1){
    drawLine();
  }
}

function clean(){
  background(0);
}

function drawShape(){
  drawMode = 'DrawShape';
  if (mouseX > 0 && mouseX < width && mouseY > 0 && mouseY < height){
    translate(width / 2, height / 2);
    let mx = mouseX - width / 2;
    let my = mouseY - height / 2;

      fill(mouseY, mouseX, 255, 0.3);
      noStroke();
      for(let i = 0; i < symmetry; i++){
        rotate(angle);
        push();
        let radius = random(0,50);
        ellipse(mx, my, radius, radius);
        pop();
        push();
        scale(1, -1);
        ellipse(mx, my, radius, radius);
        pop();
    }
  }
}

function drawLine(){
  drawMode = 'DrawLine';

  if (mouseX > 0 && mouseX < width && mouseY > 0 && mouseY < height){
    translate(width / 2, height / 2);
    let mx = mouseX - width / 2;
    let my = mouseY - height / 2;
    let pmx = pmouseX - width / 2;
    let pmy = pmouseY - height / 2;

    if(mouseIsPressed){

      stroke(mouseY, mouseX, 255, 255, 50);
      for(let i = 0; i < symmetry; i++){
        rotate(angle);
        strokeWeight(splitString[0]);
        push();
        line(mx, my, pmx, pmy);
        pop();
        push();
        scale(1, -1);
        line(mx, my, pmx, pmy);
        pop();
      }
    }
  }
}
