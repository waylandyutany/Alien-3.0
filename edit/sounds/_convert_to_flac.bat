for %%X in (*.wav) do y:\Projects\alien\code\utils\audio\flac %%X -o flac/%%X
cd flac
ren *.wav *.flac
cd ..
pause
