cool little pong thingy

download it at http://ntpd7cq74foxcjd3qsucdryix7pl7aacakd3shlegphbvayxjzoxu5qd.onion/pong/pong_lin_x64 (or x86), or at https://7dbb-51-6-19-154.ngrok-free.app/pong.pong_lin_x64 (again, or 32. it might be down as ngrok free is only temporary and i'm not gonna replace this every fucking time i reboot my pc), or through the github releases.

or compile yourself with

`sudo apt update -y`

`sudo apt install libsdl2-dev`

`gcc pong.c -o pong -lSDL2`

then (even if you got the prebuilt binary)

`chmod +x pong`

`./pong`
