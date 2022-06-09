```
** These can be combined. For example:
** 100c355v : Set channel 100 to value 255.
```
```
void loop() {
  int c;

  while (!Serial.available());
  c = Serial.read();
  if ((c >= '0') && (c <= '9')) {
    value = 10 * value + c - '0';
  } else {
    if (c == 'c') channel = value;
    else if (c == 'v') {
      DmxSimple.write(channel, value);
      Serial.print("Ch:");
      Serial.print(channel);
      Serial.print(" Value:");
      Serial.print(value);
      Serial.println();
    }
    value = 0;
  }
}
```