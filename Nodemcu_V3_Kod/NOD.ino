#include <SoftwareSerial.h> //Arduino ile seri haberleşmeyi başlatmak için SoftwareSerial kütüphanesini çağırıyoruz.
#include <ESP8266WiFi.h> // Nodemcu Wifi ağına bağlamak için ESP8266WiFi kütüphanesini çağırıyoruz.
#include <ESP8266WebServer.h> // Nodemcu ile sunucu oluşturmak veya oluşan sunucuyu denetlemek için ESP8266WebServer  kütüphanesini çağırıyoruz.

SoftwareSerial mySerial(D1, D2); //Nodemcu ile Arduino hangi pinler aracılığı ile haberleşileceğini tanımlıyoruz. 

const char* ag_adi = "Serhat";  //Wifi adını ag_adi değişkenine tanımlıyoruz.
const char* ag_sifresi = "12345678";  //wifi şifresini ag_sifresi değişkenine tanımlıyoruz.
int sensorDegeri = 0;  // potansiyometreden okunacak degeri saklamak icin 

ESP8266WebServer server(80); //80 portunu kullanarak bir web server nesnesi oluşturuyoruz.

void setup() {
  Serial.begin(115200); //Seri haberleşmeyi başlatıyoruz.
  delay(100);
  mySerial.begin(9600); //Arduino ile iletişimi sağlayacak olan mySerial isimli bir seri haberleşme başlatıyoruz.
  
  Serial.println("wifi'ya bağlanılıyor "); 
  Serial.println(ag_adi);

  WiFi.begin(ag_adi, ag_sifresi);  // Wifi taraması yapılır. Tarama sonucunda istenilen wifi ağı ile eşleşme gerçekleştirilir.

  while (WiFi.status() != WL_CONNECTED) {  // Eşleşmenin gerçekleşip bağlantının sağlandığı kontrol ediliyoe.
  delay(1000);
  Serial.print("."); // bağlantı sağlanana kadar ekranda "." çıkmasını sağlıyoruz.
  }
  Serial.println("");
  Serial.println("WiFi Bağlandı..!");
  Serial.print("IP Adresiniz: ");  
  Serial.println(WiFi.localIP()); //Nodemcu'nun IP'sini ekrana yazdırıyoruz.

  server.begin(); //Sunucuyu başlatıyoruz.
  Serial.println("HTTP Sunucusu Başladı");

}

void loop() {
  String deger = mySerial.readStringUntil('\r'); //mySerial seri haberleşmesi üzerinden gelen verileri deger değişkenine atar.
  Serial.println(deger); 
 
  server.handleClient(); // Alıcıdan gelen istekleri dinlemeyi başlatıyoruz.
  delay(1000);
  server.send(200, "text/html", SendHTML(deger)); // Sunucu 200 yani OK yanıtını döndürürse deger değişkenini yanıt olarak göndermesini sağlıyoruz.
}
String SendHTML(String deger){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Online Tarti</title>\n";
  ptr +="<style>html { font-family: Times, serif; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: black ;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 14px;color: green;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body bgcolor= white style=color:black>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>Online Tarti</h1>\n";
  
  ptr +="<p>Agirlik:";
  ptr +=deger;
  ptr +="<br>";

  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
