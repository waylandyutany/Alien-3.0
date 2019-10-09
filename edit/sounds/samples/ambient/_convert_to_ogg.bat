for %%X in (*.wav) do y:\Projects\alien\code\utils\audio\oggenc %%X -o ogg/%%X
cd ogg
ren *.wav *.ogg
cd ..
pause
