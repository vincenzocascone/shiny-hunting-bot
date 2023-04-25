# 4th Gen Pokémon Shiny Hunting Bot

This Arduino project is designed to automate shiny hunting for legendary Pokémon in 4th generation games through soft resetting.

![DSC_9181](https://user-images.githubusercontent.com/94491363/151028208-cc47c95d-c850-4d97-adab-d11d3cb21ca8.jpg)

## Components

- Arduino board (main microcontroller)
- Two servo motors (press A button and START button)
- Photoresistor (detect light changes on Nintendo's screen)
- Two input buttons (save SRs count and change settings)
- LCD display (show SRs count and servo failure percentage)
- Elastic band (hold Nintendo's back buttons)
- Weight (hold down SELECT button)
- Lego bricks (bot's physical structure)

![DSC_9196](https://user-images.githubusercontent.com/94491363/151029009-72e00bdd-3384-4df0-9e77-08b9877b6db3.jpg)

## How It Works

The bot uses two servos to automate Pokémon encounters. The photoresistor detects light changes during a Pokémon fight scene. If the Pokémon is shiny, the encounter animation lasts longer, and the health bar is displayed with a slight delay. This delay is used to check for shininess.

https://user-images.githubusercontent.com/94491363/151034627-f73ab602-defb-47f1-a0d7-7a72ad3f1789.mp4

## Usage

1. Press the right button to start the bot.
2. View the SRs counter and fail percentage on the LCD display.
3. Press the left button to save the current session's SR count and access the settings menu.
4. Press both buttons to exit the settings menu.

|![MoltresShiny](https://user-images.githubusercontent.com/94491363/152589351-588121e2-8a8e-42b1-81b6-66b8ee25d786.jpg)|![MoltresShinyLCD](https://user-images.githubusercontent.com/94491363/152590698-a25d5c28-6564-4966-9e4b-b2c159cf09d3.jpg)|
