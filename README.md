### ParticleDev

A vagrant machine for a passionate dad and all my particle/sparkcore dev tools and code for various projects to entertain his son.

 - MilkTime : Device to help my wife tracking how much milk Jeremy drinks and ..eh... a little more http://ourgreenbox.net/jeremy

 - PottyTime : Device to keep Jeremy entertain when going to potty train. Everytime he goes to toilet, he gets to click the button which says a welcome message and gives him an electronic badge in an oldschool 8x8 LED matrix. Most importantly, this button will trigger a timer and a slack notification will be sent to his parents 10 minutes later to remind him to go to toilet again (This is important in the first few weeks when he is not yet used to wearing undie)

### Usage:

`vagrant up` should be enough. :) 

the `firmware/user/applications` folder is soft-linked to `/home/vagrant/src/firmware/user/applications` inside vagrant machine so we can use IDE to modify files here and compile it inside the vagrant box. 

### Some cool examples

[Adafruit-LED-Backpack-Library](https://github.com/pkourany/Adafruit-LED-Backpack-Library)
[I2C LCD](https://community.particle.io/t/lcd-i2c-lib-info-customcharacters/4670)

### Some neat things

To create a (webhook)[https://api.slack.com/incoming-webhooks] integration for Slack:

Create a json file 
```
{
    "event": "slackbutton",
    "url": "https://hooks.slack.com/services/((AWESOME WEBHOOK URL))",
    "requestType": "POST",
    "json": {
        "username": "Pottytrain",
        "icon_emoji": ":poop:",
        "text": "{{SPARK_EVENT_VALUE}}"
    },
    "mydevices": "true"
}
```


Create a slackwebhook.json

`particle webhook create slackwebhook.json`


And now, in your code, to call this event, simply do:

```
Spark.publish("slackbutton", slackstring,60,PRIVATE);
```

Since spark always connect to its server, this will trigger the webhook ;)