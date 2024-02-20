This is an ESP-IDF project, for an ESP32S3. It takes in an INMP441 mic input, FFTs it and prints out something like 

```
G#7/Ab7 ( - 88 )
```
to the monitor. In this case that's a 3234Hz frequency dominating the input signal, and it's 88Hz lower than a perfect Ab7.  
    
I'm using KISSFFT and older version of the I2S protocol for the ESP IDF for the sake of getting something running. Based on the obfuscation-level readability of the ESP-DSP library the former is probably here to stay.  
   
Eventually this will be a little OLED screen or something rather than a terminal tool. Still waiting for components in the mail.