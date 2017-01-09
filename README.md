# Watchface No. 1
A simple, modular watchface for the Pebble platform.

<img src="/screenshot_2.jpg" width="600">

# Content
The watchface currently contains the following layers:
* Time
  * Hour, Minute and Second (if enabled in configuration)
* Date
  * Showing current Weekday abbreviation, day of month and month
* Battery (Text)
  * Showing current battery level
* Battery (filled Icon)
  * Showing a little lightning icon when connected to charger
  * Animated when charging
* Heartrate Monitor (animated Image + Text)
  * Animated at the displayed heartrate
    * The animation will only play 10 times to improve battery life
    * The animation will play when opening the watchface or the heartrate value has changed
* Phone connection indicator (image)
  * Crossed out when disconnected from phone
* Weather (Text)
  * Showing current temperature

# Theme
The Theme can easily configured using the build in Settings screen, accessible in the Pebble app.
After saving your modified settings, the watchface needs to be restarted to reload the changed settings.
To do this you can open the Pebble main menu and close it right after. The watchface will then have to reinitialize and load all settings from scratch.

The ```Enable Animations``` option currently has <b>no effect</b> and animations are always on. This will change in a future release.

<img src="/settings.png" width="250">

These are the two main Themes:

<img src="/screen_dark_framed.png"><img src="/screen_light_framed.png">

# Fonts
There are multipe variables to specify the font that should be used for every component of the watchface. Currently those can only be changed in code. Have a look at the ```set_fonts()``` method in  ```Theme.c``` to modify them.
