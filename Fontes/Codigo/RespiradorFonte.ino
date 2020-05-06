#include <Stepper.h>
#include <LiquidCrystal_I2C.h>

int botaoVolume = 3;
int botaoRpm = 4;
int Volume = 10;
int Rpm = 10;
bool IsIniciado = false;
const int stepsPerRevolution = 200;

#define chaveFimCurso 8
#define sensorIR 9

// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27, 16, 12);
Stepper myStepper(stepsPerRevolution, 6, 7);

void setup() {

	Serial.begin(9600);
	myStepper.setSpeed(600);

	pinMode(sensorIR, INPUT);
	pinMode(chaveFimCurso, INPUT_PULLUP); //DEFINE O PINO COMO ENTRADA / "_PULLUP" É PARA ATIVAR O RESISTOR INTERNO
	pinMode(botaoVolume, INPUT_PULLUP); // define o pino do botao como entrada "INPUT"
	pinMode(botaoRpm, INPUT_PULLUP); // define o pino do botao como entrada "INPUT"

	lcd.init();
	lcd.backlight();
	lcd.clear();
}

void loop() {

	String mensagemVolume = String("Volume:") + Volume;
	String mensagemRPM = String("RPM:") + Rpm;

	//Teste sensor IR
	bool statusIR = digitalRead(sensorIR);

	if (statusIR)
	{
		//Voltar ate o fim de curso.
		while (digitalRead(chaveFimCurso) == HIGH) {
			Serial.println(digitalRead(sensorIR));
			Serial.println(digitalRead(chaveFimCurso));
			Serial.println("voltando...");
			myStepper.step(-stepsPerRevolution);
		}
		return;
	}

	if (digitalRead(botaoVolume) == LOW) // Se o botão for pressionado
	{
		int tempPressao = key_read(3);
		if (tempPressao == 2)
		{
			Inicializando();
			return;
		}

		Volume = AumentarVolume();

		Serial.println(String("Volume:") + Volume);
		delay(500);
	}

	if (IsIniciado)
	{
		IniciarMotor(Rpm, Volume);
		PrintLCD(0, 14, "I", false);
	}
	else
	{
		PrintLCD(0, 14, "D", false);
	}

	PrintLCD(0, 0, mensagemVolume, false);

	if (digitalRead(botaoRpm) == LOW) // Se o botão for pressionado
	{
		Rpm = AumentarRpm();

		Serial.println(String("RPM:") + Rpm);
		delay(500);
	}
	PrintLCD(1, 0, mensagemRPM, false);

}

void Inicializando()
{
	PrintLCD(0, 0, "Iniciando em 3", true);

	delay(1000);
	PrintLCD(0, 0, "Iniciando em 2", true);

	delay(1000);
	PrintLCD(0, 0, "Iniciando em 1", true);

	delay(1000);
	PrintLCD(0, 0, "Iniciado", true);

	IsIniciado = true;
}

int AumentarVolume()
{
	Volume += 1;

	if (Volume > 38)
		Volume = 10;

	return Volume;
}

int AumentarRpm()
{
	Rpm += 1;

	if (Rpm > 25)
		Rpm = 10;

	return Rpm;
}

void IniciarMotor(int rpm, int volume)
{
	int velocidade = CalcularRPM(rpm);

	if (velocidade >= 500)
	{
		myStepper.setSpeed(velocidade);

		Serial.println("Empurrando");
		for (int i = 0; i <= volume; i++) {
			myStepper.step(stepsPerRevolution);
		}

		while (digitalRead(chaveFimCurso) == HIGH) {
			Serial.println("Soltando");
			myStepper.step(-stepsPerRevolution);
		}
		//delay(100);
	}
}

int CalcularRPM(int rpm)
{
	if (rpm == 10)
		return 600;
	else if (rpm == 11)
		return 700;
	else
		return 0;
}

void PrintLCD(int linha, int coluna, String msg, bool limparVisor)
{
	if (limparVisor)
		lcd.clear();
	lcd.setCursor(coluna, linha);
	lcd.print(msg);

}

int key_read(int pin) {
	int pressTime = 0;
	while (digitalRead(pin) == LOW) { // that means the key is pressed
		pressTime++;
		delay(1000);
		if (pressTime >= 3) {
			return 2;
		}
	}
	return 1;
}