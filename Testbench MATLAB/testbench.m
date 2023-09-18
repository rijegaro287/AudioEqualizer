duracion = 2; % Duración en el tiempo de la señal de entrada

Fs = 44100;

F1 = 1500;
F2 = 2828;
F3 = 4000;
F4 = 5657;
F5 = 7500;

n = (0:(duracion * Fs - 1))'; % Vector de muestras

% Señales de entrada para probar el filtro
x_1 = sin(2*pi*(F1/Fs)*n);
x_2 = sin(2*pi*(F2/Fs)*n);
x_3 = sin(2*pi*(F3/Fs)*n);
x_4 = sin(2*pi*(F4/Fs)*n);
x_5 = sin(2*pi*(F5/Fs)*n);
x = x_1 + x_2 + x_3 + x_4 + x_5;


N = 8192; % N alto para aumentar la precisión del espectro usando FFT 
F = 0:(Fs/N):(Fs - Fs/N); % Vector de frecuencias para el espectro de las señales

salida = filtrar(x);

X_k = abs(fft(x, N)); % Espectro de la entrada

% Se dividen los espectros por el valor máximo de la entrada para que
% la escala sea de 0 a 1 y poder comparar las magnitudes de cada
% componente de frecuencia
Y_k = abs(fft(salida, N)) ./ max(X_k);
X_k = X_k ./ max(X_k);

% Grafica cada entrada con su respectiva salida en figuras distintas
figure(1)

subplot(2,1,1);
stem(F, X_k);
title('Espectro de la entrada');

subplot(2,1,2);
stem(F, Y_k);
title('Espectro de la salida');
