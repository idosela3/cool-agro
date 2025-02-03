
# I'm Singing in the Rain

![Singing in the Rain](files/G_converted.gif)


Dolev Arav, Yehonatn Dimentman, Ido Sela, Liana Yohananov

#### *Components*
1. [1x PCB  5X7 cm(Printed Circuit Board)](https://sastronlimited.com/product/pcb-circuit-board-5x7cm-single-prototype-pcb-universal-printed-circuit-board-universal/)
2. [1x PCB 9x15 cm](https://hubtronics.in/9x15cm-double-sided-pcb-board)
3. [2x Boxes](https://www.digikey.co.il/he/products/detail/serpac/032,GY/307527?gclsrc=aw.ds&&utm_adgroup=&utm_source=google&utm_medium=cpc&utm_campaign=PMax_Product_L%20Runner&utm_term=&productid=307527&utm_content=&utm_id=go_cmp-20158096705_adg-_ad-__dev-c_ext-_prd-307527_sig-Cj0KCQiA4-y8BhC3ARIsAHmjC_E6ZxI3Hn3FR8iidUplO33CM01ca_AFMdk6UsEWYJNrJsl13fiAdEcaAjzlEALw_wcB&gad_source=1&gclid=Cj0KCQiA4-y8BhC3ARIsAHmjC_E6ZxI3Hn3FR8iidUplO33CM01ca_AFMdk6UsEWYJNrJsl13fiAdEcaAjzlEALw_wcB) 
4. [1x FireBeetle 2 ESP32-E](https://www.dfrobot.com/product-2195.html)
5. [1x botton](https://www.instructables.com/Read-a-Pushbutton-With-Digital-Input/)
6. [1x Buck convertor](https://www.pixelelectric.com/lm2596-dc-dc-buck-converter-step-down-power-module/) 
7. [1x Resistor 1KΩ](https://www.filotronix.com/fixed-value-resistors/279-1k-carbon-resistor.html) 
8. [1x OLED Screen](https://www.instructables.com/Monochrome-096-i2c-OLED-display-with-arduino-SSD13/) 
 
 sensors: 
1. [1x SHT30 Sensor (Temperature and RH)](https://nettigo.eu/products/sensirion-sht30-humidity-and-temperature-sensor-with-i2c-interface)   
2. [1x Rain Sensor](https://botland.store/weather-sensors/23474-gravity-rainfall-sensor-i2c-and-uart-dfrobot-sen0575-6959420923632.html)

## System Goals  
The system will irrigate the plot every day at 5 AM and 5 PM based on the total amount of rain that fell in the last 12 hours.  

- **0 mm** → Irrigates for **10 minutes**  
- **1-5 mm** → Irrigates for **5 minutes**  
- **5 mm and above** → **No irrigation**  


### External View
![External View](files/External1.jpg)



### Internal Components
#### Main Circuit Board
![Main Circuit Board](files/main.jpg)

### PCB  5X7 cm:
![Board](files/p.jpg)

### PCB 9x15 cm:
![Board2](files/p2.jpg)



### Our Circuuit Diagram

![Diagram](files/Diagram.jpg)



### Rain Sensor Description and Calculation Methodology

The rain sensor we are using operates based on gravity. It consists of a surface that drains water into a small container. Once a certain amount of water accumulates, the container tips due to the weight, producing a clicking sound.
Each click corresponds to X mm of rainfall.

#### Conversion Methodology
We will explain how the number of clicks is converted into milliliters (mL) and subsequently into millimeters (mm).














### Rain Sensor 
![Rain Sensor](files/Ra.jpg)

![Rain Sensor](files/Rai.jpg)

![Rain Sensor](files/R.jpg)










**Assumptions:**
1. **50 mL = 24 clicks** (experimentally verified in the lab).
2. **1 mL = 24/50 clicks**.
3. **1 liter of rain water = 1 Kg** (under assumptions)

To simplify the calculation, we compare the values obtained to a box with an area of 100 cm² and a height of 1 mm. This box serves as a model for rain falling outside the sensor. The box's volume is 1 liter, meaning that 1 liter of water fully fills the box, creating a 1-mm water layer. Assuming rain has filled the box, we can state:  
**1 L = 1 mm**.

The area of our sensor is:
**5 cm × 11 cm = 55 cm²**.

To determine the amount of water captured by the sensor, we use the ratio of the areas between the box and the sensor:

![Rain Sensor Formula](files/1.png)




Where:  
![Vbox](files/2.png)
 is the volume of water falling into the box, which is 1 liter.

By substituting the values into the formula:


![Vsensor](files/3.png)

Simplifying further:

![Simplifying further](files/4.png)

From this calculation, we observe that when 1 mm of rain falls into the box (or 1 liter), the amount of water collected by the sensor is 5.5 mL. Thus, the conversion factor is:

![Simplifying further](files/5.png)

#### Conversion from Clicks to mm
After determining the conversion factor between the rainfall in mm for the box and the volume in mL collected by the sensor, we now convert clicks to mL and subsequently to mm. To do so, we use **Assumption 2**.

Substituting **Assumption 2** into the formula:

![Box](files/6.png)

Isolating the value per click:

![Simplifying further](files/new7.png)


Thus, we find that each click represents:

![Simplifying further](files/new8.png)

This calculation provides the conversion rate from clicks to millimeters of rainfall.

---




##  Mashal🎥  
[![Watch the Video](Yair)](https://drive.google.com/file/d/1pzMQNw56-CucK9XnvPMlS3xP7YjBqYh9/view?usp=sharing)

### Yair's Full Explenation:
[![Watch the Video](Yair)](https://drive.google.com/file/d/1bnbMqHPZWbym1N-Tkr1z-iVA7Qd7LRhc/view?usp=sharing)



### Oled screen
The OLED screen is controlled by a button. When the button is pressed, the screen displays the data collected by the sensors in the past hour.


![Device Demo](files/vid_converted.gif)



## Sensor Data Visualization
Graphs representing the collected data from the sensors, including temperature, humidity, and rainfall.



### Temperature Graph
This graph represents the temperature data collected by the sensor.
![Temperature Graph](files/Temp.jpg)





### RH Graph
This graph illustrates the RH levels measured by the sensor.
![Humidity Graph](files/HR.jpg)





### Rain Graph
This graph shows the rainfall data captured by the rain sensor.
![Rain Graph](files/Rain.jpg)

### ThingSpeak Link:
[(ThingSpeak Link)](https://thingspeak.mathworks.com/channels/2799038/private_show)
