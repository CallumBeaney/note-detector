idf_component_register(
  SRCS 
    "main.c"
    "config/i2s_config.c"
    "i2s_mic/i2s_mic.c"
    "signal_handlers/signal_handlers.c"
    "note_parsing/notes.c"
    "kissfft/kiss_fft.c" 
  
  INCLUDE_DIRS 
    "." 
    "config"
    "i2s_mic"
    "signal_handlers"
    "note_parsing"
    "kissfft"

  # REQUIRES 
    # "freertos"
    # "esp_hw_support"
    # "driver"
    # "esp_timer"
)