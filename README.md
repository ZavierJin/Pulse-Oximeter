# Pulse Oximeter

## Introduction<img src="https://github.com/ZavierJin/Pulse-Oximeter/blob/main/figure/figure_0.jpg" width = 20% height = 20% div align=right />

This project has built a complete set of **Wearable Continuous Pulse Oximeter** based on **PPG**(*Photoplethysmography*), and completed the development of corresponding mobile App. Its basic functions include signal acquisition, Bluetooth transmission, SpO2 calculation and real-time display.

The content in the repository is the code of the **hardware part** of the project. 

See this link for the code of mobile app: https://gitee.com/zzhcng/BloodOxygenCheck

## System Architecture

The system mainly includes MAX30105 blood oxygen sensor, LIS2DH12 motion sensor, nRF52832 MCU integrated with Bluetooth module and supporting Android mobile App.

The sensor detects the PPG signal and acceleration signal of the wrist in real time, and transmits the data to the Android mobile phone through Bluetooth. The mobile phone calculates the bleeding oxygen saturation through its own algorithm, and then transmits it back to the hardware through Bluetooth and displays it on the OLED. Meanwhile, the mobile app can display the waveform of PPG signal and current blood oxygen saturation in real time.

<img src="https://github.com/ZavierJin/Pulse-Oximeter/blob/main/figure/figure_2.jpg" width = 49% height = 49% div align=left /><img src="https://github.com/ZavierJin/Pulse-Oximeter/blob/main/figure/figure_1.png" width = 46% height = 46% />

## Signal Processing

The PPG signal is processed adaptively by twice filtering. Coarse filtering uses a filter with a large passband to filter out the noise that has a great impact on the PPG signal and find the fundamental frequency; Fine filtering adjusts the passband according to the fundamental frequency to better remove high-frequency and low-frequency noise. 

<img src="https://github.com/ZavierJin/Pulse-Oximeter/blob/main/figure/figure_3.png" width = 90% height = 90% />

In the calculation of blood oxygen value, the local peak valley method, spectrum method and regression algorithm are compared and analyzed. Finally, **the regression algorithm** with better experimental results is selected.

## Experiment

In the experiment, **12 subjects** were selected and **221 groups of effective data** were obtained.

At the same time, **Bland–Altman Plot** is used to evaluate the consistency between the experimental algorithm and the calibrated oximeter. The data within the boundary accounts for **95.57%**, indicating good consistency.

<img src="https://github.com/ZavierJin/Pulse-Oximeter/blob/main/figure/figure_7.jpg" width = 47% height = 47% /><img src="https://github.com/ZavierJin/Pulse-Oximeter/blob/main/figure/figure_8.jpg" width = 17% height = 17% /><img src="https://github.com/ZavierJin/Pulse-Oximeter/blob/main/figure/figure_9.jpg" width = 17% height = 17% /><img src="https://github.com/ZavierJin/Pulse-Oximeter/blob/main/figure/figure_10.jpg" width = 17% height = 17% />

