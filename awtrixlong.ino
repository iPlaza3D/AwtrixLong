esphome:
  name: awtrixlong
  friendly_name: Awtrix Long

psram:
  mode: octal

esp32:
  board: esp32-s3-devkitc-1
  flash_size: 16MB
  framework:
    type: esp-idf

logger:

api:
  encryption:
    key: "zxas0kb0nXrEo/UhXzCKwPZzk12WKSY0UQBZIjAqW4c="

ota:
  - platform: esphome
    password: "b5eaa87d91a11a07fc7eedc0c9881567"

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password
  manual_ip:
    static_ip: 192.168.1.100
    gateway: 192.168.1.1
    subnet: 255.255.255.0
  ap:
    ssid: "Awtrixlong Fallback Hotspot"
    password: "8Rq9wWvAfdzC"

mqtt:
  broker: 192.168.1.2
  username: !secret mqtt_username
  password: !secret mqtt_password

captive_portal:
web_server:

substitutions:
  logo: https://brands.home-assistant.io/homeassistant/icon.png
  background_color: 0x000000
  text_color: 0xFFFFFF
  text_color_secondary: 0x626262
  iddle_time: 10s
  greeting: "Home Assistant miniPanel"

  # ENTIDADES QUE SÍ TIENES
  weather_temperature: weather.forecast_casa # temperatura exterior
  home_temperature: sensor.a4c1384daab5_temperature # temperatura salón
  home_humidity: sensor.a4c1384daab5_humidity # humedad salón
  switch_01: switch.enchufe_01_enchufe # enchufe domotico 01
  climate_wall: climate.warm_wall_pro  # calefactor baño
  
  # NUEVAS ENTIDADES PARA MENÚ DE LUCES
  switch_exterior_lights: switch.luces_exteriores_enchufe # luces exteriores
  switch_garage_big: switch.garaje_interruptor_1 # luz garaje grande
  switch_garage_small: switch.garaje_interruptor_2 # luz garaje pequeño
  light_sofia_bedside: light.luz_sofi # lamparita mesilla Sofi

  # CARACTERES PERMITIDOS
  allowed_characters: " ¿?¡!#%'()+,-./:µ³°0123456789ABCDEFGHIJKLMNOPQRSTUVWYZabcdefghijklmnopqrstuvwxyzáéíóú"

font:
  - file: "gfonts://Kanit"
    id: font_clock
    size: 40
    glyphs: ${allowed_characters}
  - file: "gfonts://Kanit"
    id: font_date
    size: 20
    glyphs: ${allowed_characters}
  - file: "gfonts://Kanit"
    id: font_greeting
    size: 20
    glyphs: ${allowed_characters}
  - file: "gfonts://Material+Symbols+Outlined"
    id: font_home_icons
    size: 40
    glyphs: [
      "\U0000e430", # sol
      "\U0000e1ff", # termometro
      "\U0000f87e", # humedad
      "\U0000e58c", # menu
      "\U0000e879", # back
      " ",
    ]
  - file: "gfonts://Kanit"
    id: font_home_info
    size: 28
    glyphs: ${allowed_characters}
  - file: "gfonts://Material+Symbols+Outlined"
    id: font_device_icons
    size: 70
    glyphs: [
      "\U0000f02a", # lampara de techo
      "\U0000ef55", # llama fuego
      "\U0000f46c", # escalera
      "\U0000e531", # coche
      "\U0000e21e", # lamparita
      "\U0000f29e", # calefacción casa
      "\U0000f8c4", # radiador
      "\U0000efdd", # ducha
      " ",
    ]
  - file: "gfonts://Kanit"
    id: font_device_status
    size: 20
    glyphs: ${allowed_characters}

i2c:
  sda: 15
  scl: 10
  id: touchscreen_bus

image:
  - file: ${logo}
    id: logo
    resize: 90x90
    type: RGB565
    transparency: alpha_channel

interval:
  - interval: 60s
    then:
      - lvgl.label.update:
          id: lbl_date
          text: !lambda 'return id(esptime).now().strftime("%d/%m/%y");'
      - lvgl.label.update:
          id: lbl_clock
          text: !lambda 'return id(esptime).now().strftime("%H:%M");'

light:
  - platform: monochromatic
    output: backlight_pwm
    id: backlight
    name: "Display"
    icon: mdi:tablet
    restore_mode: ALWAYS_ON

output:
  - platform: ledc
    pin: GPIO1
    id: backlight_pwm

sensor:
  - platform: homeassistant
    id: home_icon_weather_temperature
    entity_id: ${weather_temperature}
    attribute: temperature
    internal: true
    on_value:
      then:
      - lvgl.label.update:
          id: lbl_weather_temperature
          text:
            format: "%.0f °C"
            args:
              - id(home_icon_weather_temperature).state
  
  - platform: homeassistant
    id: home_icon_home_temperature
    entity_id: ${home_temperature}
    internal: true
    on_value:
      then:
      - lvgl.label.update:
          id: lbl_home_temperature
          text:
            format: "%.0f °C"
            args:
              - id(home_icon_home_temperature).state
  
  - platform: homeassistant
    id: home_icon_home_humidity
    entity_id: ${home_humidity}
    internal: true
    on_value:
      then:
      - lvgl.label.update:
          id: lbl_home_humidity
          text:
            format: "%.0f%%"
            args:
              - id(home_icon_home_humidity).state

spi:
  - id: display_qspi
    type: quad
    clk_pin: 17
    data_pins:
      - 13
      - 18
      - 21
      - 14

text_sensor:
  - platform: homeassistant
    id: device_switch_01
    entity_id: ${switch_01}
    internal: true
    on_value:
      then:
        - lvgl.label.update:
            id: lbl_device_switch_01
            text_color: !lambda |-
              if (x == "on") {
                return lv_color_hex(0xffae00);  
              } else {
                return lv_color_hex(0x626262);  
              }

  # NUEVO: Sensor para warm wall pro
  - platform: homeassistant
    id: device_climate_wall
    entity_id: ${climate_wall}
    internal: true
    on_value:
      then:
        - lvgl.label.update:
            id: lbl_device_climate_wall
            text_color: !lambda |-
              if (x == "heat") {
                return lv_color_hex(0xffae00);  
              } else {
                return lv_color_hex(0x626262);  
              }

  # NUEVOS SENSORES PARA MENÚ DE LUCES
  - platform: homeassistant
    id: switch_exterior_lights_state
    entity_id: ${switch_exterior_lights}
    internal: true
    on_value:
      then:
        - lvgl.label.update:
            id: lbl_switch_exterior_lights
            text_color: !lambda |-
              if (x == "on") {
                return lv_color_hex(0xe6d754);  
              } else {
                return lv_color_hex(0x626262);  
              }

  - platform: homeassistant
    id: switch_garage_big_state
    entity_id: ${switch_garage_big}
    internal: true
    on_value:
      then:
        - lvgl.label.update:
            id: lbl_switch_garage_big
            text_color: !lambda |-
              if (x == "on") {
                return lv_color_hex(0xe6d754);  
              } else {
                return lv_color_hex(0x626262);  
              }

  - platform: homeassistant
    id: switch_garage_small_state
    entity_id: ${switch_garage_small}
    internal: true
    on_value:
      then:
        - lvgl.label.update:
            id: lbl_switch_garage_small
            text_color: !lambda |-
              if (x == "on") {
                return lv_color_hex(0xe6d754);  
              } else {
                return lv_color_hex(0x626262);  
              }

  - platform: homeassistant
    id: light_sofia_bedside_state
    entity_id: ${light_sofia_bedside}
    internal: true
    on_value:
      then:
        - lvgl.label.update:
            id: lbl_light_sofia_bedside
            text_color: !lambda |-
              if (x == "on") {
                return lv_color_hex(0xe6d754);  
              } else {
                return lv_color_hex(0x626262);  
              }

time:
  - platform: homeassistant
    id: esptime

touchscreen:
  - platform: axs15231
    id: main_touch
    display: main_display
    i2c_id: touchscreen_bus
    transform:
      mirror_x: true
      mirror_y: true
      swap_xy: false
    calibration:
      x_min: 0
      x_max: 640
      y_min: 0
      y_max: 180
    on_release:
      then:
        - if:
            condition: lvgl.is_paused
            then:
              - lvgl.page.show: page_home
              - light.turn_on: backlight
              - lvgl.resume:

display:
  - platform: mipi_spi
    id: main_display
    spi_id: display_qspi
    model: AXS15231
    dimensions:
      height: 640      
      width: 180     
    cs_pin: 12
    reset_pin: 16
    rotation: 90
    auto_clear_enabled: false

lvgl:
  displays:
    - main_display
  touchscreens:
    - main_touch
  buffer_size: 100%
  
  on_idle:
    - timeout: ${iddle_time}
      then:
        - lvgl.pause:
        - light.turn_off:
            id: backlight
  
  style_definitions:
    - id: button_menu
      width: 60
      height: 60
      pad_all: 0
      bg_color: 0x000000
      border_color: 0x000000
      align: BOTTOM_RIGHT
    
    - id: button_device_1
      width: 130
      height: 130
      pad_all: 0
      translate_x: 30
      bg_color: 0x000000
      border_color: 0x000000
      align: BOTTOM_LEFT
    
    - id: button_device_2
      width: 130
      height: 130
      pad_all: 0
      translate_x: 160
      bg_color: 0x000000
      border_color: 0x000000
      align: BOTTOM_LEFT
    
    - id: button_device_3
      width: 130
      height: 130
      pad_all: 0
      translate_x: 290
      bg_color: 0x000000
      border_color: 0x000000
      align: BOTTOM_LEFT
    
    - id: button_device_4
      width: 130
      height: 130
      pad_all: 0
      translate_x: 420
      bg_color: 0x000000
      border_color: 0x000000
      align: BOTTOM_LEFT

  pages:
    - id: page_home
      bg_color: ${background_color}
      widgets:
        - image:
            align: LEFT_MID
            src: logo
        
        - label:
            align: TOP_MID
            id: lbl_greeting
            text: "${greeting}"
            text_color: ${text_color_secondary}
            text_font: font_greeting
            pad_top: 20
        
        - label:
            align: TOP_RIGHT
            id: lbl_clock
            text: "00:00"
            text_color: ${text_color}
            text_font: font_clock
            pad_right: 20
        
        - label:
            align: TOP_RIGHT
            id: lbl_date
            text: "00/00/00"
            text_color: ${text_color_secondary}
            text_font: font_date
            pad_top: 45
            pad_right: 20

        - label:
            align: CENTER
            text_font: font_home_icons
            text_color: 0xe6d754
            text: "\U0000e430"  
            pad_top: 40
            pad_right: 220
        
        - label:
            id: lbl_weather_temperature
            align: CENTER
            text_font: font_home_info
            text_color: ${text_color}
            text: "20 °C"  
            pad_top: 40
            pad_right: 110
        
        - label:
            align: CENTER
            text_font: font_home_icons
            text_color: 0xe65476
            text: "\U0000e1ff"  
            pad_top: 40
        
        - label:
            id: lbl_home_temperature
            align: CENTER
            text_font: font_home_info
            text_color: ${text_color}
            text: "20 °C"  
            pad_top: 40
            pad_left: 100
        
        - label:
            align: CENTER
            text_font: font_home_icons
            text_color: 0x54c9e6
            text: "\U0000f87e"  
            pad_top: 40
            pad_left: 220
        
        - label:
            id: lbl_home_humidity
            align: CENTER
            text_font: font_home_info
            text_color: ${text_color}
            text: "50%"  
            pad_top: 40
            pad_left: 320

        - obj:
            styles: button_menu
            on_press:
              then:
                lvgl.page.show: page_devices
            widgets:
            - label:
                text_font: font_home_icons
                text_color: ${text_color_secondary}
                text: "\U0000e58c"

    - id: page_devices
      bg_color: ${background_color}
      widgets:
        # Botón 1: Luces (abre menú de luces)
        - obj:
            styles: button_device_1
            widgets:
            - label:
                text_font: font_device_icons
                text_color: ${text_color_secondary}
                text: "\U0000f02a"  
            on_press:
              then:
                lvgl.page.show: page_lights
        
        # Botón 2: Calefacción (abre menú calefacción)
        - obj:
            styles: button_device_2
            widgets:
            - label:
                text_font: font_device_icons
                text_color: ${text_color_secondary}
                text: "\U0000f29e"  
            on_press:
              then:
                lvgl.page.show: page_climate
        
        # Botones 3 y 4: VACÍOS
        - obj:
            styles: button_device_3
            widgets:
            - label:
                text_font: font_device_icons
                text_color: ${text_color_secondary}
                text: " "
        
        - obj:
            styles: button_device_4
            widgets:
            - label:
                text_font: font_device_icons
                text_color: ${text_color_secondary}
                text: " "
        
        # Botón para volver al home
        - obj:
            styles: button_menu
            on_press:
              then:
                lvgl.page.show: page_home
            widgets:
            - label:
                text_font: font_home_icons
                text_color: ${text_color_secondary}
                text: "\U0000e879"

    # PÁGINA: MENÚ DE LUCES
    - id: page_lights
      bg_color: ${background_color}
      widgets:
        # Título del menú
        - label:
            align: TOP_MID
            text: "PANEL DE LUCES GENERAL"
            text_color: ${text_color}
            text_font: font_greeting
            pad_top: 10
        
        # Botón 1: Luces exteriores
        - obj:
            styles: button_device_1
            widgets:
            - label:
                id: lbl_switch_exterior_lights
                text_font: font_device_icons
                text_color: ${text_color_secondary}
                text: "\U0000f46c"  
            on_press:
              then:
                - homeassistant.action:
                    service: switch.toggle
                    data:
                      entity_id: ${switch_exterior_lights}
        
        # Botón 2: Garaje grande
        - obj:
            styles: button_device_2
            widgets:
            - label:
                id: lbl_switch_garage_big
                text_font: font_device_icons
                text_color: ${text_color_secondary}
                text: "\U0000e531"  
            on_press:
              then:
                - homeassistant.action:
                    service: switch.toggle
                    data:
                      entity_id: ${switch_garage_big}
        
        # Botón 3: Garaje pequeño
        - obj:
            styles: button_device_3
            widgets:
            - label:
                id: lbl_switch_garage_small
                text_font: font_device_icons
                text_color: ${text_color_secondary}
                text: "\U0000e531"  
            on_press:
              then:
                - homeassistant.action:
                    service: switch.toggle
                    data:
                      entity_id: ${switch_garage_small}
        
        # Botón 4: Mesilla Sofi
        - obj:
            styles: button_device_4
            widgets:
            - label:
                id: lbl_light_sofia_bedside
                text_font: font_device_icons
                text_color: ${text_color_secondary}
                text: "\U0000e21e"  
            on_press:
              then:
                - homeassistant.action:
                    service: light.toggle
                    data:
                      entity_id: ${light_sofia_bedside}
        
        # Botón para volver atrás (menú dispositivos)
        - obj:
            styles: button_menu
            on_press:
              then:
                lvgl.page.show: page_devices
            widgets:
            - label:
                text_font: font_home_icons
                text_color: ${text_color_secondary}
                text: "\U0000e879"

    # NUEVA PÁGINA: MENÚ DE CALEFACCIÓN
    - id: page_climate
      bg_color: ${background_color}
      widgets:
        # Título del menú
        - label:
            align: TOP_MID
            text: "PANEL GENERAL CALEFACCION"
            text_color: ${text_color}
            text_font: font_greeting
            pad_top: 10
        
        # Botón 1: Clima Salon
        - obj:
            styles: button_device_1
            widgets:
            - label:
                id: lbl_device_switch_01
                text_font: font_device_icons
                text_color: ${text_color_secondary}
                text: "\U0000f8c4"  
            on_press:
              then:
                - homeassistant.action:
                    service: switch.toggle
                    data:
                      entity_id: ${switch_01}
        
        # Botón 2: Warm Wall Pro
        - obj:
            styles: button_device_2
            widgets:
            - label:
                id: lbl_device_climate_wall
                text_font: font_device_icons
                text_color: ${text_color_secondary}
                text: "\U0000efdd"  
            on_press:
              then:
                - homeassistant.action:
                    service: climate.toggle
                    data:
                      entity_id: ${climate_wall}
        
        # Botones 3 y 4: VACÍOS (para futuros dispositivos)
        - obj:
            styles: button_device_3
            widgets:
            - label:
                text_font: font_device_icons
                text_color: ${text_color_secondary}
                text: " "
        
        - obj:
            styles: button_device_4
            widgets:
            - label:
                text_font: font_device_icons
                text_color: ${text_color_secondary}
                text: " "
        
        # Botón para volver atrás (menú dispositivos)
        - obj:
            styles: button_menu
            on_press:
              then:
                lvgl.page.show: page_devices
            widgets:
            - label:
                text_font: font_home_icons
                text_color: ${text_color_secondary}
                text: "\U0000e879"
