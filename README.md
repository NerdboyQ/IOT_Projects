# IOT_Projects
All IOT Projects will be stored in this repo moving forward
> NOTE: This project runs with the expectation that you already have python installed (v3.7 or better).


## BT Ghost Trap Lamp
_Objective: To design an Arduino powered LED Lamp, that can be controlled via a Flask webapp using bluetooth._

#### Functional Requirements:
* Flask Webapp 
   * UI elements to scan for noth BLE and non-BLE devices
   * Store previously connected devices in local database
   * UI elements to connect & communicate to bluetooth devices
* Arduino
    * Control the opening & closing of the ghost-trap door with a servo
    * Control RGB LED's for the lamp lighting settings
    * Communicate using bluetooth with the computer running the webapp

## Running the Webapp:
1. After cloning the repo, navigate to the flask subfolder \
`cd IOT_PROJECTS/BT_Lights/src/webapp/flask/bt_iot_webapp`
2. Run the command to start the webapp \
`flask --app bt_iot_webapp start`
    > NOTE : The app is built to automatically install all the python dependencies before starting the webapp.
3. If successfully started, the terminal window will display the IP address to go to in your web browser of choice. Once running, you can use this address on any devices connected to your local Wifi network.