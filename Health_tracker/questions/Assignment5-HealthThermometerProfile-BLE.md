Please include your answers to the questions below with your submission, entering into the space below each question
See [Mastering Markdown](https://guides.github.com/features/mastering-markdown/) for github markdown formatting if desired.

*Be sure to take measurements with logging disabled to ensure your logging logic is not impacting current/time measurements.*

*Please include screenshots of the profiler window detailing each current measurement captured.  See the file Instructions to add screenshots in assignment.docx in the ECEN 5823 Student Public Folder.*

1. Provide screen shot verifying the Advertising period matches the values required for the assignment.
   <br>Ans: Yes, the advertising period is 250msec, and the energy profile shows 270 ms .
   <br>Screenshot:  
   ![advertising_period](https://github.com/CU-ECEN-5823/ecen5823-assignment5-samu7988/blob/master/images/advertising_period.PNG)  

2. What is the average current between advertisements?
   <br>Answer:222.23uA
   <br>Screenshot:  
   ![avg_current_between_advertisements](https://github.com/CU-ECEN-5823/ecen5823-assignment5-samu7988/blob/master/images/advertising_period.PNG)  

3. What is the peak current of an advertisement? 
   <br>Answer:26.32mA
   <br>Screenshot:  
   ![peak_current_of_advertisement](https://github.com/CU-ECEN-5823/ecen5823-assignment5-samu7988/blob/master/images/advertise_peak_current.PNG)  

4. Provide screen shot showing the connection interval settings. Do they match the values you set in your slave(server) code or the master's(client) values?.
  <br>Ans: The connection interval provided by client is 90msec as shown in first screenshot. The connection interval in the energy profile comes around 89.90msec  
   ![connection_interval_log](https://github.com/CU-ECEN-5823/ecen5823-assignment5-samu7988/blob/master/images/set_parameters_print.PNG)
  <br>Screenshot: 
   ![connection_interval](https://github.com/CU-ECEN-5823/ecen5823-assignment5-samu7988/blob/master/images/connection_interval.PNG)  

5. What is the average current between connection intervals?
   <br>Answer:190.01uA
   <br>Screenshot:
   ![avg_current_between_connection_intervals](https://github.com/CU-ECEN-5823/ecen5823-assignment5-samu7988/blob/master/images/connection_interval.PNG)  

6. If possible, provide screen shot verifying the slave latency matches what was reported when you logged the values from event = gecko_evt_le_connection_parameters_id. 
   <br>Screenshot: The slave latency provided by client is 300msec as shown in first screenshot. The slave latency in the energy profile comes around 359msec  
   ![slave_latency_log](https://github.com/CU-ECEN-5823/ecen5823-assignment5-samu7988/blob/master/images/set_parameters_print.PNG)
   ![slave_latency](https://github.com/CU-ECEN-5823/ecen5823-assignment5-samu7988/blob/master/images/slave_latency_300msec.PNG)  

7. What is the peak current of a data transmission when the phone is connected and placed next to the Blue Gecko? 
   <br>Answer:22.20mA
   <br>Screenshot:  
   ![peak_current_phone_next_to](https://github.com/CU-ECEN-5823/ecen5823-assignment5-samu7988/blob/master/images/phone_near_gecko.PNG)  
   
8. What is the peak current of a data transmission when the phone is connected and placed approximately 20 feet away from the Blue Gecko? 
   <br>Answer:24.17 mA
   <br>Screenshot:  
   ![peak_current_phone_20ft_away](https://github.com/CU-ECEN-5823/ecen5823-assignment5-samu7988/blob/master/images/phone_away_gecko.PNG)  
   