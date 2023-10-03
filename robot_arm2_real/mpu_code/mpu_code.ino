// #include <SoftwareSerial.h>
void setup(){
  Serial.begin(9600);
}
int main(){
  Serial.println("hello");
  delay(100);
}