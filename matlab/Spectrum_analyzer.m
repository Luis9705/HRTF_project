%[y,Fs] = audioread('song_output.wav','native');
close all
[y,Fs] = audioread('../output/song_output.wav');
left_channel  = y(:,1);
right_channel = y(:,2);
t = linspace(0, numel(left_channel) / Fs, numel(left_channel));
figure;
plot(t, left_channel)
title('Left Channel')

f=linspace(-Fs/2,Fs/2,numel(t));
f = f(length(right_channel)/2:length(right_channel));

spectrum = fftshift(abs(fft(right_channel)));
spectrum = spectrum(length(right_channel)/2:length(right_channel));
figure;
semilogy(f, real(spectrum))
title('Right Channel')


spectrum = fftshift(abs(fft(left_channel)));
spectrum = spectrum(length(left_channel)/2:length(left_channel));
figure;
semilogy(f,real(spectrum))
title('Left Channel')