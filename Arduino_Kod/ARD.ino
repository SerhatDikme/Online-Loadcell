#include <HX711_ADC.h> //Loadcell kullanımı için HX711_ADC kütüphanesini ekliyoruz.
#include <LiquidCrystal_I2C.h> // Ekran kullanımı için LiquidCrystal_I2C kütüphanesini ekliyoruz.
HX711_ADC LoadCell(4, 5); // Loadcell ile haberleşmek için kullanacağımız pinleri tanımlıyoruz. (4=DT pin, 5=SCK pin)
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD Ekranın adresini ve tipini belirtiyoruz.

int dara = 6; // Dara almak için gerekli olan değişkeni tanımladık.
int a = 0;
float b = 0;

void setup() {
  Serial.begin(9600);//Seri haberleşmeyi başlatıyoruz.
  
  pinMode (dara, INPUT_PULLUP); // Dara değişkenine bağlı pinin bir input(giriş) pini olduğunu belirtiyoruz.
  
  LoadCell.begin(); // HX711 ile haberleşmeyi başlatıyoruz.
  LoadCell.start(1000); // Loadcelin stabileze olması için bir süre bekliyoruz.
  LoadCell.setCalFactor(416.31); // Kalibrasyon işleminde 100g ağırlık için verilen değeri giriyoruz.
  
  lcd.init(); // LCD modül ile haberleşmeyi başlatıyoruz  
  lcd.backlight(); // Arka plan ışığını aktif hale getiriyoruz.
  lcd.setCursor(0, 0); // İmleci ilk satıra getiriyoruz.
  lcd.print("-DIJITAL  TARTI-"); // İstenilen karakterleri ekrana yazdırıyoruz.
  lcd.setCursor(4, 1); // İmleci ikinci satırın 4. hanesine getiriyoruz
  lcd.print("MAX  5KG"); // İstenilen karakterleri ekrana yazdırıyoruz.
  delay(3000); // Yazıların okunabilmesi için 3 sn bekliyoruz.
  lcd.clear(); // Ekranı Temizliyoruz.
}

void loop() {
  lcd.setCursor(0, 0); 
  lcd.print("-DIJITAL  TARTI- "); 
  LoadCell.update(); // HX711 üzerinden veri almayı başlatıyoruz.
  float i = LoadCell.getData(); // verileri i değişkenine atıyoruz.
  //değerler eksi gelmesi durumunda pozitif olmasını sağlıyoruz.
 if (i<0)
 {
  i = 0; // eğer i negatif değer alırsa ekrana 0 yazılmasını sağlıyoruz.
  lcd.setCursor(3, 1);
  lcd.print("-");
  lcd.print(i,1); // Ölçülen değerin ekranda konumunu belirliyoruz. Aynı zaman seri port üzerinden bu veriyi nodemcuya gönderiyoruz.
  lcd.print("g ");
  lcd.setCursor(12, 1);
  lcd.print("-");
  Serial.println(i);
 }
 else
 {
  lcd.setCursor(3, 1);
  lcd.print("-");
  lcd.print(i,1); // Ölçülen değerin ekranda konumunu belirliyoruz. Aynı zaman seri port üzerinden bu veriyi nodemcuya gönderiyoruz. (Pozitif değerler için)
  lcd.print("g ");
  lcd.setCursor(12, 1);
  lcd.print("-");
  Serial.println(i);
 }
  
  if (i>=5000)
  {
  lcd.clear();
  lcd.setCursor(3, 0); // Maksimum yük durumda ekrana uyarı gelmesini sağlıyoruz.
  lcd.print("-MAX  YUK-"); 
  delay(200);
  }
  
  if (digitalRead (dara) == LOW) // Dara değişkeni LOW olduğunda(GND ile bağlantısı sağlandığında) dara alma işlemini başlatıyoruz.
  {
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("DARA ALINIYOR..."); 
    LoadCell.start(1000); // Dara alma işlemini başlatıyoruz.
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("-DARA  ALINDI-"); //Kulanıcıya dara alma işleminin bittiğinin bilgisini veriyoruz.
    delay(1000);
  }

}
