# Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`AtmosphericVector`](#class_atmospheric_vector) | A wrapper of vector which allows a user-defined maximum size.
`class `[`HSCM_PSI`](#class_h_s_c_m___p_s_i) | HSCM class based on [https://github.com/rodan/honeywell_hsc_ssc_i2c/blob/master/hsc_ssc_i2c.cpp](https://github.com/rodan/honeywell_hsc_ssc_i2c/blob/master/hsc_ssc_i2c.cpp).
`class `[`Logger`](#class_logger) | [Logger](#class_logger) for sensor data.
`class `[`PinCtrl`](#class_pin_ctrl) | Pin Controller class.
`class `[`PVC4000`](#class_p_v_c4000) | [PVC4000](#class_p_v_c4000) class for the [PVC4000](#class_p_v_c4000) or PVC4100 MEMs transducers.
`class `[`Reader`](#class_reader) | Binary reader of struct data.
`class `[`SensorPoller`](#class_sensor_poller) | 
`class `[`Writer`](#class_writer) | Writing class for outputing data into binary or text.
`struct `[`logType`](#structlog_type) | A log standard that is shared between the reader, logger, and writer classes.

# class `AtmosphericVector` 

```
class AtmosphericVector
  : public std::vector< T >
```  

A wrapper of vector which allows a user-defined maximum size.

https://stackoverflow.com/questions/9248728/vector-how-to-set-the-maximum-number-of-elements-and-find-the-current-count

#### Parameters
* `T`

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline void `[`setMaxSize`](#class_atmospheric_vector_1a54fe37c8110bbfdf68704acf2a7173dd)`(int maxSize)` | Set the capacity of the vector.
`public inline void `[`push_back`](#class_atmospheric_vector_1a7a106e4ea19953c8ac462e2694852770)`(const T & element)` | Append an element to the vector as long as the vector size is less than the capacity.
`public inline bool `[`isFilled`](#class_atmospheric_vector_1afae4276f4d9bd7a732e3a4750c4d7c8e)`()` | Check if the vector is filled to its capacity.

## Members

#### `public inline void `[`setMaxSize`](#class_atmospheric_vector_1a54fe37c8110bbfdf68704acf2a7173dd)`(int maxSize)` 

Set the capacity of the vector.

#### Parameters
* `maxSize`

#### `public inline void `[`push_back`](#class_atmospheric_vector_1a7a106e4ea19953c8ac462e2694852770)`(const T & element)` 

Append an element to the vector as long as the vector size is less than the capacity.

#### Parameters
* `element`

#### `public inline bool `[`isFilled`](#class_atmospheric_vector_1afae4276f4d9bd7a732e3a4750c4d7c8e)`()` 

Check if the vector is filled to its capacity.

#### Returns
true 

#### Returns
false

# class `HSCM_PSI` 

HSCM class based on [https://github.com/rodan/honeywell_hsc_ssc_i2c/blob/master/hsc_ssc_i2c.cpp](https://github.com/rodan/honeywell_hsc_ssc_i2c/blob/master/hsc_ssc_i2c.cpp).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public hscm_output_type `[`m_output_type`](#class_h_s_c_m___p_s_i_1a4fa8f62695d0d66e646088397fe9f336) | 
`public uint8_t `[`m_i2cAddress`](#class_h_s_c_m___p_s_i_1ae3bc375649408a1d480076fd595594f7) | 
`public int `[`m_tf_type`](#class_h_s_c_m___p_s_i_1a63d459ed11f0a38961f373f1d4edd10b) | 
`public int `[`m_pressure_type`](#class_h_s_c_m___p_s_i_1a0cca745c27711d1db3e37eaaf2624091) | 
`public  `[`HSCM_PSI`](#class_h_s_c_m___p_s_i_1a941174c0151507e8d8b72a0193a42e4a)`(int i2cAddress,int tf_type,int p_type)` | Construct the HSCM using the I2C address.
`public  `[`HSCM_PSI`](#class_h_s_c_m___p_s_i_1a677e240c8558cf83cb4f2dfc63e4576e)`(hscm_output_type output_type,int tf_type,int p_type)` | Construct the HSCM using the types listed in the datasheet.
`public uint8_t `[`read`](#class_h_s_c_m___p_s_i_1a031053cbe875f5cf94f45b69686d26ac)`()` | Poll the HSCM for new pressure and temperature data and check the status of the read.
`public float `[`pressure`](#class_h_s_c_m___p_s_i_1ad100480c035dcb4a57665a853c70b764)`()` | Return the ambient pressure (in psi) read by the HSCM.
`public float `[`temperature`](#class_h_s_c_m___p_s_i_1a85ee6c72fc17db6d5345fef5b367c6c1)`()` | Return the ambient temperature (in C) read by the HSCM.

## Members

#### `public hscm_output_type `[`m_output_type`](#class_h_s_c_m___p_s_i_1a4fa8f62695d0d66e646088397fe9f336) 

#### `public uint8_t `[`m_i2cAddress`](#class_h_s_c_m___p_s_i_1ae3bc375649408a1d480076fd595594f7) 

#### `public int `[`m_tf_type`](#class_h_s_c_m___p_s_i_1a63d459ed11f0a38961f373f1d4edd10b) 

#### `public int `[`m_pressure_type`](#class_h_s_c_m___p_s_i_1a0cca745c27711d1db3e37eaaf2624091) 

#### `public  `[`HSCM_PSI`](#class_h_s_c_m___p_s_i_1a941174c0151507e8d8b72a0193a42e4a)`(int i2cAddress,int tf_type,int p_type)` 

Construct the HSCM using the I2C address.

#### Parameters
* `i2cAddress` 

* `tf_type` 

* `p_type`

#### `public  `[`HSCM_PSI`](#class_h_s_c_m___p_s_i_1a677e240c8558cf83cb4f2dfc63e4576e)`(hscm_output_type output_type,int tf_type,int p_type)` 

Construct the HSCM using the types listed in the datasheet.

#### Parameters
* `output_type` 

* `tf_type` 

* `p_type`

#### `public uint8_t `[`read`](#class_h_s_c_m___p_s_i_1a031053cbe875f5cf94f45b69686d26ac)`()` 

Poll the HSCM for new pressure and temperature data and check the status of the read.

#### Returns
uint8_t

#### `public float `[`pressure`](#class_h_s_c_m___p_s_i_1ad100480c035dcb4a57665a853c70b764)`()` 

Return the ambient pressure (in psi) read by the HSCM.

#### Returns
float

#### `public float `[`temperature`](#class_h_s_c_m___p_s_i_1a85ee6c72fc17db6d5345fef5b367c6c1)`()` 

Return the ambient temperature (in C) read by the HSCM.

#### Returns
float

# class `Logger` 

[Logger](#class_logger) for sensor data.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`logType`](#structlog_type)` `[`logData`](#class_logger_1a4b71165774ee49293228f547869c325b) | 
`public  `[`Logger`](#class_logger_1af5561fd7ff77bb54d5d6cd797ebe2d28)`(int arraySize)` | Construct a [Logger](#class_logger) class, which is responsible for storing the time, low pressure, high pressure, and acceleraetion values.
`public int `[`init`](#class_logger_1a9a048968b68dcb6ad6de58d23b2eacfe)`()` | Initialize the [Logger](#class_logger) and flush all values.
`public void `[`pushData`](#class_logger_1aa9983a0826820827e31fcf8fb77636a7)`(float time,float lowPressure,float highPressure,float acceleration)` | Append the time, low pressure, high pressure, and acceleration into the logger's buffer.
`public void `[`flushArrays`](#class_logger_1a69480ae18f1c2961b07fb7cadcdd59d6)`()` | Clear the values from each buffer.
`public bool `[`isStructFilled`](#class_logger_1a5ba805b3d05cb8f2492251e2229600c5)`()` | Check if the buffer is filled by checking the time variable, which always stores data.

## Members

#### `public `[`logType`](#structlog_type)` `[`logData`](#class_logger_1a4b71165774ee49293228f547869c325b) 

#### `public  `[`Logger`](#class_logger_1af5561fd7ff77bb54d5d6cd797ebe2d28)`(int arraySize)` 

Construct a [Logger](#class_logger) class, which is responsible for storing the time, low pressure, high pressure, and acceleraetion values.

#### Parameters
* `arraySize`

#### `public int `[`init`](#class_logger_1a9a048968b68dcb6ad6de58d23b2eacfe)`()` 

Initialize the [Logger](#class_logger) and flush all values.

#### Returns
int

#### `public void `[`pushData`](#class_logger_1aa9983a0826820827e31fcf8fb77636a7)`(float time,float lowPressure,float highPressure,float acceleration)` 

Append the time, low pressure, high pressure, and acceleration into the logger's buffer.

#### Parameters
* `t` 

* `lp` 

* `hp` 

* `a`

#### `public void `[`flushArrays`](#class_logger_1a69480ae18f1c2961b07fb7cadcdd59d6)`()` 

Clear the values from each buffer.

#### `public bool `[`isStructFilled`](#class_logger_1a5ba805b3d05cb8f2492251e2229600c5)`()` 

Check if the buffer is filled by checking the time variable, which always stores data.

#### Returns
true 

#### Returns
false

# class `PinCtrl` 

```
class PinCtrl
  : public Protothread
```  

Pin Controller class.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`PinCtrl`](#class_pin_ctrl_1aa4cfc2d94200389c25ee1eeb2726a214)`(int flowClose1,int flowOpen1,int flowClose2,int flowOpen2,int ventOpen,int ventClose,int signal)` | Construct a PinController class with the pins corresponding to a valve or signal on the FEMTA Suborbital Experiment.
`public void `[`openFlowValve1`](#class_pin_ctrl_1a38719c52dcbe99f6230a51bf17f28f77)`()` | Open flow valve 1.
`public void `[`closeFlowValve1`](#class_pin_ctrl_1a58db08121a93246bd87410709b36a692)`()` | Close flow valve 1.
`public void `[`openFlowValve2`](#class_pin_ctrl_1a5e386cb600e543d75e8caf00355a3941)`()` | Open flow valve 2.
`public void `[`closeFlowValve2`](#class_pin_ctrl_1ad0ce7969311375d091aa06ce7621179a)`()` | Close flow valve 2.
`public void `[`openVentValve`](#class_pin_ctrl_1a6ef0214620aed42aa885e659be665d87)`()` | Open vent valve 1.
`public void `[`closeVentValve`](#class_pin_ctrl_1a147c686c6879b7e574569bbfea69dd70)`()` | Close vent valve 1.
`public void `[`signalStart`](#class_pin_ctrl_1aa658b5b491087b47e0c83ec4ceccc6e7)`()` | Signal the start of an experiment.
`public void `[`signalStop`](#class_pin_ctrl_1a2ac6386b567543305a3f82e14054c58d)`()` | Signal to stop the experiment.
`public int `[`init`](#class_pin_ctrl_1acb4a92935a1e3d525f4e29e8a6e607d7)`(bool willInitializeValves)` | Initialize the pin modes and set the initial state of the valves at pre-flight.
`public bool `[`Run`](#class_pin_ctrl_1a0959fe33d8bba4514287ed0d5c64a870)`()` | An overridden Protothread method which acts as a state machine.

## Members

#### `public  `[`PinCtrl`](#class_pin_ctrl_1aa4cfc2d94200389c25ee1eeb2726a214)`(int flowClose1,int flowOpen1,int flowClose2,int flowOpen2,int ventOpen,int ventClose,int signal)` 

Construct a PinController class with the pins corresponding to a valve or signal on the FEMTA Suborbital Experiment.

The flow and vent pins correspond to valves on the Propellant Management Experiment, while the signal pin is an output which is measured by the FEMTA Thruster Experiment and determines whether the Thruster Experiment should begin at the right time.

#### Parameters
* `flowClose1` closeSV1 

* `flowOpen1` openSV1 

* `flowClose2` closeSV2 

* `flowOpen2` openSV2 

* `ventClose` closeLV1 

* `ventOpen` openLV1 

* `signal` signal

#### `public void `[`openFlowValve1`](#class_pin_ctrl_1a38719c52dcbe99f6230a51bf17f28f77)`()` 

Open flow valve 1.

#### `public void `[`closeFlowValve1`](#class_pin_ctrl_1a58db08121a93246bd87410709b36a692)`()` 

Close flow valve 1.

#### `public void `[`openFlowValve2`](#class_pin_ctrl_1a5e386cb600e543d75e8caf00355a3941)`()` 

Open flow valve 2.

#### `public void `[`closeFlowValve2`](#class_pin_ctrl_1ad0ce7969311375d091aa06ce7621179a)`()` 

Close flow valve 2.

#### `public void `[`openVentValve`](#class_pin_ctrl_1a6ef0214620aed42aa885e659be665d87)`()` 

Open vent valve 1.

#### `public void `[`closeVentValve`](#class_pin_ctrl_1a147c686c6879b7e574569bbfea69dd70)`()` 

Close vent valve 1.

#### `public void `[`signalStart`](#class_pin_ctrl_1aa658b5b491087b47e0c83ec4ceccc6e7)`()` 

Signal the start of an experiment.

#### `public void `[`signalStop`](#class_pin_ctrl_1a2ac6386b567543305a3f82e14054c58d)`()` 

Signal to stop the experiment.

#### `public int `[`init`](#class_pin_ctrl_1acb4a92935a1e3d525f4e29e8a6e607d7)`(bool willInitializeValves)` 

Initialize the pin modes and set the initial state of the valves at pre-flight.

#### Returns
int

#### `public bool `[`Run`](#class_pin_ctrl_1a0959fe33d8bba4514287ed0d5c64a870)`()` 

An overridden Protothread method which acts as a state machine.

This checks if a certain pin is turned on and sets the pin to HIGH. After a certain time delay t_delay, set the pin to LOW.

#### Returns
true 

#### Returns
false

# class `PVC4000` 

[PVC4000](#class_p_v_c4000) class for the [PVC4000](#class_p_v_c4000) or PVC4100 MEMs transducers.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint16_t `[`m_i2cAddress`](#class_p_v_c4000_1ad8fd0e3cc1ba5b441258c2a893dc553f) | 
`public  `[`PVC4000`](#class_p_v_c4000_1ab31a08063d6c570167391800d86bef11)`(int address)` | Construct a [PVC4000](#class_p_v_c4000) class using an I2C address, if necessary.
`public int `[`read`](#class_p_v_c4000_1ac122759c1e4e377d672d54b661635509)`()` | Update the pressure and temperature values and return the status based on the raw values.
`public void `[`calibrate`](#class_p_v_c4000_1ab53fa675adb01243365a843116e69ff3)`()` | Poll the sensor for new pressure and temperature readings.
`public void `[`init`](#class_p_v_c4000_1a639d26c43bfd632608d5d6128e5ac752)`()` | Initialize the [PVC4000](#class_p_v_c4000).
`public float `[`baselineTemperature`](#class_p_v_c4000_1a52012201d7a9199558053865c9eadac8)`()` | 
`public float `[`temperature`](#class_p_v_c4000_1a59cc546886f0a720d3a03f0ff8bb2c4b)`()` | Obtain the temperature from the last read command.
`public float `[`pressure`](#class_p_v_c4000_1a91cce578ec12ebfc42f0b529cb886547)`()` | Obtain the pressure from the last read command.
`public uint16_t `[`get_raw_upper`](#class_p_v_c4000_1ab422024da4c792301181f537f066d7a4)`()` | Return a raw count corresponding to the most significant byte.
`public uint16_t `[`get_raw_lower`](#class_p_v_c4000_1adc333675bd99d8ce897ba721959607c5)`()` | Return a raw count corresponding to the least significant byte.

## Members

#### `public uint16_t `[`m_i2cAddress`](#class_p_v_c4000_1ad8fd0e3cc1ba5b441258c2a893dc553f) 

#### `public  `[`PVC4000`](#class_p_v_c4000_1ab31a08063d6c570167391800d86bef11)`(int address)` 

Construct a [PVC4000](#class_p_v_c4000) class using an I2C address, if necessary.

#### Parameters
* `address`

#### `public int `[`read`](#class_p_v_c4000_1ac122759c1e4e377d672d54b661635509)`()` 

Update the pressure and temperature values and return the status based on the raw values.

#### Returns
int

#### `public void `[`calibrate`](#class_p_v_c4000_1ab53fa675adb01243365a843116e69ff3)`()` 

Poll the sensor for new pressure and temperature readings.

#### `public void `[`init`](#class_p_v_c4000_1a639d26c43bfd632608d5d6128e5ac752)`()` 

Initialize the [PVC4000](#class_p_v_c4000).

#### `public float `[`baselineTemperature`](#class_p_v_c4000_1a52012201d7a9199558053865c9eadac8)`()` 

#### `public float `[`temperature`](#class_p_v_c4000_1a59cc546886f0a720d3a03f0ff8bb2c4b)`()` 

Obtain the temperature from the last read command.

#### Returns
float

#### `public float `[`pressure`](#class_p_v_c4000_1a91cce578ec12ebfc42f0b529cb886547)`()` 

Obtain the pressure from the last read command.

#### Returns
float

#### `public uint16_t `[`get_raw_upper`](#class_p_v_c4000_1ab422024da4c792301181f537f066d7a4)`()` 

Return a raw count corresponding to the most significant byte.

#### Returns
uint16_t

#### `public uint16_t `[`get_raw_lower`](#class_p_v_c4000_1adc333675bd99d8ce897ba721959607c5)`()` 

Return a raw count corresponding to the least significant byte.

#### Returns
uint16_t

# class `Reader` 

Binary reader of struct data.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`Reader`](#class_reader_1aa53c935378e501762119494dbd1fdffb)`(int chipSelect,int bufferSize)` | Construct a [Reader](#class_reader) class, which takes a chip number (usually 4) for where the SD card is located.
`public int `[`readFile`](#class_reader_1a28fe13eed13d02af3eb622de08604e87)`(const char * fileName)` | Read the file in the SD card and returns a check if it is available.
`public `[`logType`](#structlog_type)` `[`readVector`](#class_reader_1aa761bdab52a52b4a9f61be941cb779a7)`()` | Outputs the time, low pressure, high pressure, and acceleration values into a single, large vector for reading.
`public int `[`init`](#class_reader_1aefa8f6560711e520cd36ec0f21afc403)`()` | Initialize the SD card for reading.

## Members

#### `public  `[`Reader`](#class_reader_1aa53c935378e501762119494dbd1fdffb)`(int chipSelect,int bufferSize)` 

Construct a [Reader](#class_reader) class, which takes a chip number (usually 4) for where the SD card is located.

#### Parameters
* `chipSelect`

#### `public int `[`readFile`](#class_reader_1a28fe13eed13d02af3eb622de08604e87)`(const char * fileName)` 

Read the file in the SD card and returns a check if it is available.

#### Parameters
* `fileName` 

#### Returns
int

#### `public `[`logType`](#structlog_type)` `[`readVector`](#class_reader_1aa761bdab52a52b4a9f61be941cb779a7)`()` 

Outputs the time, low pressure, high pressure, and acceleration values into a single, large vector for reading.

The writer class stores data by struct, rather than by outputting to a large vector. To compensate for this, we first read each vector to a temporary vector, push each index of the temporary vector to the output vector, and repeat this process for each chunk of the buffer.

[https://stackoverflow.com/questions/31212680/c-how-to-write-several-simple-vectors-to-a-binary-file-in-one-shot](https://stackoverflow.com/questions/31212680/c-how-to-write-several-simple-vectors-to-a-binary-file-in-one-shot)
#### Returns
[logType](#structlog_type)

#### `public int `[`init`](#class_reader_1aefa8f6560711e520cd36ec0f21afc403)`()` 

Initialize the SD card for reading.

#### Returns
int

# class `SensorPoller` 

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int `[`pollRate`](#class_sensor_poller_1a03bb1b528db3a08ac6baf266e12deef4) | 
`public void `[`addToSensorVector`](#class_sensor_poller_1aa85a57a10c9605cd42b776c6286fcc65)`()` | 
`public float[] `[`readVector`](#class_sensor_poller_1a0b8145fbf9f50710d1c42943c049f9c0)`()` | 
`public  `[`SensorPoller`](#class_sensor_poller_1aa138651a728049e6c3a69053a7e7ee6a)`()` | 
`public void `[`init`](#class_sensor_poller_1a51cc1ae086e9b89287363cf1eb8ce23d)`()` | 
`public void `[`readAccelerometer`](#class_sensor_poller_1a4a3044944aa9049c2b8c4c5ffd3a4a34)`(float * vec)` | 
`public void `[`readLowAltBaro`](#class_sensor_poller_1aa2020c7d465f711303e678e9070dadab)`(float * pressure,float * temp)` | 
`public void `[`readHighAltBaro`](#class_sensor_poller_1ac1e38e730991296e93ebebb88245536a)`(float * pressure,float * temp)` | 
`public void `[`readPressureSensors`](#class_sensor_poller_1ad75cf23df4e4dd6b8c1e6d5cd22e4c2e)`(float * pressures,float * temperatures)` | 
`public void `[`readFlowMeter`](#class_sensor_poller_1a1785417aa3d67e67076340f92c29bd1c)`(float * flow)` | 

## Members

#### `public int `[`pollRate`](#class_sensor_poller_1a03bb1b528db3a08ac6baf266e12deef4) 

#### `public void `[`addToSensorVector`](#class_sensor_poller_1aa85a57a10c9605cd42b776c6286fcc65)`()` 

#### `public float[] `[`readVector`](#class_sensor_poller_1a0b8145fbf9f50710d1c42943c049f9c0)`()` 

#### `public  `[`SensorPoller`](#class_sensor_poller_1aa138651a728049e6c3a69053a7e7ee6a)`()` 

#### `public void `[`init`](#class_sensor_poller_1a51cc1ae086e9b89287363cf1eb8ce23d)`()` 

#### `public void `[`readAccelerometer`](#class_sensor_poller_1a4a3044944aa9049c2b8c4c5ffd3a4a34)`(float * vec)` 

#### `public void `[`readLowAltBaro`](#class_sensor_poller_1aa2020c7d465f711303e678e9070dadab)`(float * pressure,float * temp)` 

#### `public void `[`readHighAltBaro`](#class_sensor_poller_1ac1e38e730991296e93ebebb88245536a)`(float * pressure,float * temp)` 

#### `public void `[`readPressureSensors`](#class_sensor_poller_1ad75cf23df4e4dd6b8c1e6d5cd22e4c2e)`(float * pressures,float * temperatures)` 

#### `public void `[`readFlowMeter`](#class_sensor_poller_1a1785417aa3d67e67076340f92c29bd1c)`(float * flow)` 

# class `Writer` 

Writing class for outputing data into binary or text.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`Writer`](#class_writer_1a06639f3d8669309bb57aa89fe85de302)`(const char * name,const int chipSelect,const bool toBinary,const bool overwrite)` | Construct a [Writer](#class_writer) class, which is responsible for the taking [logType](#structlog_type) data and writing it to a file for post-processing. This takes some user parameters for writing to binary (false by default) or overwriting the log file (for testing purposes, false by default).
`public int `[`writeToFile`](#class_writer_1a4e59a184209bcd1133ccf030087fab7f)`(`[`logType`](#structlog_type)` data)` | Writes [logType](#structlog_type) data to a File through text or binary methods. If successful, the method returns 0. Otherwise, it fails at other integer values.
`public int `[`init`](#class_writer_1a3f99b88bff437fbe0b36afe7d32ef8c8)`()` | Initialize the writer by making sure the SD card can be detected. If not detected after 10 tries, time out. Also check to see if the user plans to overwrite the existing log file on initialization.

## Members

#### `public  `[`Writer`](#class_writer_1a06639f3d8669309bb57aa89fe85de302)`(const char * name,const int chipSelect,const bool toBinary,const bool overwrite)` 

Construct a [Writer](#class_writer) class, which is responsible for the taking [logType](#structlog_type) data and writing it to a file for post-processing. This takes some user parameters for writing to binary (false by default) or overwriting the log file (for testing purposes, false by default).

#### Parameters
* `name` 

* `chipSelect` 

* `toBinary` 

* `overwrite`

#### `public int `[`writeToFile`](#class_writer_1a4e59a184209bcd1133ccf030087fab7f)`(`[`logType`](#structlog_type)` data)` 

Writes [logType](#structlog_type) data to a File through text or binary methods. If successful, the method returns 0. Otherwise, it fails at other integer values.

#### Parameters
* `data` 

#### Returns
int

#### `public int `[`init`](#class_writer_1a3f99b88bff437fbe0b36afe7d32ef8c8)`()` 

Initialize the writer by making sure the SD card can be detected. If not detected after 10 tries, time out. Also check to see if the user plans to overwrite the existing log file on initialization.

#### Returns
int

# struct `logType` 

A log standard that is shared between the reader, logger, and writer classes.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`AtmosphericVector`](#class_atmospheric_vector)`< float > `[`time`](#structlog_type_1a99408840dfd21747ed6a6ac0b8010061) | 
`public `[`AtmosphericVector`](#class_atmospheric_vector)`< float > `[`lowPressure`](#structlog_type_1a3393cdfa35b7fc60c5d1c6b734bbed21) | 
`public `[`AtmosphericVector`](#class_atmospheric_vector)`< float > `[`highPressure`](#structlog_type_1ac427d4daf20af0f2756e54154951ed3d) | 
`public `[`AtmosphericVector`](#class_atmospheric_vector)`< float > `[`acceleration`](#structlog_type_1a07557f8ebc502e7ba07083a57f4337b5) | 

## Members

#### `public `[`AtmosphericVector`](#class_atmospheric_vector)`< float > `[`time`](#structlog_type_1a99408840dfd21747ed6a6ac0b8010061) 

#### `public `[`AtmosphericVector`](#class_atmospheric_vector)`< float > `[`lowPressure`](#structlog_type_1a3393cdfa35b7fc60c5d1c6b734bbed21) 

#### `public `[`AtmosphericVector`](#class_atmospheric_vector)`< float > `[`highPressure`](#structlog_type_1ac427d4daf20af0f2756e54154951ed3d) 

#### `public `[`AtmosphericVector`](#class_atmospheric_vector)`< float > `[`acceleration`](#structlog_type_1a07557f8ebc502e7ba07083a57f4337b5) 

Generated by [Moxygen](https://sourcey.com/moxygen)