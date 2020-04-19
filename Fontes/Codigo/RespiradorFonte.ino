

#include <Stepper.h>
const int stepsPerRevolution = 280;  
Stepper myStepper(stepsPerRevolution, 6, 7);

void setup() {

  myStepper.setSpeed(10);
  Serial.begin(9600);
}

int CalculaPressao(int valorPotenciometro)
{
  int valorPressao = 0;
  
  if (valorPotenciometro > 100 && valorPotenciometro <= 200)
  {
    valorPressao = 6;
  }
  else if (valorPotenciometro > 200 && valorPotenciometro <= 300)
  {
    valorPressao = 8;
  }
  else if (valorPotenciometro > 300 && valorPotenciometro <= 400)
  {
    valorPressao = 10;
  }
  else if (valorPotenciometro > 400 && valorPotenciometro <= 500)
  {
    valorPressao = 12;
  }
  else if (valorPotenciometro > 500 && valorPotenciometro <= 600)
  {
    valorPressao = 14;
  }
  else if (valorPotenciometro > 600 && valorPotenciometro <= 700)
  {
    valorPressao = 16;
  }
  else if (valorPotenciometro > 700 && valorPotenciometro <= 800)
  {
    valorPressao = 18;
  }
  else if (valorPotenciometro > 800 && valorPotenciometro <= 1000)
  {
    valorPressao = 20;
  }
  else
  {
    valorPressao = 0;
  }

  return valorPressao;
}

void loop() {
  delay(500);
  int velocidade = (analogRead(A0));
  int pressao = analogRead(A1);
  int totalPressao = CalculaPressao(pressao);

  Serial.println( String("-----------------"));
  Serial.println( String("Pressao Real:") + pressao);
  //velocidade = (velocidade/1.1);
  //velocidade = 600;
  Serial.println(String("Velocidade Motor:") + velocidade);
  Serial.println( String("Numero Pressao:") + totalPressao);

  Serial.println( String("-----------------"));

  if (velocidade >= 500)
  {
    myStepper.setSpeed(velocidade);

    Serial.println("Empurrando");
    for (int i = 0; i <= totalPressao; i++) {
      myStepper.step(stepsPerRevolution);
    }

    Serial.println("Soltando");
    for (int i = 0; i <= totalPressao; i++) {

      myStepper.step(-stepsPerRevolution);
    }
    //delay(100);
  }
}

