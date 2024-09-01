void initializeBME680() {
if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}

// = = = = = = ==============================================================================
//             sampleSensorData
// Samples the sensors and adds to the running average
// Stored in globals for now...
void sampleSensorData() {
 if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  float usePressure = bme.pressure / 100.0;
  pressureValue     = pressureFilter.filter(usePressure);

  float useTemperature = bme.temperature * 1.8 + 32.0;
  temperatureValue     = temperatureFilter.filter(useTemperature);

  float useHumidity = bme.humidity;
  humidityValue     = humidityFilter.filter(useHumidity);
  
  float useGas = bme.gas_resistance/1000;;
  gasValue     = gasFilter.filter(useGas);
}


