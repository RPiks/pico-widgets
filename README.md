# A tiny GUI for Raspberry Pi Pico

The pico-widgets for Raspberry Pi Pico is a lightweight graphical user interface (GUI) which is based on Pico Touchscreen SDK https://github.com/RPiks/pico-touchscr-sdk.

![ezgif com-add-text](https://user-images.githubusercontent.com/47501785/222282840-043caf1d-04c9-4bf5-8bd5-c6e6c5f35afe.gif)

# A `Frame` concept
The pico-widgets is based on graphical rectangular frames which receive touch events and process them in highly customizable manner. Which allows to implement a vast range of widgets' types such as dialogs, lists and keyboard.

# Example Code

It is included.

# Quick-start

1. Install Raspberry Pi Pico SDK. Configure environment variables. Test
whether it is built successfully.

2. git clone this repository. 
cd pico-widgets
git submodule init
git submodule update
./build.sh

Check whether output file ./build/pico-widgets.uf2 appears.

3. Assemble the test board using pin connection information provided in test.c file of https://github.com/RPiks/pico-touchscr-sdk

4. Load the .uf2 file (2) into the Pico.

5. You now have the screen with the Top menu. Press 'PSK' in order to bring the keyboard up.

6. Use the SDK on your projects freely. Any possible contribution appreciated.

7. Please take in account that the project is still in development and many of its features might be a subject of change.
