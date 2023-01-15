# Shiny-Hunting-Bot-4th-gen

This is a simple Arduino build that is capable of shiny hunting legendaries in 4th generation pokemon games through soft resetting.

![DSC_9181](https://user-images.githubusercontent.com/94491363/151028208-cc47c95d-c850-4d97-adab-d11d3cb21ca8.jpg)

# How is it built

I used a variety of basic components to build the Arduino-powered bot, including:

 - An Arduino board as the main microcontroller
 - Two servo motors, which are utilized to press the A button and START button on the Nintendo console
 - A photoresistor, which is used to detect changes in light on the Nintendo's screen during the encounter
 - Two input buttons, which can be used to save the current session's number of SRs and change settings
 - An LCD display that displays information such as the SRs counter and the servo failure percentage
 - An elastic band to hold the Nintendo's back buttons pressed and a weight to hold down the SELECT button
 - Lastly, I used Lego bricks to construct the bot's physical structure.

![DSC_9196](https://user-images.githubusercontent.com/94491363/151029009-72e00bdd-3384-4df0-9e77-08b9877b6db3.jpg)


# How does it work

The system uses two servos for automating the process of encountering Pokemon in a game. One servo is used to press the A button to navigate through the game's introduction, menus, and encounters. The other servo is used for a soft reset after encountering a non-shiny Pokemon. A photoresistor is placed in front of the screen to detect changes in light during a Pokemon fight scene. The photoresistor is specifically positioned to detect changes in the area of the screen where the Pokemon's health bar is displayed. If the Pokemon is shiny, the animation of the encounter lasts longer, and the health bar is displayed with a slight delay. This delay is used to check if the Pokemon is shiny. If the Pokemon is not shiny, the system will perform a soft reset and try again. If the Pokemon is shiny, the system will pause and wait for the user to attempt to catch it.


https://user-images.githubusercontent.com/94491363/151034627-f73ab602-defb-47f1-a0d7-7a72ad3f1789.mp4

# How to use it

The bot can be activated by pressing the right button on the Arduino board. While the bot is running, the SRs (soft reset) counter can be viewed on the LCD display. The fail percentage, which indicates the functioning of the servos, can also be viewed on the LCD display. To save the current session's SR count, the left button must be pressed after the start button is pressed and before the A button is pressed for the first time in the cycle. By doing this, you will also enter a menu where you can set the light value that works best with the ambient light. To exit the menu, press both buttons together. After the session is saved, you can turn off the Arduino board, and when you turn it on again, the SR counter will start from the previous saved number.


|![MoltresShiny](https://user-images.githubusercontent.com/94491363/152589351-588121e2-8a8e-42b1-81b6-66b8ee25d786.jpg)|![MoltresShinyLCD](https://user-images.githubusercontent.com/94491363/152590698-a25d5c28-6564-4966-9e4b-b2c159cf09d3.jpg)|
|----------------------------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------|

# Diamond and Pearl Starter version


https://user-images.githubusercontent.com/94491363/153247838-a96d05c6-e990-4b9b-b20d-eaae3d0a2917.mp4

