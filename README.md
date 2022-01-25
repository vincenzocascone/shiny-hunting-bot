# Shiny-Hunting-Bot-4th-gen

This is a simple Arduino build that is capable of shiny hunting legendaries in 4th generation pokemon games through soft resetting.

![DSC_9181](https://user-images.githubusercontent.com/94491363/151028208-cc47c95d-c850-4d97-adab-d11d3cb21ca8.jpg)

# How is it built

I only used some basic Arduino components such as:
- a basic Arduino board
- 2 servo motors that are used to press the nintendo A button and START button
- a photoresistor, used to sense a light change in the nintendo's screen
- 2 input buttons that you can use in order to save the current session's number of SRs or change some settings
- an LCD display that shows some info like the SRs counter and the servos fail %
- an elastic band to keep the nintendo's back buttons pressed and a weight to do the same for the SELECT button
- lastly I used some lego bricks to build the structure

![DSC_9196](https://user-images.githubusercontent.com/94491363/151029009-72e00bdd-3384-4df0-9e77-08b9877b6db3.jpg)


# How does it work

The system uses one servo to press the A button in order to go through the initial game intro, menus and finally the encounter. The other servo is used to soft reset 
after a non shiny encouter. In order to check the shininess of the pokemon it uses the photoresistor, which is placed in front of the screen, precisely pointing the area  where your pokemon's 
healthbar is located during the fight scene. When the healthbar displays, there's a change in light sensed by the photoresistor. If the pokemon is shiny, the animation 
of the encounter lasts longer, so the bar is displayed with a small delay that it's actually used to check the shininess. When the pokemon isn't shiny the whole cycle starts 
again with a soft reset. If the pokemon is shiny the bot stops and waits for you to try and catch it. In order to start the bot, the right arduino button must be pressed. While the bot is hunting you can see the SRs counter on the LCD 
display and you can also check if the servos are working fine through the fail % displayed. In order to save the number of SRs executed in the current session you must
press the left button after the START button is pressed and before the A button is pressed for the first time in the cycle. If you do so, besides saving, you also enter a 
menu in which you can set the light value that suits the best with the ambient light. In order to exit the menu you just need to press both the buttons together.
After you have saved you can just turn off the arduino board and the next time you turn it on the counter will start from the previous number saved.
