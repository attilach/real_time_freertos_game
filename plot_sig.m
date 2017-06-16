% plot signal and spectrum

Fe=8000;
r=load('dtmf_example.txt');
len=length(r);

figure(1)
x=(0:len-1);
plot(x,r)


figure(2)
fft_prec=4096*4;
z=fft(r,fft_prec)/fft_prec;
f=(0:fft_prec/2-1)/fft_prec*Fe;
f_mod=abs(z(1:fft_prec/2));
plot(f, f_mod)
[m idx]=max(abs(z(1:fft_prec/2)));
title('Signal spectrum')
xlabel('Frequency [Hz]')
ylabel('Amplitude')