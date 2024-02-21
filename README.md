TODO: pitch estimation, see: https://stackoverflow.com/a/34824467/18022338
TODO: OLED screen
TODO: multi-read buffer

This is an ESP-IDF project, for an ESP32-S3. It takes in mic audio and processes it, and prints the nearest note played

```
G#7/Ab7 ( - 88 )
```
to the monitor. In this case that's a 3234Hz frequency dominating the input signal, and it's 88Hz lower than a perfect Ab7.  
       
Eventually this will be a little OLED screen or something rather than a terminal tool. Still waiting for components in the mail.