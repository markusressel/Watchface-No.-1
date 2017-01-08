# Watchface No. 1
A simple, modular watchface for the Pebble platform.

<img src="/screenshot.jpg" width="400">

# Content
The watchface currently contains the following layers:
* Time
* Date
* Battery (Text)
* Battery (filled Icon)
* Heartrate Monitor (animated Image + Text)
* Phone connection indicator (image)

# Theme
Currently there is no compagnion app for this watchface so there is no way to customize it without recompiling the whole thing.
However it is possible to set a theme in code using the following line in ```main.c```:
 
    static void init() {
      // set theme
      enum Theme theme = DARK;
      init_theme(theme);
      
      [...]
      
    }
    
You can find (and customize) the Theme definitions in ```theme.h``` and ```theme.c``` respectively.

# Fonts
There are multipe variables to specify the font that should be used for every component of the watchface. Just like with the Theme you can set them in ```Theme.c```.
