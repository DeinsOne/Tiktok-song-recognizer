# Recognizer Bot

Recognizer bot can recognize song from tiktok videos which customer sent to the bot by a link. Predefined architecture provides facility to add more services for recognition. Contact the developer to add one (@deinsone).

 - Send tiktok video link
 - Send text with tiktok video link
 - Send multiple tiktok links

Recognizer bot is built over a simple handmade telegram bot toolchain, written in c++ to be fast and to deal with hard load. It provides multiple connections to serve many customers in the same time. It was built in educational purpose.

Bot can be found on telegram [@song_by_tt_bot](https://t.me/song_by_tt_bot).

# Third party services

Bot uses external services such as tiktok video downloader and song recognizer. I built simple api servers with heroku.com which might be unaccessible. Main feature of them are caching. It helps to increase speed of responses. [Recognizer](https://song-recognizer-service.herokuapp.com/) and [downloader](https://tt-audio-downloader.herokuapp.com/).


# Building and running

Bot ```ssl crypto zlib```

```
git clone https://github.com/DeinsOne/Tiktok-song-recognizer.git
cd Tiktok-song-recognizer

./utilities/install-modules.sh
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j4

./recognizer-bot-exe
```
